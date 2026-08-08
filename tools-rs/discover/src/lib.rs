// Recursive-descent code discovery over a raw GBA image: seed on the reset
// vector and the header veneers, walk ARM and Thumb flow with a small
// constant-tracker so literal pools and jump tables resolve, then re-seed from
// pointer tables and prologue shapes until nothing new appears.
//
// WHY a Rust port: this walker is the bottom of the tool dependency graph.
// `tools/lib/overlay_disasm.ts` seeds a `Discovery` to decide which overlay
// bytes are code, and `overlay_inventory`, `overlay_call_order_check` and
// `executable_gap_sources` all build on that. None of them can leave
// TypeScript while `Discovery` only exists there, so this file is the unblock.
//
// Ported from tools/lib/discover.ts. The walk is byte-for-byte behavioural:
// the same seeds in the same order, the same instruction classification, the
// same jump-table heuristics, and a report whose canonical JSON is compared
// against the TypeScript writer's output in the parity run.

pub mod json;

use json::Json;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

pub const ROM_BASE: i64 = 0x0800_0000;

#[derive(Clone, Copy, PartialEq, Eq, Debug, PartialOrd, Ord, Hash)]
pub enum Mode {
    Arm,
    Thumb,
}

impl Mode {
    pub fn as_str(self) -> &'static str {
        match self {
            Mode::Arm => "arm",
            Mode::Thumb => "thumb",
        }
    }
}

/// `x & y` and `x << n` in JavaScript coerce through a signed 32-bit integer,
/// so `0x81000002 & ~1` is negative there and must be negative here too: the
/// value then fails `inside` and is dropped rather than being read as a
/// plausible pointer. Rust's `as i32` truncates (it does not saturate the way
/// a float cast would), which is exactly ToInt32.
fn and32(a: i64, b: i64) -> i64 {
    ((a as i32) & (b as i32)) as i64
}

/// `sx` from the TypeScript, transcribed with the same coercion: the `^` runs
/// in 32-bit space and the following subtraction does not, so the result can
/// leave the 32-bit range for a caller that then adds it to an address.
pub fn sx(value: i64, bits: u32) -> i64 {
    let sign = 1i64 << (bits - 1);
    (((value as i32) ^ (sign as i32)) as i64) - sign
}

/// PORT NOTE: faithful to `value.toString(16).padStart(8, "0")`, including its
/// odd treatment of a negative number (`-2` becomes `000000-2`). Every call
/// site passes a non-negative address, so the odd branch is unreachable in
/// practice; it is reproduced rather than "fixed" so a future caller sees the
/// same string the TypeScript would have produced.
fn hex(value: i64) -> String {
    let digits = if value < 0 {
        format!("-{:x}", -(value as i128))
    } else {
        format!("{value:x}")
    };
    if digits.len() >= 8 {
        digits
    } else {
        format!("{}{}", "0".repeat(8 - digits.len()), digits)
    }
}

#[derive(Clone, Copy, Debug)]
pub struct Instruction {
    pub size: i64,
    pub mode: Mode,
    pub kind: &'static str,
}

#[derive(Clone, Debug)]
pub struct FunctionInfo {
    pub entry: i64,
    pub mode: Mode,
    /// `[...fn.sources].sort()` in the report is JavaScript's default string
    /// sort, which is UTF-16 code-unit order. Every label is ASCII, where that
    /// coincides with byte order, so a `BTreeSet<String>` reproduces it.
    pub sources: BTreeSet<String>,
    pub blocks: BTreeSet<i64>,
    pub instructions: BTreeSet<i64>,
    pub callees: BTreeSet<i64>,
    pub external_callees: BTreeSet<i64>,
    pub unresolved: BTreeSet<i64>,
}

/// Insertion-ordered function table.
///
/// Every report field derived from it is sorted, so order looks free — but
/// `code_banks` counts banks in iteration order and `discover_prologue_boundaries`
/// then seeds bank by bank in that order, which decides the ORDER of the
/// `conflicts` array. A HashMap here would make `conflicts` nondeterministic.
#[derive(Default, Clone, Debug)]
struct FunctionTable {
    order: Vec<i64>,
    entries: HashMap<i64, FunctionInfo>,
}

impl FunctionTable {
    fn get(&self, address: i64) -> Option<&FunctionInfo> {
        self.entries.get(&address)
    }
    fn get_mut(&mut self, address: i64) -> Option<&mut FunctionInfo> {
        self.entries.get_mut(&address)
    }
    fn insert(&mut self, info: FunctionInfo) {
        self.entries.insert(info.entry, info);
    }
    fn len(&self) -> usize {
        self.order.len()
    }
    fn keys(&self) -> impl Iterator<Item = i64> + '_ {
        self.order.iter().copied()
    }
    fn values_mut(&mut self) -> impl Iterator<Item = &mut FunctionInfo> + '_ {
        self.entries.values_mut()
    }
    fn clear(&mut self) {
        self.order.clear();
        self.entries.clear();
    }
}

/// `Map`-backed set of `[source, target, mode]` triples, keyed the way the
/// TypeScript keys it (a string join), so the same duplicates collapse.
#[derive(Default, Clone, Debug)]
struct CallSet {
    order: Vec<(i64, i64, Mode)>,
    seen: HashSet<(i64, i64, Mode)>,
}

impl CallSet {
    fn add(&mut self, call: (i64, i64, Mode)) {
        if self.seen.insert(call) {
            self.order.push(call);
        }
    }
    fn len(&self) -> usize {
        self.order.len()
    }
}

/// Registers r0..r15 tracked as literal constants. The TypeScript uses a
/// `Map<number, number>` that is only ever get/set/delete/has'd by register
/// number, so a fixed array is observationally identical and copies cheaply
/// (the walker clones it per queued successor block).
type Constants = [Option<i64>; 16];

const NO_CONSTANTS: Constants = [None; 16];

/// What one `walk_*` call learned about the function being walked.
///
/// The TypeScript mutates `fn` in place while also mutating `this`, which Rust
/// will not allow through one `&mut self`. Nothing in the walk ever READS the
/// function's own sets (only `fn.entry` and `fn.mode`), so accumulating here
/// and merging at the end of `walk_function` is equivalent — and the targets
/// are all sets, so the merge cannot depend on order.
#[derive(Default)]
struct FnDelta {
    instructions: BTreeSet<i64>,
    callees: BTreeSet<i64>,
    external_callees: BTreeSet<i64>,
    unresolved: BTreeSet<i64>,
}

pub struct Discovery {
    pub data: Vec<u8>,
    pub base: i64,
    pub limit: i64,
    functions: FunctionTable,
    pub instructions: BTreeMap<i64, Instruction>,
    calls: CallSet,
    external_calls: CallSet,
    pub unresolved: BTreeSet<i64>,
    pub conflicts: Vec<(i64, Mode, Mode, String)>,
    pub data_refs: BTreeSet<i64>,
    pub literal_slots: BTreeSet<i64>,
    pub pointer_tables: BTreeMap<i64, Vec<i64>>,
    pub jump_tables: BTreeMap<i64, Vec<i64>>,
    pub jump_table_sites: BTreeMap<i64, i64>,
    pub blocked_data: BTreeSet<i64>,
}

impl Discovery {
    pub fn new(data: &[u8], base: i64) -> Self {
        Discovery {
            data: data.to_vec(),
            base,
            limit: base + data.len() as i64,
            functions: FunctionTable::default(),
            instructions: BTreeMap::new(),
            calls: CallSet::default(),
            external_calls: CallSet::default(),
            unresolved: BTreeSet::new(),
            conflicts: Vec::new(),
            data_refs: BTreeSet::new(),
            literal_slots: BTreeSet::new(),
            pointer_tables: BTreeMap::new(),
            jump_tables: BTreeMap::new(),
            jump_table_sites: BTreeMap::new(),
            blocked_data: BTreeSet::new(),
        }
    }

    pub fn function_count(&self) -> usize {
        self.functions.len()
    }

    pub fn call_count(&self) -> usize {
        self.calls.len()
    }

    pub fn external_call_count(&self) -> usize {
        self.external_calls.len()
    }

    pub fn function(&self, address: i64) -> Option<&FunctionInfo> {
        self.functions.get(address)
    }

    pub fn inside(&self, address: i64, size: i64) -> bool {
        self.base <= address && address + size <= self.limit
    }

    /// PORT NOTE: Node's `readUInt16LE` throws a catchable `RangeError` on an
    /// out-of-range offset; this panics. Every call site in the walk is
    /// guarded by `inside`, and the two that are not (`thumb_jump_table_extended`
    /// reaching below `base` for a function that starts at the very first byte
    /// of the image) are unreachable for any image whose first bytes are the
    /// reset vector.
    pub fn u16(&self, address: i64) -> i64 {
        let offset = (address - self.base) as usize;
        assert!(
            address >= self.base && offset + 2 <= self.data.len(),
            "u16 out of range at 0x{}",
            hex(address)
        );
        (self.data[offset] as i64) | ((self.data[offset + 1] as i64) << 8)
    }

    pub fn u32(&self, address: i64) -> i64 {
        let offset = (address - self.base) as usize;
        assert!(
            address >= self.base && offset + 4 <= self.data.len(),
            "u32 out of range at 0x{}",
            hex(address)
        );
        (self.data[offset] as i64)
            | ((self.data[offset + 1] as i64) << 8)
            | ((self.data[offset + 2] as i64) << 16)
            | ((self.data[offset + 3] as i64) << 24)
    }

    fn read_u32_at_offset(&self, offset: usize) -> i64 {
        (self.data[offset] as i64)
            | ((self.data[offset + 1] as i64) << 8)
            | ((self.data[offset + 2] as i64) << 16)
            | ((self.data[offset + 3] as i64) << 24)
    }

    pub fn add_seed(&mut self, address: i64, mode: Mode, source: &str) -> bool {
        let address = if mode == Mode::Thumb {
            and32(address, !1)
        } else {
            address
        };
        if !self.inside(address, if mode == Mode::Thumb { 2 } else { 4 }) {
            return false;
        }
        if self.blocked_data.contains(&address) {
            return false;
        }
        if let Some(old) = self.functions.get(address) {
            let old_mode = old.mode;
            if old_mode != mode {
                self.conflicts
                    .push((address, old_mode, mode, source.to_string()));
                return false;
            }
            self.functions
                .get_mut(address)
                .expect("just found")
                .sources
                .insert(source.to_string());
            return false;
        }
        self.functions.order.push(address);
        self.functions.insert(FunctionInfo {
            entry: address,
            mode,
            sources: BTreeSet::from([source.to_string()]),
            blocks: BTreeSet::new(),
            instructions: BTreeSet::new(),
            callees: BTreeSet::new(),
            external_callees: BTreeSet::new(),
            unresolved: BTreeSet::new(),
        });
        true
    }

    pub fn initial_seeds(&mut self) -> i64 {
        let reset = self.u32(self.base);
        assert!(
            (reset >> 24) == 0xea,
            "reset vector is not an unconditional ARM branch"
        );
        let entry = self.base + 8 + (sx(reset & 0xffffff, 24) << 2);
        self.add_seed(entry, Mode::Arm, "reset");
        let stop = std::cmp::min(entry + 0x100, self.limit);
        let mut address = entry;
        while address < stop {
            let value = self.u32(address);
            if (value & 1) != 0 && self.inside(and32(value, !1), 2) {
                self.add_seed(value, Mode::Thumb, "startup-literal");
            }
            address += 4;
        }
        let mut veneer = self.base + 0xc0;
        while veneer + 8 <= entry && self.u32(veneer) == 0x4720_4c00 {
            self.add_seed(veneer, Mode::Thumb, "header-veneer");
            let target = self.u32(veneer + 4);
            if (target & 1) != 0 {
                self.add_seed(target, Mode::Thumb, "header-veneer-target");
            }
            veneer += 8;
        }
        entry
    }

    fn note_instruction(
        &mut self,
        mode: Mode,
        delta: &mut FnDelta,
        address: i64,
        size: i64,
        kind: &'static str,
    ) -> bool {
        if let Some(old) = self.instructions.get(&address) {
            if old.mode != mode {
                let old_mode = old.mode;
                self.conflicts
                    .push((address, old_mode, mode, "flow".to_string()));
                return false;
            }
        }
        self.instructions
            .insert(address, Instruction { size, mode, kind });
        delta.instructions.insert(address);
        true
    }

    fn call(&mut self, delta: &mut FnDelta, source: i64, target: i64, mode: Mode) {
        let target = if mode == Mode::Thumb {
            and32(target, !1)
        } else {
            target
        };
        if !self.inside(target, if mode == Mode::Thumb { 2 } else { 4 }) {
            delta.external_callees.insert(target);
            self.external_calls.add((source, target, mode));
            return;
        }
        delta.callees.insert(target);
        self.calls.add((source, target, mode));
        self.add_seed(target, mode, &format!("call:{}", hex(source)));
    }

    #[allow(clippy::too_many_arguments)]
    fn walk_arm(
        &mut self,
        start: i64,
        delta: &mut FnDelta,
        queue: &mut Vec<(i64, Constants)>,
        seen: &mut HashSet<i64>,
        initial: &Constants,
    ) {
        let mode = Mode::Arm;
        let mut pc = start;
        let mut constants = *initial;
        while self.inside(pc, 4) && !seen.contains(&pc) {
            if self.blocked_data.contains(&pc) || self.blocked_data.contains(&(pc + 2)) {
                return;
            }
            seen.insert(pc);
            let word = self.u32(pc);
            let cond = word >> 28;
            let mut kind = "linear";
            let mut successors: Vec<i64> = Vec::new();
            let mut stop = false;

            if ((word >> 26) & 3) == 1 && (word & (1 << 25)) == 0 {
                let load = (word & (1 << 20)) != 0;
                let base_register = (word >> 16) & 15;
                let destination = ((word >> 12) & 15) as usize;
                let preindexed = (word & (1 << 24)) != 0;
                let writeback = (word & (1 << 21)) != 0;
                if load {
                    constants[destination] = None;
                }
                if load && base_register == 15 && preindexed && !writeback {
                    let displacement = word & 0xfff;
                    let mut literal = pc + 8;
                    literal += if (word & (1 << 23)) != 0 {
                        displacement
                    } else {
                        -displacement
                    };
                    if self.inside(literal, 4) {
                        self.literal_slots.insert(literal);
                        let value = self.u32(literal);
                        constants[destination] = Some(value);
                        if self.inside(and32(value, !1), 2) {
                            self.data_refs.insert(and32(value, !1));
                        }
                    }
                }
            }

            if (word & 0x0e00_0000) == 0x0a00_0000 {
                let link = (word & 0x0100_0000) != 0;
                let target = pc + 8 + (sx(word & 0xffffff, 24) << 2);
                kind = if link { "call" } else { "branch" };
                if link {
                    self.call(delta, pc, target, Mode::Arm);
                    for register in [0usize, 1, 2, 3, 12, 14] {
                        constants[register] = None;
                    }
                } else {
                    successors.push(target);
                    stop = cond == 0xe;
                }
                if cond != 0xe {
                    successors.push(pc + 4);
                    stop = true;
                }
            } else if (word & 0x0fff_fff0) == 0x012f_ff10 {
                let register = (word & 15) as usize;
                if register == 14 {
                    kind = "return";
                } else {
                    kind = "indirect";
                    if let Some(value) = constants[register] {
                        self.call(
                            delta,
                            pc,
                            value,
                            if (value & 1) != 0 {
                                Mode::Thumb
                            } else {
                                Mode::Arm
                            },
                        );
                    } else {
                        delta.unresolved.insert(pc);
                        self.unresolved.insert(pc);
                    }
                }
                stop = true;
            } else if (word & 0x0fff_ffff) == 0x01a0_f00e {
                kind = "return";
                stop = true;
            } else if (word & 0x0e10_f000) == 0x0410_f000 {
                kind = "indirect";
                delta.unresolved.insert(pc);
                self.unresolved.insert(pc);
                stop = true;
            } else if (word & 0x0e10_0000) == 0x0810_0000 && (word & (1 << 15)) != 0 {
                kind = "return";
                stop = true;
            } else if ((word >> 26) & 3) == 0 {
                let opcode = (word >> 21) & 15;
                let destination = (word >> 12) & 15;
                if destination == 15 && !matches!(opcode, 8..=11) {
                    kind = "indirect";
                    delta.unresolved.insert(pc);
                    self.unresolved.insert(pc);
                    stop = true;
                }
            }
            if !self.note_instruction(mode, delta, pc, 4, kind) {
                return;
            }
            for target in successors {
                if self.inside(target, 4) {
                    queue.push((target, constants));
                }
            }
            if stop {
                return;
            }
            pc += 4;
        }
    }

    #[allow(clippy::too_many_arguments)]
    fn walk_thumb(
        &mut self,
        start: i64,
        entry: i64,
        delta: &mut FnDelta,
        queue: &mut Vec<(i64, Constants)>,
        seen: &mut HashSet<i64>,
        initial: &Constants,
    ) {
        let mode = Mode::Thumb;
        let mut pc = start;
        let mut constants = *initial;
        while self.inside(pc, 2) && !seen.contains(&pc) {
            if self.blocked_data.contains(&pc) {
                return;
            }
            seen.insert(pc);
            let half = self.u16(pc);
            let mut kind = "linear";
            let mut size = 2i64;
            let mut successors: Vec<i64> = Vec::new();
            let mut stop = false;
            let mut written: Vec<usize> = Vec::new();
            let write = |register: i64, written: &mut Vec<usize>| {
                let register = register as usize;
                if !written.contains(&register) {
                    written.push(register);
                }
            };
            if (half & 0xe000) == 0x0000 {
                write(half & 7, &mut written);
            } else if (half & 0xe000) == 0x2000 {
                if (half & 0xf800) != 0x2800 {
                    write((half >> 8) & 7, &mut written);
                }
            } else if (half & 0xfc00) == 0x4000 {
                let operation = (half >> 6) & 15;
                if !matches!(operation, 8 | 10 | 11) {
                    write(half & 7, &mut written);
                }
            } else if (half & 0xfc00) == 0x4400 {
                if ((half >> 8) & 3) != 1 {
                    write((half & 7) | ((half >> 4) & 8), &mut written);
                }
            } else if (half & 0xf000) == 0x5000 {
                if ((half >> 9) & 7) >= 3 {
                    write(half & 7, &mut written);
                }
            } else if ((half & 0xe000) == 0x6000 || (half & 0xf000) == 0x8000)
                && (half & 0x0800) != 0
            {
                // PORT NOTE: the source spells the word-load and halfword-load
                // cases as two arms with identical bodies; merged here because
                // the conditions are disjoint, so the observable effect is the
                // same and clippy rejects the duplicate.
                write(half & 7, &mut written);
            } else if (half & 0xf000) == 0x9000 && (half & 0x0800) != 0 {
                write((half >> 8) & 7, &mut written);
            } else if (half & 0xff00) == 0xbc00 || (half & 0xf800) == 0xc800 {
                // PORT NOTE: `pop {…}` and `ldmia` are likewise two arms with
                // identical bodies in the source, merged for the same reason.
                for r in 0..8 {
                    if (half & (1 << r)) != 0 {
                        write(r, &mut written);
                    }
                }
            }
            for register in written {
                constants[register] = None;
            }

            if (half & 0xf800) == 0x4800 {
                let register = ((half >> 8) & 7) as usize;
                let literal = and32(pc + 4, !3) + ((half & 0xff) << 2);
                if self.inside(literal, 4) {
                    self.literal_slots.insert(literal);
                    let value = self.u32(literal);
                    constants[register] = Some(value);
                    if self.inside(and32(value, !1), 2) {
                        self.data_refs.insert(and32(value, !1));
                    }
                }
            } else if (half & 0xf800) == 0xa000 {
                constants[((half >> 8) & 7) as usize] =
                    Some(and32(pc + 4, !3) + ((half & 0xff) << 2));
            } else if (half & 0xf800) == 0x6800 {
                let destination = (half & 7) as usize;
                let base_register = ((half >> 3) & 7) as usize;
                if let Some(mut address) = constants[base_register] {
                    address += ((half >> 6) & 0x1f) << 2;
                    if self.inside(address, 4) {
                        let value = self.u32(address);
                        constants[destination] = Some(value);
                        if self.inside(and32(value, !1), 2) {
                            self.data_refs.insert(and32(value, !1));
                        }
                    }
                }
            } else if (half & 0xfe00) == 0x5800 {
                let destination = (half & 7) as usize;
                let base = constants[((half >> 3) & 7) as usize];
                let offset = constants[((half >> 6) & 7) as usize];
                if let (Some(base), Some(offset)) = (base, offset) {
                    let address = base + offset;
                    if self.inside(address, 4) {
                        let value = self.u32(address);
                        constants[destination] = Some(value);
                        if self.inside(and32(value, !1), 2) {
                            self.data_refs.insert(and32(value, !1));
                        }
                    }
                }
            }

            if (half & 0xf800) == 0xf000 && self.inside(pc, 4) {
                let low = self.u16(pc + 2);
                if (low & 0xf800) == 0xf800 {
                    let offset = sx(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
                    let target = pc + 4 + offset;
                    kind = "call";
                    size = 4;
                    self.call(delta, pc, target, Mode::Thumb);
                    for register in [0usize, 1, 2, 3, 12, 14] {
                        constants[register] = None;
                    }
                }
            } else if (half & 0xf800) == 0xe000 {
                kind = "branch";
                successors.push(pc + 4 + (sx(half & 0x7ff, 11) << 1));
                stop = true;
            } else if (half & 0xf000) == 0xd000 {
                let condition = (half >> 8) & 0xf;
                if condition < 0xe {
                    kind = "conditional";
                    successors.push(pc + 4 + (sx(half & 0xff, 8) << 1));
                    successors.push(pc + 2);
                    stop = true;
                } else if condition == 0xf {
                    kind = "software-interrupt";
                }
            } else if (half & 0xff87) == 0x4700 {
                let register = ((half >> 3) & 15) as usize;
                let previous = if self.inside(pc - 2, 2) {
                    self.u16(pc - 2)
                } else {
                    0
                };
                let popped = register < 8
                    && (previous & 0xff00) == 0xbc00
                    && (previous & (1 << register)) != 0;
                if register == 14 || popped || (register == 12 && self.u16(entry) == 0x46f4) {
                    kind = "return";
                } else {
                    kind = "indirect";
                    if let Some(value) = constants[register] {
                        self.call(
                            delta,
                            pc,
                            value,
                            if (value & 1) != 0 {
                                Mode::Thumb
                            } else {
                                Mode::Arm
                            },
                        );
                    } else {
                        delta.unresolved.insert(pc);
                        self.unresolved.insert(pc);
                    }
                }
                stop = true;
            } else if (half & 0xff00) == 0xbd00 {
                kind = "return";
                stop = true;
            } else if (half & 0xfc00) == 0x4400 && (half & 0x0087) == 0x0087 {
                let register = (half >> 3) & 15;
                let targets = self.thumb_jump_table(pc, register, &constants);
                if let Some(targets) = targets {
                    kind = "switch";
                    successors.extend(targets);
                } else if register == 12 && self.u16(entry) == 0x46f4 {
                    kind = "return";
                } else {
                    kind = "indirect";
                    delta.unresolved.insert(pc);
                    self.unresolved.insert(pc);
                }
                stop = true;
            }

            if (half & 0xfc00) == 0x4400 && ((half >> 8) & 3) == 2 {
                let destination = ((half & 7) | ((half >> 4) & 8)) as usize;
                let source = ((half >> 3) & 15) as usize;
                constants[destination] = None;
                if let Some(value) = constants[source] {
                    constants[destination] = Some(value);
                }
            }
            if !self.note_instruction(mode, delta, pc, size, kind) {
                return;
            }
            for target in successors {
                if self.inside(target, 2) {
                    queue.push((target, constants));
                }
            }
            if stop {
                return;
            }
            pc += size;
        }
    }

    fn note_jump_table(&mut self, pc: i64, table: i64, count: i64) -> Option<Vec<i64>> {
        if !(1..=256).contains(&count)
            || (table & 3) != 0
            || (table - pc).abs() > 0x10000
            || !self.inside(table, count * 4)
        {
            return None;
        }
        let targets: Vec<i64> = (0..count).map(|index| self.u32(table + index * 4)).collect();
        if targets
            .iter()
            .any(|&target| (target & 1) != 0 || !self.inside(target, 2) || (target - pc).abs() > 0x10000)
        {
            return None;
        }
        self.data_refs.insert(table);
        self.jump_tables.insert(table, targets.clone());
        self.jump_table_sites.insert(pc, table);
        Some(targets)
    }

    fn thumb_jump_table(
        &mut self,
        pc: i64,
        register: i64,
        constants: &Constants,
    ) -> Option<Vec<i64>> {
        if let Some(&table) = self.jump_table_sites.get(&pc) {
            return Some(self.jump_tables[&table].clone());
        }
        self.thumb_jump_table_compact(pc, register)
            .or_else(|| self.thumb_jump_table_extended(pc, register, constants))
    }

    fn thumb_jump_table_compact(&mut self, pc: i64, register: i64) -> Option<Vec<i64>> {
        if !self.inside(pc - 12, 14) {
            return None;
        }
        let setup = [(pc - 6, self.u16(pc - 6)), (pc - 4, self.u16(pc - 4))];
        let table_load = self.u16(pc - 2);
        let literals: Vec<(i64, i64)> = setup
            .iter()
            .copied()
            .filter(|item| (item.1 & 0xf800) == 0x4800)
            .collect();
        let shifts: Vec<(i64, i64)> = setup
            .iter()
            .copied()
            .filter(|item| {
                (item.1 & 0xf800) == 0 && (item.1 & 7) == register && ((item.1 >> 6) & 0x1f) == 2
            })
            .collect();
        if literals.len() != 1
            || shifts.len() != 1
            || (table_load & 0xfe00) != 0x5800
            || (table_load & 7) != register
        {
            return None;
        }
        let (literal_pc, literal_load) = literals[0];
        let shift = shifts[0].1;
        let index_register = (shift >> 3) & 7;
        let direct_compare = self.u16(pc - 10);
        let direct_branch = self.u16(pc - 8);
        let guarded_compare = self.u16(pc - 12);
        let guarded_branch = self.u16(pc - 10);
        let guarded_default = self.u16(pc - 8);
        let compare = if (direct_compare & 0xf800) == 0x2800
            && ((direct_compare >> 8) & 7) == index_register
            && (direct_branch & 0xff00) == 0xd800
        {
            direct_compare
        } else if (guarded_compare & 0xf800) == 0x2800
            && ((guarded_compare >> 8) & 7) == index_register
            && (guarded_branch & 0xff00) == 0xd900
            && (guarded_default & 0xf800) == 0xe000
            && pc - 10 + 4 + (sx(guarded_branch & 0xff, 8) << 1) == pc - 6
        {
            guarded_compare
        } else {
            return None;
        };
        let base_register = (literal_load >> 8) & 7;
        let mut load_registers: Vec<i64> = Vec::new();
        for candidate in [(table_load >> 3) & 7, (table_load >> 6) & 7] {
            if !load_registers.contains(&candidate) {
                load_registers.push(candidate);
            }
        }
        if load_registers.len() != 2
            || !load_registers.contains(&register)
            || !load_registers.contains(&base_register)
        {
            return None;
        }
        let literal = and32(literal_pc + 4, !3) + ((literal_load & 0xff) << 2);
        if !self.inside(literal, 4) {
            return None;
        }
        let table = self.u32(literal);
        self.note_jump_table(pc, table, (compare & 0xff) + 1)
    }

    fn thumb_jump_table_extended(
        &mut self,
        pc: i64,
        register: i64,
        constants: &Constants,
    ) -> Option<Vec<i64>> {
        // (shift site, index register, table address); insertion order decides
        // which candidate wins, so this stays a Vec.
        let mut dispatches: Vec<(i64, i64, i64)> = Vec::new();
        let mut load_pc = pc - 2;
        while load_pc > std::cmp::max(pc - 42, self.base) {
            let load = self.u16(load_pc);
            if (load & 0xfe00) != 0x5800 || (load & 7) != register {
                load_pc -= 2;
                continue;
            }
            let mut address_registers: Vec<i64> = Vec::new();
            for candidate in [(load >> 3) & 7, (load >> 6) & 7] {
                if !address_registers.contains(&candidate) {
                    address_registers.push(candidate);
                }
            }
            let mut shift_pc = load_pc - 2;
            while shift_pc > std::cmp::max(load_pc - 10, self.base) {
                let shift = self.u16(shift_pc);
                if (shift & 0xf800) != 0
                    || ((shift >> 6) & 0x1f) != 2
                    || !address_registers.contains(&(shift & 7))
                {
                    shift_pc -= 2;
                    continue;
                }
                let index_register = (shift >> 3) & 7;
                let shifted_register = shift & 7;
                for base_register in address_registers
                    .iter()
                    .copied()
                    .filter(|item| *item != shifted_register)
                {
                    let mut table = constants[base_register as usize];
                    let mut literal_pc = load_pc - 2;
                    while literal_pc > std::cmp::max(load_pc - 18, self.base) {
                        let literal_load = self.u16(literal_pc);
                        if (literal_load & 0xf800) == 0x4800
                            && ((literal_load >> 8) & 7) == base_register
                        {
                            let literal = and32(literal_pc + 4, !3) + ((literal_load & 0xff) << 2);
                            if self.inside(literal, 4) {
                                table = Some(self.u32(literal));
                            }
                            break;
                        }
                        literal_pc -= 2;
                    }
                    if let Some(table) = table {
                        dispatches.push((shift_pc, index_register, table));
                    }
                }
                shift_pc -= 2;
            }
            load_pc -= 2;
        }

        let load = self.u16(pc - 2);
        if (load & 0xf800) == 0x6800
            && (load & 7) == register
            && ((load >> 3) & 7) == register
            && ((load >> 6) & 0x1f) == 0
        {
            let add = self.u16(pc - 4);
            let shift = self.u16(pc - 8);
            let literal_load = self.u16(pc - 6);
            if (add & 0xfe00) == 0x1800
                && (add & 7) == register
                && (shift & 0xf800) == 0
                && ((shift >> 6) & 0x1f) == 2
                && (shift & 7) == register
                && (literal_load & 0xf800) == 0x4800
            {
                let mut add_registers: Vec<i64> = Vec::new();
                for candidate in [(add >> 3) & 7, (add >> 6) & 7] {
                    if !add_registers.contains(&candidate) {
                        add_registers.push(candidate);
                    }
                }
                let base_register = (literal_load >> 8) & 7;
                if add_registers.len() == 2
                    && add_registers.contains(&register)
                    && add_registers.contains(&base_register)
                {
                    let literal = and32(pc - 6 + 4, !3) + ((literal_load & 0xff) << 2);
                    if self.inside(literal, 4) {
                        let table = self.u32(literal);
                        dispatches.push((pc - 8, (shift >> 3) & 7, table));
                    }
                }
            }
        }

        for (shift_pc, _, table) in dispatches {
            let mut compare_pc = shift_pc - 2;
            while compare_pc > std::cmp::max(shift_pc - 50, self.base) {
                let compare = self.u16(compare_pc);
                if (compare & 0xf800) != 0x2800 {
                    compare_pc -= 2;
                    continue;
                }
                let branch = self.u16(compare_pc + 2);
                if !matches!(branch & 0xff00, 0xd800 | 0xd900) {
                    compare_pc -= 2;
                    continue;
                }
                let branch_target = compare_pc + 6 + (sx(branch & 0xff, 8) << 1);
                if (branch & 0xff00) == 0xd900
                    && !(compare_pc + 4 <= branch_target && branch_target <= shift_pc)
                {
                    compare_pc -= 2;
                    continue;
                }
                if let Some(targets) = self.note_jump_table(pc, table, (compare & 0xff) + 1) {
                    return Some(targets);
                }
                compare_pc -= 2;
            }
        }
        None
    }

    pub fn walk_function(&mut self, address: i64) {
        let (mode, entry) = {
            let info = self
                .functions
                .get(address)
                .unwrap_or_else(|| panic!("unknown function: {}", hex(address)));
            (info.mode, info.entry)
        };
        let mut queue: Vec<(i64, Constants)> = vec![(address, NO_CONSTANTS)];
        let mut seen: HashSet<i64> = HashSet::new();
        let mut delta = FnDelta::default();
        let mut blocks: BTreeSet<i64> = BTreeSet::new();
        let mut head = 0usize;
        while head < queue.len() {
            let (block, constants) = queue[head];
            head += 1;
            if seen.contains(&block) {
                continue;
            }
            blocks.insert(block);
            if mode == Mode::Arm {
                self.walk_arm(block, &mut delta, &mut queue, &mut seen, &constants);
            } else {
                self.walk_thumb(block, entry, &mut delta, &mut queue, &mut seen, &constants);
            }
        }
        let info = self.functions.get_mut(address).expect("checked above");
        info.blocks.extend(blocks);
        info.instructions.extend(delta.instructions);
        info.callees.extend(delta.callees);
        info.external_callees.extend(delta.external_callees);
        info.unresolved.extend(delta.unresolved);
    }

    /// Banks in first-seen order, matching the JavaScript `Map`/`Set` pair.
    fn code_banks(&self) -> (Vec<i64>, HashSet<i64>) {
        let mut order: Vec<i64> = Vec::new();
        let mut counts: HashMap<i64, usize> = HashMap::new();
        for entry in self.functions.keys() {
            let bank = (entry - self.base).div_euclid(0x10000);
            let next = counts.get(&bank).copied().unwrap_or(0) + 1;
            if counts.insert(bank, next).is_none() {
                order.push(bank);
            }
        }
        let banks: Vec<i64> = order
            .into_iter()
            .filter(|bank| counts[bank] >= 5)
            .collect();
        let membership: HashSet<i64> = banks.iter().copied().collect();
        (banks, membership)
    }

    pub fn discover_global_pointer_tables(&mut self) -> bool {
        let (_, code_banks) = self.code_banks();
        let mut added = false;
        let mut offset = 0i64;
        let length = self.data.len() as i64;
        while offset + 8 <= length {
            if !code_banks.contains(&offset.div_euclid(0x10000)) {
                offset += 4;
                continue;
            }
            let mut targets: Vec<i64> = Vec::new();
            let mut cursor = offset;
            while cursor + 4 <= length && targets.len() < 1024 {
                let value = self.read_u32_at_offset(cursor as usize);
                let target = and32(value, !1);
                let target_bank = (target - self.base).div_euclid(0x10000);
                if (value & 1) == 0
                    || !self.inside(target, 2)
                    || !code_banks.contains(&target_bank)
                    || !self.plausible(target)
                {
                    break;
                }
                targets.push(target);
                cursor += 4;
            }
            if targets.len() < 2 {
                offset += 4;
                continue;
            }
            let table = self.base + offset;
            self.pointer_tables.insert(table, targets.clone());
            let label = format!("global-pointer-table:{}", hex(table));
            for target in targets {
                if !self.instructions.contains_key(&target) {
                    added = self.add_seed(target, Mode::Thumb, &label) || added;
                }
            }
            offset = cursor;
        }
        added
    }

    fn plausible(&self, address: i64) -> bool {
        let half = self.u16(address);
        (half & 0xff00) == 0xb500 || half == 0x4770 || (half & 0xf800) == 0x2000
    }

    pub fn discover_individual_function_pointers(&mut self) -> bool {
        let (_, code_banks) = self.code_banks();
        let mut added = false;
        let length = self.data.len() as i64;
        let mut offset = 0i64;
        while offset < length - 3 {
            if !code_banks.contains(&offset.div_euclid(0x10000)) {
                offset += 4;
                continue;
            }
            let value = self.read_u32_at_offset(offset as usize);
            let target = and32(value, !1);
            if (value & 1) == 0
                || !self.inside(target, 2)
                || !code_banks.contains(&(target - self.base).div_euclid(0x10000))
                || (self.u16(target) & 0xff00) != 0xb500
                || self.instructions.contains_key(&target)
            {
                offset += 4;
                continue;
            }
            let label = format!("function-pointer:{}", hex(self.base + offset));
            added = self.add_seed(target, Mode::Thumb, &label) || added;
            offset += 4;
        }
        added
    }

    pub fn discover_prologue_boundaries(&mut self) -> bool {
        let (banks, _) = self.code_banks();
        let mut added = false;
        for bank in banks {
            let start = self.base + bank * 0x10000;
            let end = std::cmp::min(start + 0x10000, self.limit);
            let mut address = start + 2;
            while address < end {
                if self.instructions.contains_key(&address)
                    || self.literal_slots.contains(&address)
                    || (self.u16(address) & 0xff00) != 0xb500
                {
                    address += 2;
                    continue;
                }
                if !self.inside(address + 2, 2) || matches!(self.u16(address + 2), 0 | 0xffff) {
                    address += 2;
                    continue;
                }
                let previous = self.u16(address - 2);
                if matches!(previous, 0 | 0xffff)
                    || previous == 0x4770
                    || (previous & 0xff00) == 0xbd00
                {
                    let label = format!("prologue-boundary:{}", hex(address));
                    added = self.add_seed(address, Mode::Thumb, &label) || added;
                }
                address += 2;
            }
        }
        added
    }

    /// Everything EXCEPT `blocked_data`, which is the point: the re-run must
    /// remember which spans were proved to be data.
    pub fn reset_flow(&mut self) {
        self.functions.clear();
        self.instructions.clear();
        self.calls = CallSet::default();
        self.external_calls = CallSet::default();
        self.unresolved.clear();
        self.conflicts.clear();
        self.data_refs.clear();
        self.literal_slots.clear();
        self.pointer_tables.clear();
        self.jump_tables.clear();
        self.jump_table_sites.clear();
    }

    pub fn block_structural_overlaps(&mut self) -> bool {
        let mut blocked: BTreeSet<i64> = BTreeSet::new();
        for &address in &self.literal_slots {
            if self.instructions.contains_key(&address)
                || self.instructions.contains_key(&(address + 2))
            {
                blocked.insert(address);
                blocked.insert(address + 2);
            }
        }
        for (&address, targets) in &self.jump_tables {
            let end = address + 4 * targets.len() as i64;
            let mut item = address;
            while item < end {
                if self.instructions.contains_key(&item) {
                    let mut value = address;
                    while value < end {
                        blocked.insert(value);
                        value += 2;
                    }
                    break;
                }
                item += 2;
            }
        }
        let mut added = false;
        for address in blocked {
            if self.blocked_data.insert(address) {
                added = true;
            }
        }
        added
    }

    pub fn discover_once(&mut self) -> i64 {
        let entry = self.initial_seeds();
        let mut walked: HashSet<i64> = HashSet::new();
        loop {
            let mut pending: Vec<i64> = {
                let mut items: Vec<i64> = self
                    .functions
                    .keys()
                    .filter(|address| !walked.contains(address))
                    .collect();
                items.sort_unstable();
                items
            };
            let mut head = 0usize;
            while head < pending.len() {
                let address = pending[head];
                if walked.contains(&address) {
                    head += 1;
                    continue;
                }
                self.walk_function(address);
                walked.insert(address);
                let mut discovered: Vec<i64> = self
                    .functions
                    .keys()
                    .filter(|item| !walked.contains(item))
                    .collect();
                discovered.sort_unstable();
                for item in discovered {
                    // `pending.includes(item, head + 1)` — the search starts
                    // past the cursor, so an address already consumed can be
                    // appended again and is skipped by the `walked` guard.
                    if !pending[head + 1..].contains(&item) {
                        pending.push(item);
                    }
                }
                head += 1;
            }
            let mut added = false;
            let refs: Vec<i64> = self.data_refs.iter().copied().collect();
            for table in refs {
                if (table & 3) != 0 || !self.inside(table, 8) {
                    continue;
                }
                let mut targets: Vec<i64> = Vec::new();
                let mut cursor = table;
                while self.inside(cursor, 4) && targets.len() < 1024 {
                    let value = self.u32(cursor);
                    if (value & 1) == 0 || !self.inside(and32(value, !1), 2) {
                        break;
                    }
                    targets.push(and32(value, !1));
                    cursor += 4;
                }
                if targets.len() < 2 {
                    continue;
                }
                self.pointer_tables.insert(table, targets.clone());
                let label = format!("pointer-table:{}", hex(table));
                for target in targets {
                    added = self.add_seed(target, Mode::Thumb, &label) || added;
                }
            }
            added = self.discover_global_pointer_tables() || added;
            added = self.discover_individual_function_pointers() || added;
            added = self.discover_prologue_boundaries() || added;
            if !added {
                break;
            }
        }
        entry
    }

    pub fn run(&mut self) -> i64 {
        let entry;
        loop {
            let found = self.discover_once();
            if !self.block_structural_overlaps() {
                entry = found;
                break;
            }
            self.reset_flow();
        }
        let sites: Vec<i64> = self.jump_table_sites.keys().copied().collect();
        for site in &sites {
            self.unresolved.remove(site);
        }
        for info in self.functions.values_mut() {
            for site in &sites {
                info.unresolved.remove(site);
            }
        }
        entry
    }

    pub fn report(&self, entry: i64, details: bool) -> Json {
        let numbers = |values: &BTreeSet<i64>| -> Json {
            Json::Arr(values.iter().map(|&value| Json::Int(value)).collect())
        };
        let mut functions: Vec<i64> = self.functions.keys().collect();
        functions.sort_unstable();
        let function_rows: Vec<Json> = functions
            .iter()
            .map(|&address| {
                let info = &self.functions.entries[&address];
                let insns: Vec<i64> = info.instructions.iter().copied().collect();
                let last = insns.last().copied();
                Json::Obj(vec![
                    ("entry".into(), Json::Int(address)),
                    ("mode".into(), Json::Str(info.mode.as_str().into())),
                    (
                        "sources".into(),
                        Json::Arr(
                            info.sources
                                .iter()
                                .map(|source| Json::Str(source.clone()))
                                .collect(),
                        ),
                    ),
                    ("blocks".into(), Json::Int(info.blocks.len() as i64)),
                    ("instruction_count".into(), Json::Int(insns.len() as i64)),
                    (
                        "min_address".into(),
                        insns.first().map_or(Json::Null, |&v| Json::Int(v)),
                    ),
                    (
                        "max_address".into(),
                        last.map_or(Json::Null, |v| Json::Int(v + self.instructions[&v].size)),
                    ),
                    ("callees".into(), numbers(&info.callees)),
                    ("external_callees".into(), numbers(&info.external_callees)),
                    ("unresolved".into(), numbers(&info.unresolved)),
                ])
            })
            .collect();

        let mut report: Vec<(String, Json)> = vec![
            ("rom_base".into(), Json::Int(self.base)),
            ("rom_size".into(), Json::Int(self.data.len() as i64)),
            ("reset_entry".into(), Json::Int(entry)),
            ("function_count".into(), Json::Int(function_rows.len() as i64)),
            (
                "instruction_count".into(),
                Json::Int(self.instructions.len() as i64),
            ),
            ("call_count".into(), Json::Int(self.calls.len() as i64)),
            (
                "external_call_count".into(),
                Json::Int(self.external_calls.len() as i64),
            ),
            (
                "unresolved_count".into(),
                Json::Int(self.unresolved.len() as i64),
            ),
            (
                "pointer_table_count".into(),
                Json::Int(self.pointer_tables.len() as i64),
            ),
            (
                "jump_table_count".into(),
                Json::Int(self.jump_tables.len() as i64),
            ),
            (
                "conflicts".into(),
                Json::Arr(
                    self.conflicts
                        .iter()
                        .map(|(address, old, new, source)| {
                            Json::Obj(vec![
                                ("address".into(), Json::Int(*address)),
                                ("old".into(), Json::Str(old.as_str().into())),
                                ("new".into(), Json::Str(new.as_str().into())),
                                ("source".into(), Json::Str(source.clone())),
                            ])
                        })
                        .collect(),
                ),
            ),
            ("functions".into(), Json::Arr(function_rows)),
        ];

        if details {
            report.push((
                "instructions".into(),
                Json::Arr(
                    self.instructions
                        .iter()
                        .map(|(address, instruction)| {
                            Json::Obj(vec![
                                ("address".into(), Json::Int(*address)),
                                ("size".into(), Json::Int(instruction.size)),
                                ("mode".into(), Json::Str(instruction.mode.as_str().into())),
                                ("kind".into(), Json::Str(instruction.kind.into())),
                            ])
                        })
                        .collect(),
                ),
            ));
            // PORT NOTE: the TypeScript breaks ties with
            // `a[2].localeCompare(b[2])`, ICU collation of "arm" vs "thumb".
            // Both are lowercase ASCII, where ICU, UTF-16 and byte order all
            // agree, so the derived `Ord` on `Mode` (Arm < Thumb) matches.
            report.push(("calls".into(), sorted_calls(&self.calls)));
            report.push(("external_calls".into(), sorted_calls(&self.external_calls)));
            report.push(("unresolved".into(), numbers(&self.unresolved)));
            report.push(("data_refs".into(), numbers(&self.data_refs)));
            report.push(("literal_slots".into(), numbers(&self.literal_slots)));
            report.push((
                "pointer_tables".into(),
                Json::Arr(
                    self.pointer_tables
                        .iter()
                        .map(|(address, targets)| {
                            Json::Obj(vec![
                                ("address".into(), Json::Int(*address)),
                                (
                                    "targets".into(),
                                    Json::Arr(
                                        targets.iter().map(|&value| Json::Int(value)).collect(),
                                    ),
                                ),
                            ])
                        })
                        .collect(),
                ),
            ));
            report.push((
                "jump_tables".into(),
                Json::Arr(
                    self.jump_tables
                        .iter()
                        .map(|(address, targets)| {
                            let mut sites: Vec<i64> = self
                                .jump_table_sites
                                .iter()
                                .filter(|(_, table)| *table == address)
                                .map(|(site, _)| *site)
                                .collect();
                            sites.sort_unstable();
                            Json::Obj(vec![
                                ("address".into(), Json::Int(*address)),
                                (
                                    "targets".into(),
                                    Json::Arr(
                                        targets.iter().map(|&value| Json::Int(value)).collect(),
                                    ),
                                ),
                                (
                                    "sites".into(),
                                    Json::Arr(sites.into_iter().map(Json::Int).collect()),
                                ),
                            ])
                        })
                        .collect(),
                ),
            ));
        }
        Json::Obj(report)
    }
}

fn sorted_calls(calls: &CallSet) -> Json {
    let mut rows = calls.order.clone();
    rows.sort_by(|a, b| a.0.cmp(&b.0).then(a.1.cmp(&b.1)).then(a.2.cmp(&b.2)));
    Json::Arr(
        rows.into_iter()
            .map(|(source, target, mode)| {
                Json::Obj(vec![
                    ("source".into(), Json::Int(source)),
                    ("target".into(), Json::Int(target)),
                    ("mode".into(), Json::Str(mode.as_str().into())),
                ])
            })
            .collect(),
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Assemble a synthetic image rather than assert on a constant: an
    /// 0x08000000-based ROM whose reset vector branches to an ARM entry that
    /// loads a Thumb literal and calls it.
    struct Image {
        bytes: Vec<u8>,
    }

    impl Image {
        fn new(size: usize) -> Self {
            Image {
                bytes: vec![0; size],
            }
        }
        fn u16(&mut self, offset: usize, value: u16) -> &mut Self {
            self.bytes[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
            self
        }
        fn u32(&mut self, offset: usize, value: u32) -> &mut Self {
            self.bytes[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
            self
        }
        /// `b <offset>` as the ARM reset vector at 0.
        fn reset_to(&mut self, offset: usize) -> &mut Self {
            let imm = ((offset as i64 - 8) >> 2) as u32 & 0x00ff_ffff;
            self.u32(0, 0xea00_0000 | imm)
        }
    }

    #[test]
    fn sign_extension_matches_the_javascript_coercion() {
        assert_eq!(sx(0x7ff, 11), -1);
        assert_eq!(sx(0x3ff, 11), 1023);
        assert_eq!(sx(0xffffff, 24), -1);
        assert_eq!(sx(0x800000, 24), -8388608);
        assert_eq!(sx(0, 8), 0);
    }

    #[test]
    fn masking_a_pointer_goes_through_signed_32_bit_space() {
        // 0xffff_fffe as an i32 is -2, which is what `value & ~1` yields in
        // JavaScript and what `inside` must then reject.
        assert_eq!(and32(0xffff_ffff, !1), -2);
        assert_eq!(and32(0x0800_0003, !1), 0x0800_0002);
    }

    #[test]
    fn hex_pads_to_eight_and_reproduces_the_negative_quirk() {
        assert_eq!(hex(0x0800_0000), "08000000");
        assert_eq!(hex(0x1_0000_0000), "100000000");
        assert_eq!(hex(-2), "000000-2");
    }

    #[test]
    fn reset_vector_seeds_the_arm_entry_and_startup_literals() {
        let mut image = Image::new(0x400);
        image.reset_to(0x200);
        // entry: bx lr (mov pc, lr encoding used by the walker's return test)
        image.u32(0x200, 0xe1a0_f00e);
        // a startup literal one word past the entry, tagged Thumb.
        image.u32(0x204, 0x0800_0301);
        image.u16(0x300, 0x4770); // bx lr
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        let entry = discovery.initial_seeds();
        assert_eq!(entry, ROM_BASE + 0x200);
        assert_eq!(
            discovery.function(ROM_BASE + 0x200).map(|f| f.mode),
            Some(Mode::Arm)
        );
        let thumb = discovery
            .function(ROM_BASE + 0x300)
            .expect("startup literal seeds a Thumb function");
        assert_eq!(thumb.mode, Mode::Thumb);
        assert!(thumb.sources.contains("startup-literal"));
    }

    #[test]
    fn header_veneers_seed_their_targets() {
        let mut image = Image::new(0x400);
        image.reset_to(0x200);
        image.u32(0x200, 0xe1a0_f00e);
        image.u32(0xc0, 0x4720_4c00);
        image.u32(0xc4, 0x0800_0341);
        image.u16(0x340, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.initial_seeds();
        assert!(discovery
            .function(ROM_BASE + 0xc0)
            .expect("veneer")
            .sources
            .contains("header-veneer"));
        assert!(discovery
            .function(ROM_BASE + 0x340)
            .expect("veneer target")
            .sources
            .contains("header-veneer-target"));
    }

    #[test]
    fn a_thumb_call_records_the_callee_and_seeds_it() {
        let mut image = Image::new(0x400);
        image.reset_to(0x200);
        image.u32(0x200, 0xe1a0_f00e);
        // Thumb function at 0x300: push {lr}; bl 0x320; pop {pc}
        image.u16(0x300, 0xb500);
        let offset: i64 = 0x320 - (0x302 + 4);
        let high = 0xf000 | (((offset >> 12) & 0x7ff) as u16);
        let low = 0xf800 | (((offset >> 1) & 0x7ff) as u16);
        image.u16(0x302, high);
        image.u16(0x304, low);
        image.u16(0x306, 0xbd00);
        image.u16(0x320, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "test");
        discovery.walk_function(ROM_BASE + 0x300);
        let info = discovery.function(ROM_BASE + 0x300).expect("seeded");
        assert_eq!(
            info.instructions.iter().copied().collect::<Vec<_>>(),
            vec![ROM_BASE + 0x300, ROM_BASE + 0x302, ROM_BASE + 0x306]
        );
        assert!(info.callees.contains(&(ROM_BASE + 0x320)));
        assert_eq!(discovery.instructions[&(ROM_BASE + 0x302)].size, 4);
        assert_eq!(discovery.instructions[&(ROM_BASE + 0x306)].kind, "return");
        assert!(discovery.function(ROM_BASE + 0x320).is_some());
    }

    #[test]
    fn an_indirect_branch_with_no_known_constant_is_unresolved() {
        let mut image = Image::new(0x400);
        image.u16(0x300, 0x4710); // bx r2
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "test");
        discovery.walk_function(ROM_BASE + 0x300);
        assert!(discovery.unresolved.contains(&(ROM_BASE + 0x300)));
        assert_eq!(discovery.instructions[&(ROM_BASE + 0x300)].kind, "indirect");
    }

    #[test]
    fn a_literal_pool_load_resolves_an_indirect_call() {
        let mut image = Image::new(0x400);
        // 0x300: ldr r2, [pc, #0]  -> literal at 0x304
        image.u16(0x300, 0x4a00);
        image.u16(0x302, 0x46c0); // nop, keeps the pool word aligned at 0x304
        image.u32(0x304, 0x0800_0381); // Thumb pointer to 0x380
        image.u16(0x308, 0x4710); // bx r2
        image.u16(0x380, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "test");
        discovery.walk_function(ROM_BASE + 0x300);
        let info = discovery.function(ROM_BASE + 0x300).expect("seeded");
        assert!(info.callees.contains(&(ROM_BASE + 0x380)));
        assert!(discovery.literal_slots.contains(&(ROM_BASE + 0x304)));
        assert!(discovery.data_refs.contains(&(ROM_BASE + 0x380)));
        assert!(discovery.unresolved.is_empty());
    }

    #[test]
    fn a_call_outside_the_image_is_external() {
        let mut image = Image::new(0x400);
        image.u16(0x300, 0x4a00);
        image.u16(0x302, 0x46c0);
        image.u32(0x304, 0x0300_0001); // IWRAM, outside a ROM-based image
        image.u16(0x308, 0x4710);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "test");
        discovery.walk_function(ROM_BASE + 0x300);
        let info = discovery.function(ROM_BASE + 0x300).expect("seeded");
        assert!(info.external_callees.contains(&0x0300_0000));
        assert_eq!(discovery.external_calls.len(), 1);
        assert_eq!(discovery.calls.len(), 0);
    }

    #[test]
    fn a_mode_conflict_is_recorded_and_the_seed_refused() {
        let mut image = Image::new(0x400);
        image.u16(0x300, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        assert!(discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "first"));
        assert!(!discovery.add_seed(ROM_BASE + 0x300, Mode::Arm, "second"));
        assert_eq!(discovery.conflicts.len(), 1);
        assert_eq!(discovery.conflicts[0].1, Mode::Thumb);
        assert_eq!(discovery.conflicts[0].2, Mode::Arm);
        // A repeat of the SAME mode is not a conflict; it only records a source.
        assert!(!discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "third"));
        assert_eq!(discovery.conflicts.len(), 1);
        let info = discovery.function(ROM_BASE + 0x300).expect("seeded");
        assert_eq!(
            info.sources.iter().cloned().collect::<Vec<_>>(),
            vec!["first".to_string(), "third".to_string()]
        );
    }

    #[test]
    fn seeding_a_blocked_address_is_refused() {
        let image = Image::new(0x400);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.blocked_data.insert(ROM_BASE + 0x300);
        assert!(!discovery.add_seed(ROM_BASE + 0x300, Mode::Thumb, "blocked"));
        assert_eq!(discovery.function_count(), 0);
    }

    #[test]
    fn a_thumb_pointer_table_seeds_every_entry() {
        let mut image = Image::new(0x800);
        image.reset_to(0x200);
        image.u32(0x200, 0xe1a0_f00e);
        // Seven Thumb prologues so the bank counts as code, then a table
        // pointing at the two that were never seeded directly.
        for slot in 0..7 {
            image.u16(0x400 + slot * 4, 0xb500);
            image.u16(0x402 + slot * 4, 0xbd00);
        }
        image.u32(0x600, 0x0800_0415);
        image.u32(0x604, 0x0800_0419);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        for slot in 0..5 {
            discovery.add_seed(ROM_BASE + 0x400 + slot * 4, Mode::Thumb, "test");
        }
        assert!(
            discovery.discover_global_pointer_tables(),
            "the table's two unseen targets are new functions"
        );
        assert_eq!(
            discovery.pointer_tables.get(&(ROM_BASE + 0x600)),
            Some(&vec![ROM_BASE + 0x414, ROM_BASE + 0x418])
        );
        assert!(discovery
            .function(ROM_BASE + 0x414)
            .expect("seeded from the table")
            .sources
            .iter()
            .any(|source| source.starts_with("global-pointer-table:")));
    }

    #[test]
    fn a_bank_with_fewer_than_five_functions_is_not_code() {
        let mut image = Image::new(0x800);
        for slot in 0..4 {
            image.u16(0x400 + slot * 4, 0xb500);
        }
        image.u32(0x600, 0x0800_0401);
        image.u32(0x604, 0x0800_0405);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        for slot in 0..4 {
            discovery.add_seed(ROM_BASE + 0x400 + slot * 4, Mode::Thumb, "test");
        }
        assert!(!discovery.discover_global_pointer_tables());
        assert!(discovery.pointer_tables.is_empty());
    }

    #[test]
    fn a_prologue_after_a_return_is_a_function_boundary() {
        let mut image = Image::new(0x800);
        for slot in 0..5 {
            image.u16(0x400 + slot * 4, 0xb500);
            image.u16(0x402 + slot * 4, 0xbd00);
        }
        // 0x420: bx lr, then a fresh prologue with a non-empty successor.
        image.u16(0x420, 0x4770);
        image.u16(0x422, 0xb500);
        image.u16(0x424, 0x2000);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        for slot in 0..5 {
            discovery.add_seed(ROM_BASE + 0x400 + slot * 4, Mode::Thumb, "test");
        }
        assert!(discovery.discover_prologue_boundaries());
        assert!(discovery
            .function(ROM_BASE + 0x422)
            .expect("boundary")
            .sources
            .iter()
            .any(|source| source.starts_with("prologue-boundary:")));
    }

    #[test]
    fn a_compact_switch_dispatch_resolves_its_jump_table() {
        // cmp r0, #1 / bhi .default / lsl r1, r0, #2 / ldr r1, [r2, r1] /
        // add pc, r1, with the table pointer in a literal pool.
        let mut image = Image::new(0x800);
        // Register assignment copied from a real dispatch in the base image
        // (site 0x0800aac4): cmp r3 / bhi / ldr r2,[pc] / lsl r3,r3,#2 /
        // ldr r3,[r3,r2] / add pc,r3.
        let site = 0x310usize;
        image.u16(site - 10, 0x2b01); // cmp r3, #1
        image.u16(site - 8, 0xd806); // bhi 0x318
        image.u16(site - 6, 0x4a02); // ldr r2, [pc, #8] -> 0x314
        image.u16(site - 4, 0x009b); // lsl r3, r3, #2
        image.u16(site - 2, 0x589b); // ldr r3, [r3, r2]
        image.u16(site, 0x469f); // add pc, r3
        image.u32(0x314, 0x0800_0400); // table address
        image.u16(0x318, 0x4770);
        image.u32(0x400, 0x0800_0500);
        image.u32(0x404, 0x0800_0520);
        image.u16(0x500, 0x4770);
        image.u16(0x520, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        let table = discovery.thumb_jump_table_compact(ROM_BASE + site as i64, 3);
        assert_eq!(
            table,
            Some(vec![ROM_BASE + 0x500, ROM_BASE + 0x520]),
            "compact dispatch must resolve both arms"
        );
        assert_eq!(
            discovery.jump_table_sites.get(&(ROM_BASE + site as i64)),
            Some(&(ROM_BASE + 0x400))
        );
    }

    #[test]
    fn a_jump_table_with_a_thumb_tagged_target_is_rejected() {
        let mut image = Image::new(0x800);
        image.u32(0x400, 0x0800_0501); // odd: not a jump-table target
        image.u32(0x404, 0x0800_0520);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        assert_eq!(
            discovery.note_jump_table(ROM_BASE + 0x310, ROM_BASE + 0x400, 2),
            None
        );
        assert!(discovery.jump_tables.is_empty());
    }

    #[test]
    fn note_jump_table_rejects_an_unaligned_or_distant_table() {
        let image = Image::new(0x800);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        assert_eq!(
            discovery.note_jump_table(ROM_BASE + 0x310, ROM_BASE + 0x402, 2),
            None,
            "an unaligned table is refused"
        );
        assert_eq!(
            discovery.note_jump_table(ROM_BASE + 0x310, ROM_BASE + 0x400, 0),
            None,
            "a zero-length table is refused"
        );
    }

    #[test]
    fn structural_overlap_blocking_is_idempotent() {
        let mut image = Image::new(0x400);
        image.u16(0x300, 0x4770);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.literal_slots.insert(ROM_BASE + 0x300);
        discovery.instructions.insert(
            ROM_BASE + 0x300,
            Instruction {
                size: 2,
                mode: Mode::Thumb,
                kind: "return",
            },
        );
        assert!(discovery.block_structural_overlaps());
        assert!(discovery.blocked_data.contains(&(ROM_BASE + 0x300)));
        assert!(discovery.blocked_data.contains(&(ROM_BASE + 0x302)));
        assert!(
            !discovery.block_structural_overlaps(),
            "a second pass adds nothing and must report no progress"
        );
    }

    #[test]
    fn reset_flow_keeps_blocked_data() {
        let image = Image::new(0x400);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        discovery.blocked_data.insert(ROM_BASE + 0x300);
        discovery.add_seed(ROM_BASE + 0x200, Mode::Thumb, "test");
        discovery.reset_flow();
        assert_eq!(discovery.function_count(), 0);
        assert!(discovery.blocked_data.contains(&(ROM_BASE + 0x300)));
    }

    #[test]
    fn the_report_carries_the_counts_and_the_sorted_rows() {
        let mut image = Image::new(0x400);
        image.reset_to(0x200);
        image.u32(0x200, 0xe1a0_f00e);
        let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
        let entry = discovery.run();
        let text = json::canonical_json(&discovery.report(entry, true));
        assert!(text.starts_with("{\n  \"rom_base\": 134217728,\n  \"rom_size\": 1024,"));
        assert!(text.contains("\"reset_entry\": 134218240"));
        assert!(text.contains("\"conflicts\": []"));
        assert!(text.contains("\"calls\": []"));
        assert!(text.contains("\"mode\": \"arm\""));
    }

    #[test]
    fn a_full_run_converges_and_is_deterministic() {
        let mut image = Image::new(0x2000);
        image.reset_to(0x200);
        image.u32(0x200, 0xe1a0_f00e);
        for slot in 0..6 {
            image.u16(0x400 + slot * 8, 0xb500);
            image.u16(0x402 + slot * 8, 0x2000);
            image.u16(0x404 + slot * 8, 0xbd00);
        }
        // Header veneers so the bank clears the five-function threshold and
        // the prologue-boundary pass runs at all.
        for slot in 0..5 {
            image.u32(0xc0 + slot * 8, 0x4720_4c00);
            image.u32(0xc4 + slot * 8, 0x0800_0401 + slot as u32 * 8);
        }
        let first = {
            let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
            let entry = discovery.run();
            json::canonical_json(&discovery.report(entry, true))
        };
        let second = {
            let mut discovery = Discovery::new(&image.bytes, ROM_BASE);
            let entry = discovery.run();
            json::canonical_json(&discovery.report(entry, true))
        };
        assert_eq!(first, second);
        assert!(first.contains("prologue-boundary"));
    }
}
