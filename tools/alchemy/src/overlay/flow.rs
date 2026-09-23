//! Executable Thumb code in a decoded code overlay, proved by control flow.
//!
//! A byte is code only when a walk from evidence of an entry reaches it.
//! Evidence of an entry is the loader's entry veneers, the owner registers,
//! framed function pointers, and the calls, branches and switch arms of code
//! already proved. A push-shaped halfword is not evidence by itself. Three
//! kinds of candidate rest on the approved compiler's `FUNCTION_BOUNDARY` of
//! 32 bits (agscc gcc/config/arm/arm.h), which starts every function on a
//! word boundary: a framed `push {..., lr}` prologue there, a Thumb pointer
//! to one there that proved code loads, and a function boundary inside a gap
//! between proved functions. A candidate stands only when its whole walk,
//! with every new function it calls, decodes as ARMv4T Thumb and ends in
//! control flow; a gap stands only when such walks explain every byte of it.
//! One failure anywhere discards that candidate's walk entirely; no halfword
//! is demoted on its own. Code already proved never depends on a candidate.
//!
//! Instructions are decoded from the ARMv4T Thumb encoding table (ARM
//! Architecture Reference Manual; ARM7TDMI Technical Reference Manual,
//! Thumb instruction set). Undefined and unpredictable encodings are
//! invalid; every encoding the approved assembler cannot spell for
//! `-mcpu=arm7tdmi` is among them (checked exhaustively by the tests).
//! Calls through registers that link to the next instruction (`mov lr, rN`
//! with a lone BL suffix, `mov rX, pc` with BX) return. A BL is GCC's far
//! jump, which does not return, when it targets a halfword boundary or a
//! word-aligned label inside the caller's own walk.
//!
//! Owner-register entries are checked against the code the proof reaches:
//! an entry on the zero alignment halfword after a return, or inside another
//! function's straight-line code, names no function and is dropped.

use crate::overlay::assembly::{compiler_runtime_spans, ExecutableSpan};
use std::collections::{BTreeMap, BTreeSet};

/// Overlay words hold runtime addresses: the loader places every code
/// overlay 0x8000 bytes above the base its image and calls are linked for.
const RUNTIME_DISPLACEMENT: i64 = 0x8000;

/// Without a compare and branch in the same run, a switch bound is the
/// nearest `cmp index, #last` this many halfwords before the index shift.
const SWITCH_WINDOW: i64 = 12;

/// A cap on the passes that refine the literal pools and switch tables every
/// walk stops at; each pass reuses the data the previous one proved, and the
/// overlays settle in two.
const DATA_PASSES: usize = 16;

struct Image<'a> {
    bytes: &'a [u8],
    base: i64,
}

impl Image<'_> {
    fn contains(&self, address: i64, size: i64) -> bool {
        self.base <= address && address + size <= self.base + self.bytes.len() as i64
    }

    fn half(&self, address: i64) -> Option<u16> {
        self.contains(address, 2).then(|| {
            let at = (address - self.base) as usize;
            u16::from_le_bytes([self.bytes[at], self.bytes[at + 1]])
        })
    }

    fn word(&self, address: i64) -> Option<u32> {
        self.contains(address, 4).then(|| {
            let at = (address - self.base) as usize;
            u32::from_le_bytes(self.bytes[at..at + 4].try_into().unwrap())
        })
    }

    /// The image address a stored runtime code address names.
    fn stored(&self, value: u32) -> i64 {
        i64::from(value & !1) - RUNTIME_DISPLACEMENT
    }

    /// The destination of the BL pair `high`, `low`. Stored overlays carry
    /// the loader's pre-relocation displacement: destination = image base +
    /// encoded value + the Thumb bias.
    fn bl_target(&self, high: u16, low: u16) -> i64 {
        let encoded = (i64::from(high & 0x7ff) << 12) | (i64::from(low & 0x7ff) << 1);
        self.base + ((encoded << 41) >> 41) + 2
    }
}

/// `bx lr`: a function that only returns.
const BX_LR: u16 = 0x4770;

/// Every name the image gives an address, wherever it lies: Thumb pointer
/// words (word-aligned, interworking bit set) and BL-shaped halfword pairs,
/// whether or not proved code reads or executes them.
struct References {
    /// Image addresses word-aligned Thumb pointer words name.
    pointers: BTreeSet<i64>,
    /// Destinations of BL-shaped pairs at any halfword.
    calls: BTreeSet<i64>,
}

impl References {
    fn new(image: &Image) -> Self {
        let end = image.base + image.bytes.len() as i64;
        let pointers = (image.base..end - 3)
            .step_by(4)
            .filter_map(|at| {
                let value = image.word(at)?;
                let target = image.stored(value);
                (value & 1 == 1 && image.contains(target, 2)).then_some(target)
            })
            .collect();
        let calls = (image.base..end - 3)
            .step_by(2)
            .filter_map(|at| {
                let (high, low) = (image.half(at)?, image.half(at + 2)?);
                (high >> 11 == 0x1e && low >> 11 == 0x1f).then(|| image.bl_target(high, low))
            })
            .collect();
        References { pointers, calls }
    }

    /// Does any pointer word or BL-shaped pair name `address`?
    fn named(&self, address: i64) -> bool {
        self.pointers.contains(&address) || self.calls.contains(&address)
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Flow {
    /// Execution continues with the next instruction.
    Next,
    /// A BL pair: calls an image function and returns to the next instruction.
    Call(i64),
    /// A call through a register that returns to the next instruction:
    /// `mov lr, rN` then a lone BL suffix, or `mov rX, pc` then BX.
    IndirectCall,
    /// An unconditional branch.
    Jump(i64),
    /// A conditional branch: its target and the next instruction.
    Conditional(i64),
    /// BX or a pop into PC: control leaves the function.
    Leave,
    /// MOV or ADD into PC: successors come only from a proven switch table.
    Computed,
}

#[derive(Clone, Copy, Debug)]
struct Instruction {
    size: i64,
    flow: Flow,
    literal: Option<i64>,
}

fn signed(value: u16, bits: u32) -> i64 {
    let shift = 64 - bits;
    (i64::from(value) << shift) >> shift
}

fn is_mov_lr(half: u16) -> bool {
    half & 0xff87 == 0x4686
}

/// `mov rX, pc`: the register a following BX links through, when any.
fn pc_link(half: u16) -> Option<u16> {
    (half & 0xff78 == 0x4678).then_some((half & 7) | ((half >> 4) & 8))
}

/// Decode one ARMv4T Thumb instruction. `previous` is the instruction that
/// falls through into this one, if any.
fn decode(image: &Image, pc: i64, previous: Option<u16>) -> Result<Instruction, &'static str> {
    let half = image.half(pc).ok_or("runs off the image")?;
    let plain = Instruction {
        size: 2,
        flow: Flow::Next,
        literal: None,
    };
    let flow = |flow| Ok(Instruction { flow, ..plain });
    let branch = |offset: i64| {
        let target = pc + 4 + offset;
        if image.contains(target, 2) {
            Ok(target)
        } else {
            Err("branches outside the image")
        }
    };
    let register_list = half & 0xff != 0;
    match half >> 11 {
        // Shifts, three-register and immediate arithmetic, ALU operations.
        0x00..=0x07 => Ok(plain),
        0x08 if half & 0x0400 == 0 => Ok(plain),
        0x08 => high_register(half, previous),
        0x09 => {
            let word = ((pc + 4) & !3) + i64::from(half & 0xff) * 4;
            if image.contains(word, 4) {
                Ok(Instruction {
                    literal: Some(word),
                    ..plain
                })
            } else {
                Err("loads a literal outside the image")
            }
        }
        // Register, immediate, halfword and SP-relative loads and stores;
        // PC- and SP-relative address generation.
        0x0a..=0x15 => Ok(plain),
        0x16 | 0x17 => match (half >> 8) & 0xf {
            0x0 | 0x5 => Ok(plain),
            0x4 | 0xc if register_list => Ok(plain),
            0xd => flow(Flow::Leave),
            0x4 | 0xc => Err("pushes or pops an empty register list"),
            _ => Err("is not an ARMv4T miscellaneous instruction"),
        },
        0x18 | 0x19 if register_list => Ok(plain),
        0x18 | 0x19 => Err("transfers an empty register list"),
        0x1a | 0x1b => match (half >> 8) & 0xf {
            0xe => Err("is permanently undefined"),
            0xf => Ok(plain),
            _ => flow(Flow::Conditional(branch(signed(half & 0xff, 8) * 2)?)),
        },
        0x1c => flow(Flow::Jump(branch(signed(half & 0x7ff, 11) * 2)?)),
        0x1d => Err("is an ARMv5 BLX suffix"),
        0x1e => {
            let low = image
                .half(pc + 2)
                .filter(|low| low >> 11 == 0x1f)
                .ok_or("is a BL prefix without its suffix")?;
            let target = image.bl_target(half, low);
            if !image.contains(target, 2) {
                return Err("calls outside the image");
            }
            // Compiled functions start on 32-bit boundaries, so a BL to a
            // halfword boundary is GCC's Thumb far jump ("bl %l0 @ far jump"
            // in arm.md), an unconditional branch within the function. One
            // to a word boundary is a call unless the caller's walk shows
            // its target is a label of its own (`far_jumps`).
            let flow = if target % 4 == 0 {
                Flow::Call(target)
            } else {
                Flow::Jump(target)
            };
            Ok(Instruction {
                size: 4,
                flow,
                literal: None,
            })
        }
        _ if previous.is_some_and(is_mov_lr) => flow(Flow::IndirectCall),
        _ => Err("is a BL suffix without its prefix or mov lr"),
    }
}

/// Format 5: high-register ADD, CMP, MOV and BX. A BX right after
/// `mov rX, pc` into another register is a call: Thumb reads PC as the
/// instruction's address plus four, the instruction after the BX, so rX
/// holds the return address (the IWRAM routines' ip link, INCLUDE/IWRAM_CALL.H).
fn high_register(half: u16, previous: Option<u16>) -> Result<Instruction, &'static str> {
    let plain = Instruction {
        size: 2,
        flow: Flow::Next,
        literal: None,
    };
    let flow = |flow| Ok(Instruction { flow, ..plain });
    let destination = (half & 7) | ((half >> 4) & 8);
    let source = (half >> 3) & 0xf;
    match (half >> 8) & 3 {
        3 if half & 0x87 != 0 => Err("is not an ARMv4T BX"),
        3 if previous
            .and_then(pc_link)
            .is_some_and(|link| link != source && link != 15) =>
        {
            flow(Flow::IndirectCall)
        }
        3 => flow(Flow::Leave),
        _ if half & 0xc0 == 0 => Err("names two low registers in a high-register operation"),
        1 if destination == 15 || source == 15 => Err("compares the program counter"),
        2 if destination == 15 && source == 14 => flow(Flow::Leave),
        0 | 2 if destination == 15 => flow(Flow::Computed),
        _ => Ok(plain),
    }
}

/// Registers (bit mask over r0-r15) an ARMv4T Thumb instruction writes,
/// apart from the caller-saved registers a call clobbers.
fn written(half: u16) -> u16 {
    let low = |register: u16| 1u16 << register;
    match half >> 11 {
        // Shifts, add/subtract, move and add/subtract immediate; CMP writes none.
        0x00..=0x04 | 0x06 | 0x07 => low(if half >> 11 >= 4 {
            (half >> 8) & 7
        } else {
            half & 7
        }),
        0x05 => 0,
        // ALU operations; TST, CMP and CMN write none.
        0x08 if half & 0x0400 == 0 => match (half >> 6) & 0xf {
            8 | 10 | 11 => 0,
            _ => low(half & 7),
        },
        // High-register ADD and MOV.
        0x08 => match (half >> 8) & 3 {
            0 | 2 => 1 << ((half & 7) | ((half >> 4) & 8)),
            _ => 0,
        },
        0x09 | 0x14 | 0x15 => low((half >> 8) & 7),
        // Register-offset loads (LDRSB, LDR, LDRH, LDRB, LDRSH).
        0x0a | 0x0b if (half >> 9) & 7 >= 3 => low(half & 7),
        // Immediate-offset and halfword loads.
        0x0c..=0x11 if half & 0x0800 != 0 => low(half & 7),
        // SP-relative loads.
        0x12 | 0x13 if half & 0x0800 != 0 => low((half >> 8) & 7),
        // POP writes its list; STMIA and LDMIA write back their base.
        0x16 | 0x17 if (half >> 8) & 0xe == 0xc => half & 0xff,
        0x18 => low((half >> 8) & 7),
        0x19 => low((half >> 8) & 7) | (half & 0xff),
        _ => 0,
    }
}

/// What a register holds on the way to a GCC switch dispatch.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
enum Held {
    #[default]
    Unknown,
    /// A constant built from immediates, as a bound compared against.
    Constant(i64),
    /// An index whose unsigned bound (entry count) a compare and branch proved.
    Bounded(i64),
    /// Loaded from this literal word: the table's address.
    Table(i64),
    /// `lsl #2` of an index register: the shift's address, the index
    /// register, and the index's proved entry count if any.
    Scaled(i64, u16, Option<i64>),
    /// Loaded from [table, scaled]: an arm address.
    Arm(i64, i64, u16, Option<i64>),
}

/// Registers tracked along one straight-line run, to prove GCC's Thumb switch
/// dispatch whatever the scheduler put between its steps:
///
///   cmp  index, #last
///   ldr  table, [pc, #pool]
///   lsl  scaled, index, #2
///   ldr  target, [table, scaled]
///   mov  pc, target               (possibly after copies or a branch)
///
/// The pool word holds the table's runtime address; the word-aligned table
/// holds one runtime arm address per index the compare admits: `last + 1`
/// after `cmp index, #last` with BHI or BLS, `count` after `cmp index, count`
/// with BCS or BCC.
#[derive(Clone, Default)]
struct Registers {
    held: [Held; 16],
    /// The register and bound of an unsigned compare awaiting its branch.
    compare: Option<(u16, i64)>,
}

impl Registers {
    fn step(&mut self, pc: i64, half: u16, instruction: &Instruction) {
        let compare = self.compare.take();
        let held = &mut self.held;
        let register = |r: u16| usize::from(r);
        if matches!(instruction.flow, Flow::Call(_) | Flow::IndirectCall) || half >> 8 == 0xdf {
            for clobbered in [0, 1, 2, 3, 12, 14] {
                held[clobbered] = Held::Unknown;
            }
            return;
        }
        if half & 0xf800 == 0x2800 {
            self.compare = Some(((half >> 8) & 7, i64::from(half & 0xff)));
            return;
        }
        if half & 0xffc0 == 0x4280 {
            if let Held::Constant(bound) = held[register((half >> 3) & 7)] {
                self.compare = Some((half & 7, bound));
            }
            return;
        }
        if let (Flow::Conditional(_), Some((index, bound))) = (instruction.flow, compare) {
            let count = match (half >> 8) & 0xf {
                8 | 9 => Some(bound + 1),
                2 | 3 => Some(bound),
                _ => None,
            };
            if let Some(count) = count {
                held[register(index)] = Held::Bounded(count);
            }
            return;
        }
        let constant = |r: u16| match held[register(r)] {
            Held::Constant(value) => Some(value),
            _ => None,
        };
        let copied = if half & 0xffc0 == 0x1c00 {
            // adds rd, rn, #0: GCC's low-register move.
            Some(held[register((half >> 3) & 7)])
        } else if half & 0xff00 == 0x4600 {
            Some(held[register((half >> 3) & 0xf)])
        } else {
            None
        };
        let value = if let Some(word) = instruction.literal {
            Held::Table(word)
        } else if half & 0xf800 == 0x2000 {
            Held::Constant(i64::from(half & 0xff))
        } else if half & 0xf800 == 0 && constant((half >> 3) & 7).is_some() {
            let shifted = constant((half >> 3) & 7).unwrap() << ((half >> 6) & 0x1f);
            Held::Constant(shifted & 0xffff_ffff)
        } else if half & 0xffc0 == 0x0080 {
            let index = (half >> 3) & 7;
            let count = match held[register(index)] {
                Held::Bounded(count) => Some(count),
                _ => None,
            };
            Held::Scaled(pc, index, count)
        } else if half & 0xfe00 == 0x5800 {
            match (
                held[register((half >> 3) & 7)],
                held[register((half >> 6) & 7)],
            ) {
                (Held::Table(word), Held::Scaled(at, index, count))
                | (Held::Scaled(at, index, count), Held::Table(word)) => {
                    Held::Arm(word, at, index, count)
                }
                _ => Held::Unknown,
            }
        } else {
            copied.unwrap_or_default()
        };
        let writes = written(half);
        for (target, slot) in held.iter_mut().enumerate() {
            if writes & (1 << target) != 0 {
                *slot = value;
            }
        }
    }

    /// The table and arms of a dispatch `mov pc, rX` reached with these
    /// registers. Without a compare and branch in the same run, the bound
    /// is the nearest `cmp index, #last` before the shift.
    fn dispatch(&self, image: &Image, half: u16) -> Option<(i64, Vec<i64>)> {
        if half & 0xff87 != 0x4687 {
            return None;
        }
        let Held::Arm(word, shift, index, count) = self.held[usize::from((half >> 3) & 0xf)] else {
            return None;
        };
        let count = count.or_else(|| {
            (1..=SWITCH_WINDOW)
                .filter_map(|back| image.half(shift - 2 * back))
                .find(|h| h & 0xf800 == 0x2800 && (h >> 8) & 7 == index)
                .map(|h| i64::from(h & 0xff) + 1)
        })?;
        let table = image.stored(image.word(word)?);
        if table % 4 != 0 || !image.contains(table, count * 4) {
            return None;
        }
        let arms = (0..count)
            .map(|entry| image.stored(image.word(table + entry * 4).unwrap()))
            .collect::<Vec<_>>();
        arms.iter()
            .all(|arm| image.contains(*arm, 2))
            .then_some((table, arms))
    }
}

/// Why a walk from an entry could not be proved.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Failure {
    /// The function entry the walk started from.
    pub entry: i64,
    /// The start of the straight-line run that failed.
    pub run: i64,
    /// The address where it failed.
    pub at: i64,
    pub reason: &'static str,
}

/// One function: its instructions and the data its instructions read.
#[derive(Clone, Default)]
struct Walk {
    instructions: BTreeMap<i64, Instruction>,
    literals: BTreeSet<i64>,
    tables: BTreeMap<i64, i64>,
    calls: BTreeSet<i64>,
    /// BL sites far-jumping to a word-aligned label, with that label.
    far_jumps: BTreeMap<i64, i64>,
    pointers: BTreeSet<i64>,
    unresolved: Vec<i64>,
    resolved: Vec<i64>,
    failures: Vec<Failure>,
}

/// Code proved so far: every instruction with its function, and every
/// literal word and switch table those instructions read.
#[derive(Clone, Default)]
struct Code {
    instructions: BTreeMap<i64, (Instruction, i64)>,
    literals: BTreeSet<i64>,
    tables: BTreeMap<i64, i64>,
    data: BTreeSet<i64>,
    functions: BTreeMap<i64, Evidence>,
    far_jumps: BTreeMap<i64, i64>,
    unresolved_jumps: BTreeSet<i64>,
    resolved_jumps: BTreeSet<i64>,
}

impl Code {
    fn instruction(&self, address: i64) -> Option<&(Instruction, i64)> {
        self.instructions.get(&address)
    }

    /// Is `address` the second halfword of a proved BL pair?
    fn inside(&self, address: i64) -> bool {
        self.instructions
            .get(&(address - 2))
            .is_some_and(|(instruction, _)| instruction.size == 4)
    }

    fn commit(&mut self, entry: i64, evidence: Evidence, walk: &Walk) {
        if !walk.instructions.is_empty() {
            self.functions.entry(entry).or_insert(evidence);
        }
        for (&at, &instruction) in &walk.instructions {
            self.instructions.insert(at, (instruction, entry));
        }
        for &word in &walk.literals {
            self.literals.insert(word);
            self.data.extend(word..word + 4);
        }
        for (&table, &size) in &walk.tables {
            self.tables.insert(table, size);
            self.data.extend(table..table + size);
        }
        self.far_jumps.extend(&walk.far_jumps);
        self.unresolved_jumps
            .extend(walk.unresolved.iter().copied());
        self.resolved_jumps.extend(walk.resolved.iter().copied());
    }

    /// Instructions that overlap a literal word or a switch table.
    fn conflicts(&self) -> Vec<i64> {
        self.instructions
            .iter()
            .filter(|(at, (instruction, _))| {
                (**at..**at + instruction.size).any(|byte| self.data.contains(&byte))
            })
            .map(|(at, _)| *at)
            .collect()
    }
}

/// Does the run end with a call that may not return? A noreturn call can be
/// the last instruction of its function, followed by a literal pool or the
/// next function, optionally after one zero halfword of word alignment.
fn after_call(run: &[(i64, Instruction, u16)]) -> Option<usize> {
    let is_call = |entry: &(i64, Instruction, u16)| {
        matches!(entry.1.flow, Flow::Call(_) | Flow::IndirectCall)
    };
    match run {
        [.., last] if is_call(last) => Some(run.len()),
        [.., call, pad] if is_call(call) && pad.2 == 0 && pad.0 % 4 == 2 => Some(run.len() - 1),
        _ => None,
    }
}

/// Walk one function from `entry` (see `walk`), deciding which of its BLs to
/// word boundaries are far jumps. GCC's Thumb far jump ("bl %l0 @ far jump"
/// in arm.md, alone or as `b<inverse> .+6; bl` for a conditional branch out
/// of range) can name a label on a word boundary as well as a halfword one.
/// Compiled functions are contiguous and never nest, so a BL whose
/// word-aligned target the caller's own walk reaches by other control flow
/// (such as an epilogue the preceding instruction falls into), or that lies
/// between the caller's first and last proved instructions, jumps to a label
/// of the caller. The walk is then repeated with those BLs as jumps, so their
/// targets join the caller instead of being recorded as called functions.
/// A BL to the caller's own entry recurses, and one to code another function
/// already proved stays a call.
fn walk_function(
    image: &Image,
    entry: i64,
    code: &Code,
    stops: &BTreeSet<i64>,
    strict: bool,
) -> Walk {
    let calls = walk(image, entry, code, stops, strict, &BTreeSet::new());
    let far = far_jumps(entry, &calls, code);
    if far.is_empty() {
        calls
    } else {
        walk(image, entry, code, stops, strict, &far)
    }
}

/// The BL sites of `walk` (every BL to a word boundary taken as a call) that
/// are far jumps to labels inside the function it walked from `entry`.
fn far_jumps(entry: i64, walk: &Walk, code: &Code) -> BTreeSet<i64> {
    let (Some(&first), Some(&last)) = (
        walk.instructions.keys().next(),
        walk.instructions.keys().next_back(),
    ) else {
        return BTreeSet::new();
    };
    walk.instructions
        .iter()
        .filter_map(|(&site, instruction)| match instruction.flow {
            Flow::Call(target)
                if target != entry
                    && code.instruction(target).is_none()
                    && (walk.instructions.contains_key(&target)
                        || (first < target && target < last)) =>
            {
                Some(site)
            }
            _ => None,
        })
        .collect()
}

/// Walk one function from `entry`, stopping at code already proved, at
/// `stops` (data the previous pass proved) and at data its own instructions
/// read. Each straight-line run commits only when it ends in control flow.
/// A `strict` walk (a candidate's) may not fall through into another
/// function except after a call; an entry on evidence may share a tail with
/// the function it falls into, as hand-written routines with several entry
/// points do. The BLs at `far` sites are far jumps.
fn walk(
    image: &Image,
    entry: i64,
    code: &Code,
    stops: &BTreeSet<i64>,
    strict: bool,
    far: &BTreeSet<i64>,
) -> Walk {
    let mut walk = Walk::default();
    // Each branch target starts with the registers tracked at its branch.
    let mut queue = vec![(entry, Registers::default())];
    let mut queued = BTreeSet::from([entry]);
    while let Some((start, entering)) = queue.pop() {
        if walk.instructions.contains_key(&start) || code.instruction(start).is_some() {
            continue;
        }
        let mut run: Vec<(i64, Instruction, u16)> = Vec::new();
        let mut targets = Vec::new();
        let mut literals = BTreeSet::new();
        let mut tables = Vec::new();
        let mut unresolved = None;
        let mut resolved = None;
        let mut registers = entering;
        let mut pc = start;
        let is_data = |address: i64, literals: &BTreeSet<i64>| {
            stops.contains(&address)
                || code.data.contains(&address)
                || walk
                    .literals
                    .iter()
                    .chain(literals)
                    .any(|word| (*word..*word + 4).contains(&address))
                || walk
                    .tables
                    .iter()
                    .any(|(table, size)| (*table..*table + *size).contains(&address))
        };
        let outcome: Result<(), (i64, &'static str)> = loop {
            if walk.instructions.contains_key(&pc) {
                break Ok(());
            }
            if let Some((_, owner)) = code.instruction(pc) {
                if *owner == entry || !strict {
                    break Ok(());
                }
                match after_call(&run) {
                    Some(keep) => {
                        run.truncate(keep);
                        break Ok(());
                    }
                    None => break Err((pc, "falls through into another function")),
                }
            }
            let middle = code.inside(pc)
                || walk
                    .instructions
                    .get(&(pc - 2))
                    .is_some_and(|instruction| instruction.size == 4);
            if middle {
                break Err((pc, "enters the middle of a BL pair"));
            }
            if is_data(pc, &literals) {
                match after_call(&run) {
                    Some(keep) => {
                        run.truncate(keep);
                        break Ok(());
                    }
                    None => break Err((pc, "falls into data")),
                }
            }
            let previous = run.last().map(|(_, _, half)| *half);
            let instruction = match decode(image, pc, previous) {
                Ok(Instruction {
                    flow: Flow::Call(target),
                    size,
                    literal,
                }) if far.contains(&pc) => Instruction {
                    flow: Flow::Jump(target),
                    size,
                    literal,
                },
                Ok(instruction) => instruction,
                Err(reason) => break Err((pc, reason)),
            };
            if instruction.size == 4
                && (is_data(pc + 2, &literals)
                    || code.instruction(pc + 2).is_some()
                    || walk.instructions.contains_key(&(pc + 2)))
            {
                break Err((pc, "overlaps proved code or data"));
            }
            let half = image.half(pc).unwrap();
            run.push((pc, instruction, half));
            if let Some(word) = instruction.literal {
                literals.insert(word);
            }
            registers.step(pc, half, &instruction);
            let arms = |table: i64, arms: Vec<i64>, targets: &mut Vec<_>, tables: &mut Vec<_>| {
                tables.push((table, arms.len() as i64 * 4));
                targets.extend(arms.into_iter().map(|arm| (arm, Registers::default())));
            };
            match instruction.flow {
                Flow::Next | Flow::IndirectCall | Flow::Call(_) => {}
                Flow::Conditional(target) => targets.push((target, registers.clone())),
                Flow::Jump(target) => {
                    targets.push((target, registers.clone()));
                    // A branch to a shared dispatch carries the table with it.
                    if let Some(dispatch) = image.half(target) {
                        if let Some((table, found)) = registers.dispatch(image, dispatch) {
                            arms(table, found, &mut targets, &mut tables);
                            resolved = Some(target);
                        }
                    }
                    break Ok(());
                }
                Flow::Leave => break Ok(()),
                Flow::Computed => {
                    match registers.dispatch(image, half) {
                        Some((table, found)) => {
                            arms(table, found, &mut targets, &mut tables);
                            resolved = Some(pc);
                        }
                        None => unresolved = Some(pc),
                    }
                    break Ok(());
                }
            }
            pc += instruction.size;
        };
        if let Err((at, reason)) = outcome {
            walk.failures.push(Failure {
                entry,
                run: start,
                at,
                reason,
            });
            continue;
        }
        for (at, instruction, _) in run {
            walk.instructions.insert(at, instruction);
            match instruction.flow {
                Flow::Call(target) => {
                    walk.calls.insert(target);
                }
                Flow::Jump(target) if far.contains(&at) => {
                    walk.far_jumps.insert(at, target);
                }
                _ => {}
            }
        }
        for word in literals {
            // A Thumb pointer to a word boundary that proved code loads is a
            // candidate entry (validated like a prologue).
            let value = image.word(word).unwrap();
            let pointed = image.stored(value);
            if value & 1 == 1 && pointed % 4 == 0 && image.contains(pointed, 2) {
                walk.pointers.insert(pointed);
            }
            walk.literals.insert(word);
        }
        for (table, size) in tables {
            walk.tables.insert(table, size);
        }
        walk.unresolved.extend(unresolved);
        walk.resolved.extend(resolved);
        for (target, registers) in targets {
            if queued.insert(target) {
                queue.push((target, registers));
            }
        }
    }
    walk
}

/// Where a walk started, which decides how a failure is handled.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Evidence {
    /// A loader entry veneer's target.
    EntryVeneer,
    /// An owner register entry.
    Owner,
    /// A word-aligned framed function a stored Thumb pointer names.
    FramedPointer,
    /// A call from proved code.
    Call,
    /// The first routine after the entry veneer table (candidate).
    Layout,
    /// A word-aligned Thumb pointer proved code loads (candidate).
    LoadedPointer,
    /// A word-aligned framed prologue (candidate).
    Prologue,
    /// A function boundary inside a gap between proved bytes that nothing
    /// names (candidate): the gap is kept only if complete walks explain
    /// every byte of it.
    Contiguous,
    /// A function boundary inside a gap that a stored Thumb pointer word
    /// names, such as an unframed leaf in a callback table (candidate): kept
    /// only as `Contiguous` is, with its whole gap.
    StoredPointer,
}

impl Evidence {
    pub fn name(self) -> &'static str {
        match self {
            Evidence::EntryVeneer => "entry_veneer",
            Evidence::Owner => "owner",
            Evidence::FramedPointer => "framed_pointer",
            Evidence::Call => "call",
            Evidence::Layout => "layout",
            Evidence::LoadedPointer => "loaded_pointer",
            Evidence::Prologue => "prologue",
            Evidence::Contiguous => "contiguous",
            Evidence::StoredPointer => "stored_pointer",
        }
    }

    /// Candidates are accepted only when their complete walk succeeds.
    fn candidate(self) -> bool {
        self >= Evidence::Layout
    }
}

/// A candidate entry whose walk failed and was discarded whole.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Rejection {
    pub seed: i64,
    pub evidence: Evidence,
    pub failure: Failure,
}

/// What one pass of the proof kept and discarded.
#[derive(Default)]
struct Account {
    failed_runs: Vec<Failure>,
    rejected: Vec<Rejection>,
    accepted: Vec<(i64, Evidence)>,
}

/// The proved code of one overlay and what the proof discarded.
#[derive(Default)]
pub struct OverlayCode {
    pub spans: Vec<ExecutableSpan>,
    /// Straight-line runs that failed inside functions entered on evidence.
    pub failed_runs: Vec<Failure>,
    /// Candidate entries discarded with their whole walk.
    pub rejected: Vec<Rejection>,
    /// Candidate entries whose complete walk was proved.
    pub accepted: Vec<(i64, Evidence)>,
    /// Every function entry the proof walked, with the evidence that named it.
    pub functions: BTreeMap<i64, Evidence>,
    /// Owner-register entries that name no function, and why.
    pub rejected_owners: Vec<(i64, &'static str)>,
    /// BL sites that far-jump to a word-aligned label of their own function,
    /// with the label.
    pub far_jumps: BTreeMap<i64, i64>,
    /// MOV/ADD into PC with no proved switch table (arms not followed).
    pub unresolved_jumps: Vec<i64>,
    /// Proved instructions overlapping proved data after the final pass.
    pub conflicts: Vec<i64>,
    pub passes: usize,
}

/// Fixed `ldr r4, [pc, #0]; bx r4` veneers anywhere at word boundaries whose
/// target is addressable: import veneers into the main image and IWRAM, and
/// the loader's entry table. The target word is data.
pub fn veneer_spans(input: &[u8], base: i64) -> Vec<ExecutableSpan> {
    (0..input.len().saturating_sub(7))
        .step_by(4)
        .filter(|&offset| input[offset..offset + 4] == [0x00, 0x4c, 0x20, 0x47])
        .filter(|&offset| {
            let target = u32::from_le_bytes(input[offset + 4..offset + 8].try_into().unwrap());
            // bx selects ARM or Thumb from bit zero. Even IWRAM targets are
            // real ARM import veneers, not data to omit from the image.
            matches!(target >> 24, 0x02 | 0x03 | 0x08 | 0x09)
        })
        .map(|offset| ExecutableSpan {
            start: base + offset as i64,
            end: base + offset as i64 + 8,
            kind: "veneer",
        })
        .collect()
}

/// Compiled Thumb functions named by in-image function pointers: callback and
/// scene tables, or literal words code passes to the engine. A word names one
/// only when it is a Thumb pointer (interworking bit set) to an in-image
/// address that is word-aligned, as the approved compiler's
/// `FUNCTION_BOUNDARY` of 32 bits requires (agscc gcc/config/arm/arm.h), and
/// that address opens with a framed `push {..., lr}` prologue: the rule the
/// main image applies to static callback tables. An odd byte pointer into
/// data can take the pointer shape, so unframed leaves need owner-register
/// evidence instead.
fn framed_function_pointers(image: &Image) -> Vec<i64> {
    (image.base..image.base + image.bytes.len() as i64 - 3)
        .step_by(4)
        .filter_map(|at| {
            let value = image.word(at)?;
            let target = image.stored(value);
            let framed = image
                .half(target)
                .is_some_and(|half| half & 0xff00 == 0xb500);
            (value & 1 == 1 && target % 4 == 0 && framed).then_some(target)
        })
        .collect()
}

/// Walk `seeds` to closure over their calls. Entries on evidence commit each
/// successful run; a candidate commits only if its walk and the walks of every
/// new function it calls all succeed. `fixed` spans (veneers, the runtime
/// bank) bound the gaps between proved bytes like proved code does.
fn close(
    image: &Image,
    seeds: &[(i64, Evidence)],
    fixed: &[(i64, i64)],
    stops: &BTreeSet<i64>,
    references: &References,
    code: &mut Code,
    account: &mut Account,
) {
    let mut candidates: Vec<(i64, Evidence)> = Vec::new();
    let mut pending: std::collections::VecDeque<(i64, Evidence)> = seeds
        .iter()
        .filter(|(_, evidence)| !evidence.candidate())
        .copied()
        .collect();
    candidates.extend(
        seeds
            .iter()
            .filter(|(_, evidence)| evidence.candidate())
            .copied(),
    );
    let mut tried = BTreeSet::new();
    let mut tried_gaps = BTreeSet::new();
    loop {
        while let Some((entry, evidence)) = pending.pop_front() {
            if code.instruction(entry).is_some() {
                continue;
            }
            let function = walk_function(image, entry, code, stops, false);
            code.commit(entry, evidence, &function);
            account.failed_runs.extend(function.failures);
            pending.extend(function.calls.iter().map(|&call| (call, Evidence::Call)));
            candidates.extend(
                function
                    .pointers
                    .iter()
                    .map(|&pointer| (pointer, Evidence::LoadedPointer)),
            );
        }
        candidates.sort_unstable();
        candidates.dedup_by_key(|(at, _)| *at);
        let open = |code: &Code, at: i64| {
            code.instruction(at).is_none()
                && !code.inside(at)
                && !code.data.contains(&at)
                && !stops.contains(&at)
        };
        if let Some(&(seed, evidence)) = candidates
            .iter()
            .find(|(at, _)| !tried.contains(at) && open(code, *at))
        {
            tried.insert(seed);
            match trial(image, seed, evidence, code, stops) {
                Ok((proved, pointers)) => {
                    *code = proved;
                    account.accepted.push((seed, evidence));
                    candidates.extend(pointers.into_iter().map(|at| (at, Evidence::LoadedPointer)));
                }
                Err(failure) => account.rejected.push(Rejection {
                    seed,
                    evidence,
                    failure,
                }),
            }
            continue;
        }
        let Some((start, end)) = gaps(image, code, fixed, stops)
            .into_iter()
            .find(|(start, _)| !tried_gaps.contains(start))
        else {
            break;
        };
        tried_gaps.insert(start);
        match fill(image, start, end, code, stops, references) {
            Ok((proved, entries, pointers)) => {
                *code = proved;
                account.accepted.extend(entries);
                candidates.extend(pointers.into_iter().map(|at| (at, Evidence::LoadedPointer)));
            }
            Err(failure) => account.rejected.push(Rejection {
                seed: start,
                evidence: Evidence::Contiguous,
                failure,
            }),
        }
    }
}

/// Does the proved halfword at `before` end a function: data (a literal pool
/// or switch table) or an instruction after which control cannot continue?
fn ends_function(code: &Code, before: i64) -> bool {
    match code.instruction(before) {
        Some((instruction, _)) => matches!(instruction.flow, Flow::Leave | Flow::Jump(_)),
        None => !code.inside(before),
    }
}

/// Does proved code or data, or a fixed span, cover the halfword at `at`?
fn claimed(code: &Code, fixed: &[(i64, i64)], stops: &BTreeSet<i64>, at: i64) -> bool {
    code.instruction(at).is_some()
        || code.inside(at)
        || code.data.contains(&at)
        || stops.contains(&at)
        || fixed
            .iter()
            .any(|(start, end)| (*start..*end).contains(&at))
}

/// Gaps between proved bytes that could hold unreferenced functions: each
/// is bounded by proved bytes on both sides, holds a nonzero byte, follows
/// the end of a function (a literal pool, a switch table, a fixed span, or
/// an instruction that leaves), and begins at a function boundary once one
/// zero alignment halfword is skipped. Bytes after an unresolved computed
/// jump or any instruction that continues are inside a function, not a gap.
/// Returns (first function boundary, end of the gap).
fn gaps(
    image: &Image,
    code: &Code,
    fixed: &[(i64, i64)],
    stops: &BTreeSet<i64>,
) -> Vec<(i64, i64)> {
    let ends_function = |before: i64| ends_function(code, before);
    let end = image.base + image.bytes.len() as i64;
    let mut out = Vec::new();
    let mut at = image.base;
    let mut open: Option<i64> = None;
    while at < end {
        let covered = claimed(code, fixed, stops, at);
        match (open, covered) {
            (None, false) if at > image.base && ends_function(at - 2) => open = Some(at),
            (None, false) => {
                // Inside a function: skip to the next proved byte.
                while at < end && !claimed(code, fixed, stops, at) {
                    at += 2;
                }
                continue;
            }
            (Some(start), true) => {
                open = None;
                let nonzero = (start..at)
                    .step_by(2)
                    .any(|half| image.half(half) != Some(0));
                let boundary = if start % 4 == 2 && image.half(start) == Some(0) {
                    start + 2
                } else {
                    start
                };
                if nonzero && boundary % 4 == 0 && boundary < at {
                    out.push((boundary, at));
                }
            }
            _ => {}
        }
        at += 2;
    }
    out
}

/// Explain a whole gap as consecutive functions: from each word boundary, a
/// candidate walk (with every new function it calls) must succeed, and the
/// instructions, literal pools and switch tables it proves, with at most one
/// zero halfword of alignment before each next boundary, must cover every
/// byte up to `end`. One unexplained byte or failed walk discards the gap.
/// A boundary a stored Thumb pointer names is recorded as such. A lone
/// `bx lr` (with its alignment) that no pointer word or BL anywhere names is
/// only layout, as a returning halfword and a zero pad could be any data, so
/// it explains nothing.
fn fill(
    image: &Image,
    start: i64,
    end: i64,
    code: &Code,
    stops: &BTreeSet<i64>,
    references: &References,
) -> Result<(Code, Vec<(i64, Evidence)>, Vec<i64>), Failure> {
    let mut proved = code.clone();
    let mut entries = Vec::new();
    let mut pointers = Vec::new();
    let mut at = start;
    while at < end {
        let covered = |code: &Code, at: i64| {
            code.instruction(at).is_some() || code.inside(at) || code.data.contains(&at)
        };
        if covered(&proved, at) {
            at += 2;
            continue;
        }
        if at % 4 == 2 && image.half(at) == Some(0) {
            at += 2;
            continue;
        }
        let previous = if image.half(at - 2) == Some(0) && !covered(&proved, at - 2) {
            at - 4
        } else {
            at - 2
        };
        if at % 4 != 0 || (at > start && !ends_function(&proved, previous)) {
            return Err(Failure {
                entry: start,
                run: start,
                at,
                reason: "leaves bytes no complete function explains",
            });
        }
        let evidence = if references.pointers.contains(&at) {
            Evidence::StoredPointer
        } else {
            Evidence::Contiguous
        };
        let (next, found) = trial(image, at, evidence, &proved, stops)?;
        if !covered(&next, at) {
            return Err(Failure {
                entry: at,
                run: at,
                at,
                reason: "proves no instruction at a function boundary",
            });
        }
        let lone_return = image.half(at) == Some(BX_LR)
            && next.instructions.len() == proved.instructions.len() + 1;
        if lone_return && !references.named(at) {
            return Err(Failure {
                entry: at,
                run: at,
                at,
                reason: "is a lone bx lr nothing names",
            });
        }
        proved = next;
        entries.push((at, evidence));
        pointers.extend(found);
    }
    Ok((proved, entries, pointers))
}

/// Walk a candidate entry and every new function it calls. The candidate
/// stands only if every run of every walk ends in control flow and none of
/// the data its instructions read overlaps an instruction.
fn trial(
    image: &Image,
    seed: i64,
    evidence: Evidence,
    code: &Code,
    stops: &BTreeSet<i64>,
) -> Result<(Code, Vec<i64>), Failure> {
    let mut trial = code.clone();
    let mut queue = vec![seed];
    let mut walked = BTreeSet::new();
    let mut walks = Vec::new();
    while let Some(entry) = queue.pop() {
        if !walked.insert(entry) || trial.instruction(entry).is_some() {
            continue;
        }
        let function = walk_function(image, entry, &trial, stops, true);
        if let Some(first) = function.failures.first() {
            return Err(first.clone());
        }
        let evidence = if entry == seed {
            evidence
        } else {
            Evidence::Call
        };
        trial.commit(entry, evidence, &function);
        queue.extend(function.calls.iter().copied());
        walks.push(function);
    }
    for function in &walks {
        let reads_code = function
            .literals
            .iter()
            .flat_map(|word| *word..*word + 4)
            .chain(
                function
                    .tables
                    .iter()
                    .flat_map(|(table, size)| *table..*table + *size),
            )
            .find(|byte| trial.instruction(*byte).is_some() || trial.inside(*byte));
        let overlaps_data = function.instructions.iter().find_map(|(at, instruction)| {
            (*at..*at + instruction.size)
                .find(|byte| trial.data.contains(byte))
                .map(|_| *at)
        });
        if let Some(at) = reads_code.or(overlaps_data) {
            return Err(Failure {
                entry: seed,
                run: seed,
                at,
                reason: "reads its own code as a literal or switch table",
            });
        }
    }
    let pointers = walks
        .iter()
        .flat_map(|function| function.pointers.iter().copied())
        .collect();
    Ok((trial, pointers))
}

/// Close `seeds` over their calls, candidates and gaps, pass after pass, each
/// pass stopping every walk at the data the previous one proved, until the
/// data settles. Returns the code, its account and the passes taken.
fn settle(
    image: &Image,
    seeds: &[(i64, Evidence)],
    fixed: &[(i64, i64)],
    veneer_words: &BTreeSet<i64>,
    references: &References,
) -> (Code, Account, usize) {
    let mut stops = veneer_words.clone();
    let mut pass = 1;
    loop {
        let mut code = Code::default();
        let mut account = Account::default();
        close(
            image,
            seeds,
            fixed,
            &stops,
            references,
            &mut code,
            &mut account,
        );
        let data = code
            .data
            .iter()
            .copied()
            .chain(veneer_words.iter().copied())
            .collect::<BTreeSet<_>>();
        if data == stops || pass == DATA_PASSES {
            return (code, account, pass);
        }
        // Later passes only add stops, so the walks cannot oscillate.
        stops = if pass < DATA_PASSES / 2 {
            data
        } else {
            stops.union(&data).copied().collect()
        };
        pass += 1;
    }
}

/// Why an owner-register entry names no function, judged by the proved
/// instruction just before it. Compiled functions start on word boundaries
/// after their predecessor ends: a zero halfword off a word boundary right
/// after an unconditional return is the alignment before the next function,
/// and an entry another function's straight-line code runs into (a fall
/// through or the untaken path of a conditional branch) is a point inside
/// that function, such as the middle of its prologue. Returns the reason and
/// the function whose walk proved that instruction.
fn invalid_owner(image: &Image, code: &Code, entry: i64) -> Option<(&'static str, i64)> {
    let (previous, owner) = code
        .instruction(entry - 2)
        .filter(|(instruction, _)| instruction.size == 2)?;
    match previous.flow {
        Flow::Leave if entry % 4 == 2 && image.half(entry) == Some(0) => {
            Some(("is the zero alignment halfword after a return", *owner))
        }
        Flow::Next | Flow::Conditional(_) if *owner != entry => {
            Some(("lies inside another function's straight-line code", *owner))
        }
        _ => None,
    }
}

/// Prove the executable code of a decoded code overlay. `entry_veneers` fixed
/// veneers open the image; `entries` are owner-register entry points.
pub fn overlay_code(
    input: &[u8],
    base: i64,
    entry_veneers: usize,
    entries: &[i64],
) -> Result<OverlayCode, String> {
    if !input.len().is_multiple_of(2) {
        return Err("overlay has an odd byte length".into());
    }
    if input.len() < entry_veneers * 8 {
        return Err("overlay is shorter than its entry veneer table".into());
    }
    let image = Image { bytes: input, base };
    let mut veneered = Vec::new();
    for entry in 0..entry_veneers {
        let at = base + entry as i64 * 8;
        if image.word(at) != Some(0x4720_4c00) {
            return Err(format!("overlay entry {entry} is not a fixed veneer"));
        }
        let target = image.stored(image.word(at + 4).unwrap());
        if !image.contains(target, 2) {
            return Err(format!("overlay entry {entry} target is outside the image"));
        }
        veneered.push((target, Evidence::EntryVeneer));
    }
    if let Some(entry) = entries.iter().find(|entry| !image.contains(**entry, 2)) {
        return Err(format!("owner entry 0x{entry:08x} is outside the image"));
    }
    let mut framed_and_candidates = framed_function_pointers(&image)
        .into_iter()
        .map(|at| (at, Evidence::FramedPointer))
        .collect::<Vec<_>>();
    // Camelot overlays place their first local routine immediately after the
    // fixed loader veneer table; it is a candidate like any prologue.
    framed_and_candidates.push((base + entry_veneers as i64 * 8, Evidence::Layout));
    framed_and_candidates.extend(
        (base..base + input.len() as i64 - 1)
            .step_by(4)
            .filter(|&at| image.half(at).is_some_and(|half| half & 0xff00 == 0xb500))
            .map(|at| (at, Evidence::Prologue)),
    );
    // A fixed veneer's target word is data whether or not a call reaches it.
    let veneers = veneer_spans(input, base);
    let veneer_words = veneers
        .iter()
        .flat_map(|span| span.start + 4..span.end)
        .collect::<BTreeSet<_>>();
    let fixed = veneers
        .iter()
        .chain(&compiler_runtime_spans(input, base))
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    let references = References::new(&image);
    // Each round drops the owner entries the code it proved contradicts, and
    // proves again without them, until every remaining entry stands. An entry
    // judged by the walk of another entry dropped in the same round (the code
    // after a rejected alignment halfword) is judged again without it.
    let mut owners = entries.to_vec();
    let mut rejected_owners = Vec::new();
    let (code, account, passes) = loop {
        let seeds = veneered
            .iter()
            .copied()
            .chain(owners.iter().map(|&entry| (entry, Evidence::Owner)))
            .chain(framed_and_candidates.iter().copied())
            .collect::<Vec<_>>();
        let (code, account, passes) = settle(&image, &seeds, &fixed, &veneer_words, &references);
        let invalid = owners
            .iter()
            .filter_map(|&entry| {
                invalid_owner(&image, &code, entry).map(|(reason, by)| (entry, reason, by))
            })
            .collect::<Vec<_>>();
        let Some(&(first, first_reason, _)) = invalid.first() else {
            break (code, account, passes);
        };
        let mut dropped = invalid
            .iter()
            .filter(|(_, _, by)| !invalid.iter().any(|(entry, _, _)| entry == by))
            .map(|&(entry, reason, _)| (entry, reason))
            .collect::<Vec<_>>();
        if dropped.is_empty() {
            // Entries that only judge one another: drop one to make progress.
            dropped.push((first, first_reason));
        }
        owners.retain(|entry| !dropped.iter().any(|(at, _)| at == entry));
        rejected_owners.extend(dropped);
    };
    let mut result = OverlayCode {
        spans: Vec::new(),
        failed_runs: account.failed_runs,
        rejected: account.rejected,
        accepted: account.accepted,
        functions: code.functions.clone(),
        rejected_owners,
        far_jumps: code.far_jumps.clone(),
        unresolved_jumps: code
            .unresolved_jumps
            .difference(&code.resolved_jumps)
            .copied()
            .collect(),
        conflicts: code.conflicts(),
        passes,
    };
    for (&at, (instruction, _)) in &code.instructions {
        result.spans.push(ExecutableSpan {
            start: at,
            end: at + instruction.size,
            kind: "thumb",
        });
    }
    for &word in &code.literals {
        result.spans.push(ExecutableSpan {
            start: word,
            end: word + 4,
            kind: "literal_pool",
        });
    }
    for (&table, &size) in &code.tables {
        result.spans.push(ExecutableSpan {
            start: table,
            end: table + size,
            kind: "jump_table",
        });
    }
    Ok(result)
}

/// A zero halfword between executable bytes aligns code, a literal pool or a
/// switch table to a word boundary.
pub fn alignment_spans(input: &[u8], base: i64, spans: &[ExecutableSpan]) -> Vec<ExecutableSpan> {
    let mut ranges = spans
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        match merged.last_mut() {
            Some(last) if range.0 <= last.1 => last.1 = last.1.max(range.1),
            _ => merged.push(range),
        }
    }
    merged
        .windows(2)
        .filter(|pair| pair[1].0 - pair[0].1 == 2)
        .filter(|pair| {
            let at = (pair[0].1 - base) as usize;
            input.get(at..at + 2) == Some(&[0, 0])
        })
        .map(|pair| ExecutableSpan {
            start: pair[0].1,
            end: pair[1].0,
            kind: "executable_alignment",
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::overlay::assembly::OVERLAY_BASE;

    const BASE: i64 = OVERLAY_BASE;

    /// An image of `size` bytes holding `halves` at their offsets.
    fn image(size: usize, halves: &[(usize, u16)]) -> Vec<u8> {
        let mut image = vec![0u8; size];
        for &(at, half) in halves {
            image[at..at + 2].copy_from_slice(&half.to_le_bytes());
        }
        image
    }

    fn word(image: &mut [u8], at: usize, value: u32) {
        image[at..at + 4].copy_from_slice(&value.to_le_bytes());
    }

    /// One entry veneer at offset 0 whose runtime target is image `target`.
    fn veneered(size: usize, target: usize, halves: &[(usize, u16)]) -> Vec<u8> {
        let mut image = image(size, halves);
        image[0..4].copy_from_slice(&[0x00, 0x4c, 0x20, 0x47]);
        word(&mut image, 4, 0x0200_8001 + target as u32);
        image
    }

    /// A BL pair calling image offset `target`: stored overlays encode the
    /// destination relative to the image base, with the Thumb bias.
    fn bl(target: usize) -> [u16; 2] {
        let encoded = target as u32 - 2;
        [
            0xf000 | ((encoded >> 12) & 0x7ff) as u16,
            0xf800 | ((encoded >> 1) & 0x7ff) as u16,
        ]
    }

    fn covered(code: &OverlayCode, at: usize) -> Option<&'static str> {
        let address = BASE + at as i64;
        code.spans
            .iter()
            .find(|span| span.start <= address && address < span.end)
            .map(|span| span.kind)
    }

    fn prove(image: &[u8], veneers: usize, entries: &[usize]) -> OverlayCode {
        let entries = entries
            .iter()
            .map(|at| BASE + *at as i64)
            .collect::<Vec<_>>();
        overlay_code(image, BASE, veneers, &entries).unwrap()
    }

    #[test]
    fn decoder_rejects_every_encoding_the_approved_assembler_cannot_spell() {
        // Every standalone halfword but BL halves and ARMv5 BLX suffixes
        // (decoded in pairs) and the IT/hint space, whose IT state would make
        // objdump misprint its neighbours; all of that space is undefined.
        let context = |half: u16| {
            let mut bytes = vec![0u8; 0x10000];
            bytes[0x8000..0x8002].copy_from_slice(&half.to_le_bytes());
            bytes
        };
        let valid = |half: u16| {
            let bytes = context(half);
            let image = Image {
                bytes: &bytes,
                base: BASE,
            };
            decode(&image, BASE + 0x8000, None).is_ok()
        };
        assert!((0xbf00..=0xbfffu16).all(|half| !valid(half)));
        let halves = (0..0xe800u16)
            .filter(|half| half >> 8 != 0xbf)
            .collect::<Vec<_>>();
        let work = tempfile::tempdir().unwrap();
        let binary = work.path().join("all.bin");
        let bytes = halves
            .iter()
            .flat_map(|half| half.to_le_bytes())
            .collect::<Vec<_>>();
        std::fs::write(&binary, &bytes).unwrap();
        let dump = std::process::Command::new("arm-none-eabi-objdump")
            .args(["-D", "-b", "binary", "-marmv4t", "-Mforce-thumb"])
            .arg(format!("--adjust-vma=0x{BASE:x}"))
            .arg(&binary)
            .output()
            .unwrap();
        let row = regex::Regex::new(r"^\s*([0-9a-f]+):\t([0-9a-f]{4}) \s*(.*)$").unwrap();
        let target = regex::Regex::new(
            r"\b(b|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble)(\.n)?\s+0x([0-9a-f]+)\b",
        )
        .unwrap();
        let mut sets = BTreeSet::new();
        let mut lines = halves
            .iter()
            .map(|half| format!("\t.2byte 0x{half:04x}"))
            .collect::<Vec<_>>();
        let mut rejected = vec![true; halves.len()];
        for line in String::from_utf8_lossy(&dump.stdout).lines() {
            let Some(found) = row.captures(line) else {
                continue;
            };
            let index = (i64::from_str_radix(&found[1], 16).unwrap() - BASE) as usize / 2;
            // objdump comments with `;` or, in newer releases, `@`.
            let mnemonic = found[3].split([';', '@']).next().unwrap().trim();
            if mnemonic.is_empty() || mnemonic.starts_with('.') {
                continue;
            }
            rejected[index] = false;
            lines[index] = match target.captures(mnemonic) {
                Some(branch) => {
                    sets.insert(format!("\t.set sub_{0}, 0x{0}", &branch[3]));
                    let cut = mnemonic.rfind("0x").unwrap();
                    format!("\t{}sub_{}", &mnemonic[..cut], &branch[3])
                }
                None => format!("\t{mnemonic}"),
            };
        }
        let head = [".syntax unified".to_string(), "\t.thumb".to_string()]
            .into_iter()
            .chain(sets)
            .collect::<Vec<_>>();
        let source = work.path().join("all.s");
        let text = |lines: &[String]| format!("{}\n{}\n", head.join("\n"), lines.join("\n"));
        std::fs::write(&source, text(&lines)).unwrap();
        let assembled = std::process::Command::new("arm-none-eabi-as")
            .args(["-mcpu=arm7tdmi", "-mthumb-interwork", "-o"])
            .arg(work.path().join("all.o"))
            .arg(&source)
            .output()
            .unwrap();
        let error = regex::Regex::new(r":(\d+): Error:").unwrap();
        for found in error.captures_iter(&String::from_utf8_lossy(&assembled.stderr)) {
            let index = found[1].parse::<usize>().unwrap() - head.len() - 1;
            rejected[index] = true;
            lines[index] = format!("\t.2byte 0x{:04x}", halves[index]);
        }
        let spelled = crate::overlay::compile::assemble_overlay_raw(
            &crate::overlay::source::OverlaySource::text(text(&lines)),
            BASE,
        )
        .unwrap();
        let mut disagreements = Vec::new();
        for (index, &half) in halves.iter().enumerate() {
            let round_trip = spelled[2 * index..2 * index + 2] == half.to_le_bytes();
            // The assembler spells UDF and ARMv8 HLT even for the ARM7TDMI,
            // but ARMv4T leaves both undefined.
            let undefined = half >> 8 == 0xde || half & 0xffc0 == 0xba80;
            // Every accepted encoding is an instruction the assembler spells
            // (perhaps preferring another encoding, as for add/subtract of a
            // three-bit immediate into the same register). Every rejected one
            // is refused, normalized away (unpredictable high-register and BX
            // forms), or architecturally undefined.
            let consistent = if valid(half) {
                !rejected[index]
            } else {
                rejected[index] || !round_trip || undefined
            };
            if !consistent {
                disagreements.push(format!("{half:04x}:{}", lines[index].trim()));
            }
        }
        assert!(disagreements.is_empty(), "{}", disagreements.join(" "));
    }

    #[test]
    fn calls_through_registers_return_and_far_jumps_do_not() {
        let mut bytes = image(0x100, &[(0x40, 0x469e), (0x42, 0xf800), (0x46, 0x4720)]);
        let image = Image {
            bytes: &bytes,
            base: BASE,
        };
        // mov lr, r3; bl-suffix #0 calls r3; a suffix alone is not code.
        let suffix = decode(&image, BASE + 0x42, Some(0x469e)).unwrap();
        assert_eq!(suffix.flow, Flow::IndirectCall);
        assert!(decode(&image, BASE + 0x42, Some(0x2001)).is_err());
        assert!(decode(&image, BASE + 0x42, None).is_err());
        // mov ip, pc; bx r4 links through ip. bx r4 alone, or after
        // mov r4, pc (no separate link), leaves.
        let bx = |previous| decode(&image, BASE + 0x46, previous).unwrap().flow;
        assert_eq!(bx(Some(0x46fc)), Flow::IndirectCall);
        assert_eq!(bx(None), Flow::Leave);
        assert_eq!(bx(Some(0x467c)), Flow::Leave);
        // A BL to a word boundary calls a function; to a halfword boundary
        // it is GCC's far jump; outside the image it is not code.
        for (at, [high, low]) in [(0x10, bl(0x80)), (0x20, bl(0x82)), (0x30, [0xf100, 0xf800])] {
            bytes[at..at + 2].copy_from_slice(&high.to_le_bytes());
            bytes[at + 2..at + 4].copy_from_slice(&low.to_le_bytes());
        }
        let image = Image {
            bytes: &bytes,
            base: BASE,
        };
        let flow = |at| decode(&image, BASE + at, None).map(|instruction| instruction.flow);
        assert_eq!(flow(0x10), Ok(Flow::Call(BASE + 0x80)));
        assert_eq!(flow(0x20), Ok(Flow::Jump(BASE + 0x82)));
        assert!(flow(0x30).is_err());
    }

    /// Halves at consecutive offsets from `at`.
    fn run(at: usize, halves: &[u16]) -> Vec<(usize, u16)> {
        halves
            .iter()
            .enumerate()
            .map(|(index, half)| (at + index * 2, *half))
            .collect()
    }

    #[test]
    fn a_bl_to_a_word_aligned_label_between_the_callers_instructions_jumps() {
        // (0x08) push {lr}; cmp r0, #0; bne 0x12; bl 0x18 (b<inverse> .+6;
        // far jump); (0x12) bl 0x20; b 0x1c; (0x18) movs r0, #3;
        // movs r1, #4; (0x1c) pop {pc}; pad; (0x20) a function: bx lr.
        let [far, jump] = bl(0x18);
        let [high, low] = bl(0x20);
        let halves = run(
            0x08,
            &[
                0xb500, 0x2800, 0xd101, far, jump, high, low, 0xe001, 0x2003, 0x2104, 0xbd00,
                0x0000, 0x4770,
            ],
        );
        let code = prove(&veneered(0x24, 0x08, &halves), 1, &[]);
        assert_eq!(code.far_jumps, BTreeMap::from([(BASE + 0x0e, BASE + 0x18)]));
        assert!((0x08..0x1e)
            .step_by(2)
            .all(|at| covered(&code, at) == Some("thumb")));
        // The label is part of its caller, not a called function; the BL
        // past the caller's last instruction still calls one.
        assert_eq!(
            code.functions,
            BTreeMap::from([
                (BASE + 0x08, Evidence::EntryVeneer),
                (BASE + 0x20, Evidence::Call)
            ])
        );
    }

    #[test]
    fn a_bl_to_a_word_aligned_label_the_caller_falls_into_jumps() {
        // (0x08) push {lr}; cmp r0, #0; beq 0x18; ldr r0, [pc, #4];
        // bl 0x1c (far jump); (0x14) pool; (0x18) bl 0x08 (recursion);
        // (0x1c) adds r0, r0, r1; pop {pc}. The recursive call returns
        // into the label, and a BL to the caller's own entry stays a call.
        let [far, jump] = bl(0x1c);
        let [high, low] = bl(0x08);
        let mut image = veneered(
            0x20,
            0x08,
            &[
                run(0x08, &[0xb500, 0x2800, 0xd004, 0x4801, far, jump]),
                run(0x18, &[high, low, 0x1840, 0xbd00]),
            ]
            .concat(),
        );
        word(&mut image, 0x14, 0x1234_5678);
        let code = prove(&image, 1, &[]);
        assert!(code.failed_runs.is_empty());
        assert_eq!(code.far_jumps, BTreeMap::from([(BASE + 0x10, BASE + 0x1c)]));
        assert_eq!(covered(&code, 0x14), Some("literal_pool"));
        assert!([0x10, 0x18, 0x1c, 0x1e]
            .iter()
            .all(|at| covered(&code, *at) == Some("thumb")));
        assert_eq!(
            code.functions,
            BTreeMap::from([(BASE + 0x08, Evidence::EntryVeneer)])
        );
    }

    #[test]
    fn a_bl_to_the_callers_final_epilogue_instruction_jumps() {
        // (0x08) push {lr}; cmp r0, #0; beq 0x18; ldr r0, [pc, #4];
        // bl 0x18 (far jump); (0x14) pool; (0x18) pop {pc}. The label is
        // the caller's last instruction, so it is not strictly between the
        // caller's first and last; the beq alone puts it in the caller's
        // walk, as at TLA resource_657 0x020014d0/0x020014e2 -> 0x02001d50.
        let [far, jump] = bl(0x18);
        let mut image = veneered(
            0x1c,
            0x08,
            &[
                run(0x08, &[0xb500, 0x2800, 0xd004, 0x4801, far, jump]),
                run(0x18, &[0xbd00]),
            ]
            .concat(),
        );
        word(&mut image, 0x14, 0x1234_5678);
        let code = prove(&image, 1, &[]);
        assert!(code.failed_runs.is_empty());
        assert_eq!(code.far_jumps, BTreeMap::from([(BASE + 0x10, BASE + 0x18)]));
        assert_eq!(covered(&code, 0x14), Some("literal_pool"));
        assert!((0x08..0x14)
            .step_by(2)
            .chain([0x18])
            .all(|at| covered(&code, at) == Some("thumb")));
        assert_eq!(
            code.functions,
            BTreeMap::from([(BASE + 0x08, Evidence::EntryVeneer)])
        );
    }

    #[test]
    fn a_bl_into_code_another_function_proved_stays_a_call() {
        // Veneer 0 proves (0x18) bx lr first. Veneer 1's function surrounds
        // it: (0x10) push {lr}; bl 0x18; b 0x1c; (0x1c) pop {pc}. The BL's
        // word-aligned target lies between the caller's first and last
        // instructions, but it is another function's proved code, so the BL
        // calls it and returns to the b.
        let [high, low] = bl(0x18);
        let mut bytes = image(
            0x20,
            &run(0x10, &[0xb500, high, low, 0xe001, 0x4770, 0x0000, 0xbd00]),
        );
        for (veneer, target) in [(0x00, 0x18u32), (0x08, 0x10)] {
            bytes[veneer..veneer + 4].copy_from_slice(&[0x00, 0x4c, 0x20, 0x47]);
            word(&mut bytes, veneer + 4, 0x0200_8001 + target);
        }
        let code = prove(&bytes, 2, &[]);
        assert!(code.failed_runs.is_empty());
        assert!(code.far_jumps.is_empty());
        assert!([0x10, 0x12, 0x16, 0x18, 0x1c]
            .iter()
            .all(|at| covered(&code, *at) == Some("thumb")));
        assert_eq!(
            code.functions,
            BTreeMap::from([
                (BASE + 0x10, Evidence::EntryVeneer),
                (BASE + 0x18, Evidence::EntryVeneer)
            ])
        );
    }

    #[test]
    fn owner_entries_on_alignment_or_inside_straight_line_code_are_dropped() {
        // (0x08) push {lr}; bl 0x18; bl 0x1c; movs r0, #0; pop {pc};
        // (0x16) pad; (0x18) movs r0, #1; bx lr; (0x1c) push {r4, lr};
        // mov r4, r8; (0x20) push {r4}; movs r0, #2; pop {r4}; mov r8, r4;
        // pop {r4, pc}. Owners name the pad, the function after it and the
        // middle of the second prologue.
        let [first, first_low] = bl(0x18);
        let [second, second_low] = bl(0x1c);
        let halves = run(
            0x08,
            &[
                0xb500, first, first_low, second, second_low, 0x2000, 0xbd00, 0x0000, 0x2001,
                0x4770, 0xb510, 0x4644, 0xb410, 0x2002, 0xbc10, 0x46a0, 0xbd10,
            ],
        );
        let code = prove(&veneered(0x2c, 0x08, &halves), 1, &[0x16, 0x18, 0x20]);
        assert_eq!(
            code.rejected_owners,
            [
                (BASE + 0x16, "is the zero alignment halfword after a return"),
                (
                    BASE + 0x20,
                    "lies inside another function's straight-line code"
                ),
            ]
        );
        assert!(covered(&code, 0x16).is_none());
        assert_eq!(covered(&code, 0x20), Some("thumb"));
        // The pad's own walk ran into 0x18; judged again without it, the
        // entry there stands.
        assert_eq!(
            code.functions,
            BTreeMap::from([
                (BASE + 0x08, Evidence::EntryVeneer),
                (BASE + 0x18, Evidence::Owner),
                (BASE + 0x1c, Evidence::Call)
            ])
        );
    }

    #[test]
    fn a_lone_bx_lr_in_a_gap_is_code_only_when_something_names_it() {
        // (0x08) push {lr}; bl 0x14; pop {pc}; (0x10) bx lr; pad;
        // (0x14) movs r0, #1; bx lr; (0x18) a data word.
        let [high, low] = bl(0x14);
        let halves = run(
            0x08,
            &[0xb500, high, low, 0xbd00, 0x4770, 0x0000, 0x2001, 0x4770],
        );
        let with = |data: [u16; 2]| {
            let mut image = veneered(0x20, 0x08, &halves);
            image[0x18..0x1a].copy_from_slice(&data[0].to_le_bytes());
            image[0x1a..0x1c].copy_from_slice(&data[1].to_le_bytes());
            prove(&image, 1, &[])
        };
        let unnamed = with([0, 0]);
        assert!(covered(&unnamed, 0x10).is_none());
        assert!(unnamed.rejected.iter().any(|rejection| {
            rejection.seed == BASE + 0x10
                && rejection.evidence == Evidence::Contiguous
                && rejection.failure.reason == "is a lone bx lr nothing names"
        }));
        // A Thumb pointer word or a BL anywhere names it.
        let pointed = with([0x8011, 0x0200]);
        assert_eq!(covered(&pointed, 0x10), Some("thumb"));
        assert!(pointed
            .accepted
            .contains(&(BASE + 0x10, Evidence::StoredPointer)));
        let called = with(bl(0x10));
        assert_eq!(covered(&called, 0x10), Some("thumb"));
        assert!(called
            .accepted
            .contains(&(BASE + 0x10, Evidence::Contiguous)));
    }

    #[test]
    fn a_gap_function_a_stored_pointer_names_is_recorded_as_named() {
        // The unreferenced setter below, named by a table word at 0x20.
        let [high, low] = bl(0x1c);
        let halves = run(
            0x08,
            &[0xb500, high, low, 0xbd00, 0x4b01, 0x8018, 0x4770, 0x0000],
        );
        let mut image = veneered(0x24, 0x08, &[halves.as_slice(), &[(0x1c, 0x4770)]].concat());
        word(&mut image, 0x18, 0x0200_1234);
        word(&mut image, 0x20, 0x0200_8011);
        let code = prove(&image, 1, &[]);
        assert!((0x10..0x16)
            .step_by(2)
            .all(|at| covered(&code, at) == Some("thumb")));
        assert!(code
            .accepted
            .contains(&(BASE + 0x10, Evidence::StoredPointer)));
        assert_eq!(
            code.functions.get(&(BASE + 0x10)),
            Some(&Evidence::StoredPointer)
        );
    }

    /// A switch whose table at 0x40 names arms at 0x50 and 0x54 (`bx lr`),
    /// dispatched by `tail` from the entry at 0x20 through the pool word at
    /// `pool`.
    fn switch(tail: &[(usize, u16)], pool: usize) -> Vec<u8> {
        let mut halves = tail.to_vec();
        halves.extend([(0x50, 0x4770), (0x54, 0x4770)]);
        let mut image = veneered(0x60, 0x20, &halves);
        word(&mut image, pool, 0x0200_8040);
        word(&mut image, 0x40, 0x0200_8051);
        word(&mut image, 0x44, 0x0200_8055);
        image
    }

    fn dispatched(code: &OverlayCode) -> bool {
        covered(code, 0x50) == Some("thumb")
            && covered(code, 0x54) == Some("thumb")
            && code.spans.iter().any(|span| {
                span.kind == "jump_table" && span.start == BASE + 0x40 && span.end == BASE + 0x48
            })
    }

    #[test]
    fn switch_dispatch_follows_the_gcc_register_chain_however_scheduled() {
        // cmp r3, #1; ldr r2, [pc, #12]; lsl r3, r3, #2; ldr r3, [r3, r2]; mov pc, r3
        let chain = [
            (0x20, 0x2b01),
            (0x22, 0x4a03),
            (0x24, 0x009b),
            (0x26, 0x589b),
            (0x28, 0x469f),
        ];
        let code = prove(&switch(&chain, 0x30), 1, &[]);
        assert!(dispatched(&code) && code.unresolved_jumps.is_empty());
        // The index scaled into r1 is not what the indexed load reads.
        let mut broken = chain;
        broken[2].1 = 0x0089;
        let code = prove(&switch(&broken, 0x30), 1, &[]);
        assert!(covered(&code, 0x50).is_none());
        assert_eq!(code.unresolved_jumps, [BASE + 0x28]);
        // The compare reads the unscaled source (cmp r0; lsl r3, r0, #2).
        let mut source = chain;
        source[0].1 = 0x2801;
        source[2].1 = 0x0083;
        assert!(dispatched(&prove(&switch(&source, 0x30), 1, &[])));
        // Scaled before the table load, the arm copied to ip:
        // cmp r3, #1; bhi 0x2e; lsl r3, r3, #2; ldr r2, [pc, #8];
        // ldr r3, [r3, r2]; mov ip, r3; mov pc, ip; (0x2e) bx lr
        let scheduled = [
            (0x20, 0x2b01),
            (0x22, 0xd804),
            (0x24, 0x009b),
            (0x26, 0x4a02),
            (0x28, 0x589b),
            (0x2a, 0x469c),
            (0x2c, 0x46e7),
            (0x2e, 0x4770),
        ];
        assert!(dispatched(&prove(&switch(&scheduled, 0x30), 1, &[])));
        // A register bound (movs r2, #2; cmp r3, r2; bcc 0x2c) with a far
        // jump on the other path, and a branch to the dispatch:
        // (0x26) bl 0x3a; (0x2c) ldr r2, [pc, #12]; lsl r3, r3, #2;
        // ldr r3, [r3, r2]; b 0x38; (0x38) mov pc, r3; (0x3a) bx lr
        let [far, jump] = bl(0x3a);
        let shared = [
            (0x20, 0x2202),
            (0x22, 0x4293),
            (0x24, 0xd302),
            (0x26, far),
            (0x28, jump),
            (0x2c, 0x4a03),
            (0x2e, 0x009b),
            (0x30, 0x589b),
            (0x32, 0xe001),
            (0x38, 0x469f),
            (0x3a, 0x4770),
        ];
        let code = prove(&switch(&shared, 0x3c), 1, &[]);
        assert!(dispatched(&code) && code.unresolved_jumps.is_empty());
        assert!(covered(&code, 0x2a).is_none(), "a far jump does not return");
    }

    /// One entry veneer to a `bx lr` at 0x08, then a table word at 0x10 and
    /// a function at 0x40 that no call or veneer reaches.
    fn pointed_image(pointer: u32, function: &[u16]) -> Vec<u8> {
        let halves = function
            .iter()
            .enumerate()
            .map(|(index, half)| (0x40 + index * 2, *half))
            .chain([(0x08, 0x4770)])
            .collect::<Vec<_>>();
        let mut image = veneered(0x60, 0x08, &halves);
        word(&mut image, 0x10, pointer);
        image
    }

    #[test]
    fn framed_function_pointer_reaches_a_callback_and_its_pool() {
        // push {r5, lr}; ldr r0, [pc, #8]; pop {r5}; pop {r0}; bx r0; pool 0x4c
        let function = [
            0xb520, 0x4802, 0xbc20, 0xbc01, 0x4700, 0x0000, 0x1234, 0x5678,
        ];
        let code = prove(&pointed_image(0x0200_8041, &function), 1, &[]);
        assert_eq!(covered(&code, 0x40), Some("thumb"));
        assert_eq!(covered(&code, 0x48), Some("thumb"));
        assert_eq!(covered(&code, 0x4c), Some("literal_pool"));
        // The word itself is data: a pointer names code, it is not code.
        assert!(covered(&code, 0x10).is_none());
        assert_eq!(
            code.functions.get(&(BASE + 0x40)),
            Some(&Evidence::FramedPointer)
        );
    }

    #[test]
    fn stored_pointers_name_only_word_aligned_framed_functions() {
        // An odd pointer to a halfword boundary, one to an unframed leaf and
        // an even word name nothing: unframed leaves need other evidence.
        let mut unaligned = pointed_image(0x0200_8043, &[0]);
        unaligned[0x42..0x44].copy_from_slice(&0xb520u16.to_le_bytes());
        let unframed = pointed_image(0x0200_8041, &[0x2000, 0x4770]);
        let even = pointed_image(0x0200_8040, &[0x2000, 0x4770]);
        for image in [unaligned, unframed, even] {
            let code = prove(&image, 1, &[]);
            assert!(covered(&code, 0x40).is_none() && covered(&code, 0x42).is_none());
        }
    }

    #[test]
    fn owner_entries_reach_unframed_leaves_the_register_names() {
        let image = pointed_image(0, &[0x2000, 0x4770]);
        assert!(covered(&prove(&image, 1, &[]), 0x40).is_none());
        let code = prove(&image, 1, &[0x40]);
        assert!(covered(&code, 0x40).is_some() && covered(&code, 0x42).is_some());
        assert!(overlay_code(&image, BASE, 1, &[BASE + 0x60]).is_err());
    }

    #[test]
    fn a_prologue_candidate_is_kept_or_discarded_whole() {
        // 0x10: a record whose pointer's low half reads as push {..., lr},
        // running into a lone BL suffix; 0x20: a framed function no pointer
        // names: push {lr}; movs r0, #1; pop {pc}.
        let record = [
            0xb5dd, 0x0200, 0x9585, 0x1000, 0x0000, 0xffff, 0xd949, 0x0200,
        ];
        let halves = record
            .iter()
            .enumerate()
            .map(|(index, half)| (0x10 + index * 2, *half))
            .chain([
                (0x08, 0x4770),
                (0x20, 0xb500),
                (0x22, 0x2001),
                (0x24, 0xbd00),
            ])
            .collect::<Vec<_>>();
        let code = prove(&veneered(0x40, 0x08, &halves), 1, &[]);
        assert!((0x10..0x20)
            .step_by(2)
            .all(|at| covered(&code, at).is_none()));
        assert!(code.rejected.iter().any(|rejection| {
            rejection.seed == BASE + 0x10
                && rejection.evidence == Evidence::Prologue
                && rejection.failure.at == BASE + 0x1a
        }));
        assert!((0x20..0x26)
            .step_by(2)
            .all(|at| covered(&code, at) == Some("thumb")));
        assert!(code.accepted.contains(&(BASE + 0x20, Evidence::Prologue)));
        // A push-shaped halfword off a word boundary is never a candidate.
        let unaligned = veneered(
            0x40,
            0x08,
            &[
                (0x08, 0x4770),
                (0x22, 0xb500),
                (0x24, 0x2001),
                (0x26, 0xbd00),
            ],
        );
        assert!(covered(&prove(&unaligned, 1, &[]), 0x22).is_none());
    }

    #[test]
    fn a_call_that_does_not_return_ends_at_its_literal_pool() {
        // push {lr}; ldr r0, [pc, #4]; bl 0x18; (0x10) pool; (0x18) bx lr
        let [high, low] = bl(0x18);
        let mut image = veneered(
            0x20,
            0x08,
            &[
                (0x08, 0xb500),
                (0x0a, 0x4801),
                (0x0c, high),
                (0x0e, low),
                (0x18, 0x4770),
            ],
        );
        word(&mut image, 0x10, 0x1234_5678);
        let code = prove(&image, 1, &[]);
        assert!(code.failed_runs.is_empty());
        assert_eq!(covered(&code, 0x0c), Some("thumb"));
        assert_eq!(covered(&code, 0x10), Some("literal_pool"));
        assert_eq!(covered(&code, 0x18), Some("thumb"));
    }

    #[test]
    fn a_register_call_returns_to_the_next_instruction() {
        // push {lr}; ldr r4, [pc, #8]; mov ip, pc; bx r4; movs r0, #1; pop {pc}
        let mut image = veneered(
            0x20,
            0x08,
            &[
                (0x08, 0xb500),
                (0x0a, 0x4c02),
                (0x0c, 0x46fc),
                (0x0e, 0x4720),
                (0x10, 0x2001),
                (0x12, 0xbd00),
            ],
        );
        word(&mut image, 0x14, 0x0300_0118);
        let code = prove(&image, 1, &[]);
        assert_eq!(covered(&code, 0x10), Some("thumb"));
        assert_eq!(covered(&code, 0x12), Some("thumb"));
        assert_eq!(covered(&code, 0x14), Some("literal_pool"));
    }

    #[test]
    fn a_gap_between_functions_is_code_only_when_complete_walks_explain_it() {
        // (0x08) push {lr}; bl 0x1c; pop {pc}; (0x10) an unreferenced setter:
        // ldr r3, [pc, #4]; strh r0, [r3]; bx lr; pad; pool; (0x1c) bx lr
        let [high, low] = bl(0x1c);
        let function = [
            (0x08, 0xb500),
            (0x0a, high),
            (0x0c, low),
            (0x0e, 0xbd00),
            (0x1c, 0x4770),
        ];
        let setter = [(0x10, 0x4b01), (0x12, 0x8018), (0x14, 0x4770)];
        let mut image = veneered(0x20, 0x08, &[function.as_slice(), &setter].concat());
        word(&mut image, 0x18, 0x0200_1234);
        let code = prove(&image, 1, &[]);
        assert_eq!(covered(&code, 0x10), Some("thumb"));
        assert_eq!(covered(&code, 0x14), Some("thumb"));
        assert_eq!(covered(&code, 0x18), Some("literal_pool"));
        assert!(code.accepted.contains(&(BASE + 0x10, Evidence::Contiguous)));
        // A table between the same functions never ends in control flow.
        let table = [(0x10, 0x0201), (0x14, 0x0202), (0x18, 0x0203)];
        let code = prove(
            &veneered(0x20, 0x08, &[function.as_slice(), &table].concat()),
            1,
            &[],
        );
        assert!((0x10..0x1c)
            .step_by(2)
            .all(|at| covered(&code, at).is_none()));
    }

    #[test]
    fn narrow_branch_target_recovers_an_instruction_after_a_literal_pool() {
        // ldr r0, [pc, #0]; b.n 0x08; .word 0xffffffff; cmp r0, #0; movs r0, #1; bx lr
        let image = image(
            14,
            &[
                (0, 0x4800),
                (2, 0xe001),
                (4, 0xffff),
                (6, 0xffff),
                (8, 0x2800),
                (10, 0x2001),
                (12, 0x4770),
            ],
        );
        let code = prove(&image, 0, &[0]);
        assert!((0..14).step_by(2).all(|at| covered(&code, at).is_some()));
    }

    #[test]
    fn fixed_veneer_can_enter_arm_iwram_code() {
        let spans = veneer_spans(&[0x00, 0x4c, 0x20, 0x47, 0x00, 0x00, 0x00, 0x03], BASE);
        assert_eq!(
            spans,
            [ExecutableSpan {
                start: BASE,
                end: BASE + 8,
                kind: "veneer"
            }]
        );
    }

    #[test]
    fn a_zero_halfword_between_executable_bytes_is_alignment() {
        let spans = [
            ExecutableSpan {
                start: BASE,
                end: BASE + 2,
                kind: "thumb",
            },
            ExecutableSpan {
                start: BASE + 4,
                end: BASE + 8,
                kind: "literal_pool",
            },
        ];
        assert_eq!(
            alignment_spans(&[0x70, 0x47, 0, 0, 1, 2, 3, 4], BASE, &spans).len(),
            1
        );
        assert!(alignment_spans(&[0x70, 0x47, 1, 0, 1, 2, 3, 4], BASE, &spans).is_empty());
    }
}
