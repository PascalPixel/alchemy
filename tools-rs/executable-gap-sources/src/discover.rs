// Recursive-descent code discovery over a raw GBA image: seed on the reset
// vector and the header veneers, walk ARM and Thumb flow with a small
// constant-tracker so literal pools and jump tables resolve, then re-seed from
// pointer tables and prologue shapes until nothing new appears.
//
// WHY this file exists here: `thumbSource` seeds a `Discovery` per Thumb
// section and reads back three fields -- `instructions`, `literal_slots` and
// `jump_tables` -- to decide which halfwords are instructions, which words are
// literal pool entries, and where the local labels go. Without the walker the
// gap sources cannot be regenerated at all.
//
// Ported from tools/lib/discover.ts by way of the already-validated
// `tools-rs/discover` port; copied rather than shared by path so this crate
// builds standalone. Only the subset `thumbSource` can reach is kept:
// `new`/`inside`/`u16`/`u32`/`add_seed`/`note_instruction`/`call`/`walk_arm`/
// `walk_thumb`/the jump-table recognisers/`walk_function`. The re-seeding
// passes (`initial_seeds`, `discover_*`, `run`, `report`) are unreachable from
// this tool -- it seeds explicitly from the plan's symbols -- and are omitted
// rather than carried as dead code.

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
    /// First-insertion order of `instructions`.
    ///
    /// PORT NOTE: the TypeScript field is a `Map`, and `thumbSource` iterates a
    /// filtered copy of it while DELETING from that copy -- so whether a branch
    /// target has already been deleted when its branch is examined depends on
    /// walk order, not on address order. A `BTreeMap` alone would silently
    /// substitute ascending-address order and change which targets become local
    /// labels. The map stays a `BTreeMap` for cheap range lookup; this vector
    /// carries the order the JavaScript `Map` would have iterated in.
    pub instruction_order: Vec<i64>,
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
            instruction_order: Vec::new(),
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
        if self
            .instructions
            .insert(address, Instruction { size, mode, kind })
            .is_none()
        {
            self.instruction_order.push(address);
        }
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
}
