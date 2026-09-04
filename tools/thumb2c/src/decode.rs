//! Thumb-1 decoding of an overlay window into the instruction forms the
//! lifter reasons about. Pool words are read from the window directly, so a
//! `ldr rN, [pc, #k]` carries its constant.

pub const OVERLAY_BASE: u32 = 0x0200_0000;
/// The main image lives here; decoding rebases through `decode_window_at`.
pub const MAIN_BASE: u32 = 0x0800_0000;

thread_local! {
    /// The address of byte zero of the image being decoded.
    static BASE: std::cell::Cell<u32> = const { std::cell::Cell::new(OVERLAY_BASE) };
}

/// The address of byte zero of the image being decoded on this thread.
pub fn base() -> u32 {
    BASE.with(|b| b.get())
}

/// Decodes a window of an image whose byte zero sits at `base`.
pub fn decode_window_at(image: &[u8], base: u32, entry: u32, span: u32) -> Vec<Ins> {
    let previous = BASE.with(|b| b.replace(base));
    let ins = decode_window(image, entry, span);
    BASE.with(|b| b.set(previous));
    ins
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Cond {
    Eq,
    Ne,
    Cs,
    Cc,
    Mi,
    Pl,
    Vs,
    Vc,
    Hi,
    Ls,
    Ge,
    Lt,
    Gt,
    Le,
}

impl Cond {
    fn from_bits(bits: u16) -> Option<Self> {
        Some(match bits {
            0 => Cond::Eq,
            1 => Cond::Ne,
            2 => Cond::Cs,
            3 => Cond::Cc,
            4 => Cond::Mi,
            5 => Cond::Pl,
            6 => Cond::Vs,
            7 => Cond::Vc,
            8 => Cond::Hi,
            9 => Cond::Ls,
            10 => Cond::Ge,
            11 => Cond::Lt,
            12 => Cond::Gt,
            13 => Cond::Le,
            _ => return None,
        })
    }

    pub fn mnemonic(self) -> &'static str {
        match self {
            Cond::Eq => "beq",
            Cond::Ne => "bne",
            Cond::Cs => "bcs",
            Cond::Cc => "bcc",
            Cond::Mi => "bmi",
            Cond::Pl => "bpl",
            Cond::Vs => "bvs",
            Cond::Vc => "bvc",
            Cond::Hi => "bhi",
            Cond::Ls => "bls",
            Cond::Ge => "bge",
            Cond::Lt => "blt",
            Cond::Gt => "bgt",
            Cond::Le => "ble",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Width {
    Word,
    Byte,
    Half,
    SignedHalf,
    SignedByte,
}

impl Width {
    pub fn c_type(self) -> &'static str {
        match self {
            Width::Word => "s32",
            Width::Byte => "u8",
            Width::Half => "u16",
            Width::SignedHalf => "s16",
            Width::SignedByte => "s8",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Shift {
    Lsl,
    Lsr,
    Asr,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Alu {
    And,
    Eor,
    Lsl,
    Lsr,
    Asr,
    Adc,
    Sbc,
    Ror,
    Tst,
    Neg,
    Cmp,
    Cmn,
    Orr,
    Mul,
    Bic,
    Mvn,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Offset {
    Imm(u32),
    Reg(u8),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Kind {
    Push {
        lr: bool,
    },
    Pop {
        pc: bool,
        /// The low registers popped, as a bit per register.
        list: u8,
    },
    Bx(u8),
    Nop,
    MovImm {
        rd: u8,
        imm: u32,
    },
    /// `mov rd, rm` through the high-register form.
    MovHi {
        rd: u8,
        rm: u8,
    },
    /// `movs rd, rm`, which the assembler spells as `lsls rd, rm, #0`.
    Movs {
        rd: u8,
        rm: u8,
    },
    AddImm3 {
        rd: u8,
        rn: u8,
        imm: u32,
    },
    AddImm8 {
        rd: u8,
        imm: u32,
    },
    AddReg {
        rd: u8,
        rn: u8,
        rm: u8,
    },
    AddHi {
        rd: u8,
        rm: u8,
    },
    AddSp {
        rd: u8,
        imm: u32,
    },
    AddPc {
        rd: u8,
        imm: u32,
    },
    SubImm3 {
        rd: u8,
        rn: u8,
        imm: u32,
    },
    SubImm8 {
        rd: u8,
        imm: u32,
    },
    SubReg {
        rd: u8,
        rn: u8,
        rm: u8,
    },
    SpAdjust(i32),
    ShiftImm {
        shift: Shift,
        rd: u8,
        rm: u8,
        imm: u32,
    },
    Alu {
        op: Alu,
        rd: u8,
        rm: u8,
    },
    CmpImm {
        rn: u8,
        imm: u32,
    },
    CmpReg {
        rn: u8,
        rm: u8,
    },
    LdrPool {
        rd: u8,
        word: u32,
    },
    LdrSp {
        rd: u8,
        imm: u32,
    },
    StrSp {
        rd: u8,
        imm: u32,
    },
    Load {
        width: Width,
        rd: u8,
        rn: u8,
        offset: Offset,
    },
    Store {
        width: Width,
        rd: u8,
        rn: u8,
        offset: Offset,
    },
    Ldmia {
        rn: u8,
        list: u8,
    },
    Stmia {
        rn: u8,
        list: u8,
    },
    Bcond {
        cond: Cond,
        target: u32,
    },
    B {
        target: u32,
    },
    Bl {
        target: u32,
    },
    Unknown(u16),
}

#[derive(Debug, Clone)]
pub struct Ins {
    pub addr: u32,
    pub size: u32,
    pub kind: Kind,
    pub text: String,
}

pub fn reg(n: u8) -> String {
    match n {
        13 => "sp".to_string(),
        14 => "lr".to_string(),
        15 => "pc".to_string(),
        n => format!("r{n}"),
    }
}

fn regs_of(list: u8) -> String {
    (0..8)
        .filter(|bit| list & (1 << bit) != 0)
        .map(|bit| format!("r{bit}"))
        .collect::<Vec<_>>()
        .join(", ")
}

fn sign_extend(value: u32, bits: u32) -> i32 {
    ((value << (32 - bits)) as i32) >> (32 - bits)
}

fn offset_text(rn: u8, offset: Offset) -> String {
    match offset {
        Offset::Imm(0) => format!("[{}]", reg(rn)),
        Offset::Imm(k) => format!("[{}, #{k}]", reg(rn)),
        Offset::Reg(rm) => format!("[{}, {}]", reg(rn), reg(rm)),
    }
}

fn mem_mnemonic(width: Width, store: bool) -> &'static str {
    match (width, store) {
        (Width::Word, false) => "ldr",
        (Width::Byte, false) => "ldrb",
        (Width::Half, false) => "ldrh",
        (Width::SignedHalf, false) => "ldrsh",
        (Width::SignedByte, false) => "ldrsb",
        (Width::Word, true) => "str",
        (Width::Byte, true) => "strb",
        (Width::Half, true) => "strh",
        _ => "str?",
    }
}

fn alu_mnemonic(op: Alu) -> &'static str {
    match op {
        Alu::And => "ands",
        Alu::Eor => "eors",
        Alu::Lsl => "lsls",
        Alu::Lsr => "lsrs",
        Alu::Asr => "asrs",
        Alu::Adc => "adcs",
        Alu::Sbc => "sbcs",
        Alu::Ror => "rors",
        Alu::Tst => "tst",
        Alu::Neg => "negs",
        Alu::Cmp => "cmp",
        Alu::Cmn => "cmn",
        Alu::Orr => "orrs",
        Alu::Mul => "muls",
        Alu::Bic => "bics",
        Alu::Mvn => "mvns",
    }
}

pub fn text_of(kind: &Kind) -> String {
    match kind {
        Kind::Push { lr } => format!("push {{{}}}", if *lr { "lr" } else { "" }),
        Kind::Pop { pc, .. } => format!("pop {{{}}}", if *pc { "pc" } else { "" }),
        Kind::Bx(rm) => format!("bx {}", reg(*rm)),
        Kind::Nop => "nop".to_string(),
        Kind::MovImm { rd, imm } => format!("movs r{rd}, #{imm}"),
        Kind::MovHi { rd, rm } => format!("mov {}, {}", reg(*rd), reg(*rm)),
        Kind::Movs { rd, rm } => format!("movs r{rd}, r{rm}"),
        Kind::AddImm3 { rd, rn, imm } => format!("adds r{rd}, r{rn}, #{imm}"),
        Kind::AddImm8 { rd, imm } => format!("adds r{rd}, #{imm}"),
        Kind::AddReg { rd, rn, rm } => format!("adds r{rd}, r{rn}, r{rm}"),
        Kind::AddHi { rd, rm } => format!("add {}, {}", reg(*rd), reg(*rm)),
        Kind::AddSp { rd, imm } => format!("add r{rd}, sp, #{imm}"),
        Kind::AddPc { rd, imm } => format!("add r{rd}, pc, #{imm}"),
        Kind::SubImm3 { rd, rn, imm } => format!("subs r{rd}, r{rn}, #{imm}"),
        Kind::SubImm8 { rd, imm } => format!("subs r{rd}, #{imm}"),
        Kind::SubReg { rd, rn, rm } => format!("subs r{rd}, r{rn}, r{rm}"),
        Kind::SpAdjust(k) => format!("sub sp, #{}", -k),
        Kind::ShiftImm { shift, rd, rm, imm } => format!(
            "{} r{rd}, r{rm}, #{imm}",
            match shift {
                Shift::Lsl => "lsls",
                Shift::Lsr => "lsrs",
                Shift::Asr => "asrs",
            }
        ),
        Kind::Alu { op, rd, rm } => format!("{} r{rd}, r{rm}", alu_mnemonic(*op)),
        Kind::CmpImm { rn, imm } => format!("cmp r{rn}, #{imm}"),
        Kind::CmpReg { rn, rm } => format!("cmp {}, {}", reg(*rn), reg(*rm)),
        Kind::LdrPool { rd, word } => format!("ldr r{rd}, =0x{word:x}"),
        Kind::LdrSp { rd, imm } => format!("ldr r{rd}, [sp, #{imm}]"),
        Kind::StrSp { rd, imm } => format!("str r{rd}, [sp, #{imm}]"),
        Kind::Load {
            width,
            rd,
            rn,
            offset,
        } => format!(
            "{} r{rd}, {}",
            mem_mnemonic(*width, false),
            offset_text(*rn, *offset)
        ),
        Kind::Store {
            width,
            rd,
            rn,
            offset,
        } => format!(
            "{} r{rd}, {}",
            mem_mnemonic(*width, true),
            offset_text(*rn, *offset)
        ),
        Kind::Ldmia { rn, list } => format!("ldmia r{rn}!, {{{}}}", regs_of(*list)),
        Kind::Stmia { rn, list } => format!("stmia r{rn}!, {{{}}}", regs_of(*list)),
        Kind::Bcond { cond, target } => format!("{} 0x{target:08x}", cond.mnemonic()),
        Kind::B { target } => format!("b 0x{target:08x}"),
        Kind::Bl { target } => format!("bl 0x{target:08x}"),
        Kind::Unknown(half) => format!(".2byte 0x{half:04x}"),
    }
}

/// Reads the little-endian halfword at `offset` of `bytes`.
pub fn half_at(bytes: &[u8], offset: usize) -> Option<u16> {
    Some(u16::from_le_bytes([
        *bytes.get(offset)?,
        *bytes.get(offset + 1)?,
    ]))
}

pub fn word_at(bytes: &[u8], offset: usize) -> Option<u32> {
    Some(u32::from_le_bytes([
        *bytes.get(offset)?,
        *bytes.get(offset + 1)?,
        *bytes.get(offset + 2)?,
        *bytes.get(offset + 3)?,
    ]))
}

/// Decodes one instruction at `pc`. `image` holds the whole overlay so pool
/// words and branch targets resolve against real addresses.
pub fn decode_one(image: &[u8], pc: u32) -> Option<Ins> {
    let offset = pc.checked_sub(base())? as usize;
    let half = half_at(image, offset)?;
    let rd = |shift: u32| ((half >> shift) & 7) as u8;
    let kind = match half >> 13 {
        0 => {
            let op = (half >> 11) & 3;
            if op == 3 {
                let imm_form = half & 0x0400 != 0;
                let sub = half & 0x0200 != 0;
                let value = ((half >> 6) & 7) as u32;
                match (imm_form, sub) {
                    (false, false) => Kind::AddReg {
                        rd: rd(0),
                        rn: rd(3),
                        rm: value as u8,
                    },
                    (false, true) => Kind::SubReg {
                        rd: rd(0),
                        rn: rd(3),
                        rm: value as u8,
                    },
                    (true, false) => Kind::AddImm3 {
                        rd: rd(0),
                        rn: rd(3),
                        imm: value,
                    },
                    (true, true) => Kind::SubImm3 {
                        rd: rd(0),
                        rn: rd(3),
                        imm: value,
                    },
                }
            } else {
                let imm = ((half >> 6) & 0x1f) as u32;
                let shift = [Shift::Lsl, Shift::Lsr, Shift::Asr][op as usize];
                if op == 0 && imm == 0 {
                    Kind::Movs {
                        rd: rd(0),
                        rm: rd(3),
                    }
                } else {
                    Kind::ShiftImm {
                        shift,
                        rd: rd(0),
                        rm: rd(3),
                        imm: if op != 0 && imm == 0 { 32 } else { imm },
                    }
                }
            }
        }
        1 => {
            let rdn = rd(8);
            let imm = (half & 0xff) as u32;
            match (half >> 11) & 3 {
                0 => Kind::MovImm { rd: rdn, imm },
                1 => Kind::CmpImm { rn: rdn, imm },
                2 => Kind::AddImm8 { rd: rdn, imm },
                _ => Kind::SubImm8 { rd: rdn, imm },
            }
        }
        2 => {
            if half & 0xfc00 == 0x4000 {
                let ops = [
                    Alu::And,
                    Alu::Eor,
                    Alu::Lsl,
                    Alu::Lsr,
                    Alu::Asr,
                    Alu::Adc,
                    Alu::Sbc,
                    Alu::Ror,
                    Alu::Tst,
                    Alu::Neg,
                    Alu::Cmp,
                    Alu::Cmn,
                    Alu::Orr,
                    Alu::Mul,
                    Alu::Bic,
                    Alu::Mvn,
                ];
                let op = ops[((half >> 6) & 0xf) as usize];
                if op == Alu::Cmp {
                    Kind::CmpReg {
                        rn: rd(0),
                        rm: rd(3),
                    }
                } else {
                    Kind::Alu {
                        op,
                        rd: rd(0),
                        rm: rd(3),
                    }
                }
            } else if half & 0xfc00 == 0x4400 {
                let rdn = rd(0) | (((half >> 7) & 1) as u8) << 3;
                let rm = ((half >> 3) & 0xf) as u8;
                match (half >> 8) & 3 {
                    0 => Kind::AddHi { rd: rdn, rm },
                    1 => Kind::CmpReg { rn: rdn, rm },
                    2 => Kind::MovHi { rd: rdn, rm },
                    _ => Kind::Bx(rm),
                }
            } else if half & 0xf800 == 0x4800 {
                let target = ((pc & !3) + 4 + ((half & 0xff) as u32) * 4) as usize;
                let word = word_at(image, target - base() as usize)?;
                Kind::LdrPool { rd: rd(8), word }
            } else {
                let rm = rd(6);
                let (width, store) = match (half >> 9) & 7 {
                    0 => (Width::Word, true),
                    1 => (Width::Half, true),
                    2 => (Width::Byte, true),
                    3 => (Width::SignedByte, false),
                    4 => (Width::Word, false),
                    5 => (Width::Half, false),
                    6 => (Width::Byte, false),
                    _ => (Width::SignedHalf, false),
                };
                let (rd, rn, offset) = (rd(0), rd(3), Offset::Reg(rm));
                if store {
                    Kind::Store {
                        width,
                        rd,
                        rn,
                        offset,
                    }
                } else {
                    Kind::Load {
                        width,
                        rd,
                        rn,
                        offset,
                    }
                }
            }
        }
        3 => {
            let imm = ((half >> 6) & 0x1f) as u32;
            let (width, scale) = if half & 0x1000 != 0 {
                (Width::Byte, 1)
            } else {
                (Width::Word, 4)
            };
            let store = half & 0x0800 == 0;
            let (rd, rn, offset) = (rd(0), rd(3), Offset::Imm(imm * scale));
            if store {
                Kind::Store {
                    width,
                    rd,
                    rn,
                    offset,
                }
            } else {
                Kind::Load {
                    width,
                    rd,
                    rn,
                    offset,
                }
            }
        }
        4 => {
            if half & 0xf000 == 0x8000 {
                let imm = (((half >> 6) & 0x1f) as u32) * 2;
                let (rd, rn, offset) = (rd(0), rd(3), Offset::Imm(imm));
                if half & 0x0800 == 0 {
                    Kind::Store {
                        width: Width::Half,
                        rd,
                        rn,
                        offset,
                    }
                } else {
                    Kind::Load {
                        width: Width::Half,
                        rd,
                        rn,
                        offset,
                    }
                }
            } else {
                let imm = ((half & 0xff) as u32) * 4;
                if half & 0x0800 == 0 {
                    Kind::StrSp { rd: rd(8), imm }
                } else {
                    Kind::LdrSp { rd: rd(8), imm }
                }
            }
        }
        5 => {
            if half & 0xf000 == 0xa000 {
                let imm = ((half & 0xff) as u32) * 4;
                if half & 0x0800 == 0 {
                    Kind::AddPc { rd: rd(8), imm }
                } else {
                    Kind::AddSp { rd: rd(8), imm }
                }
            } else if half & 0xff00 == 0xb000 {
                let value = ((half & 0x7f) as i32) * 4;
                Kind::SpAdjust(if half & 0x80 != 0 { -value } else { value })
            } else if half & 0xfe00 == 0xb400 {
                Kind::Push {
                    lr: half & 0x100 != 0,
                }
            } else if half & 0xfe00 == 0xbc00 {
                Kind::Pop {
                    pc: half & 0x100 != 0,
                    list: (half & 0xff) as u8,
                }
            } else if half == 0xbf00 || half == 0x46c0 {
                Kind::Nop
            } else {
                Kind::Unknown(half)
            }
        }
        6 => {
            if half & 0xf000 == 0xc000 {
                let list = (half & 0xff) as u8;
                if half & 0x0800 == 0 {
                    Kind::Stmia { rn: rd(8), list }
                } else {
                    Kind::Ldmia { rn: rd(8), list }
                }
            } else {
                let code = (half >> 8) & 0xf;
                match Cond::from_bits(code) {
                    Some(cond) => Kind::Bcond {
                        cond,
                        target: (pc as i64 + 4 + (sign_extend((half & 0xff) as u32, 8) as i64) * 2)
                            as u32,
                    },
                    None => Kind::Unknown(half),
                }
            }
        }
        _ => {
            if half & 0xf800 == 0xe000 {
                Kind::B {
                    target: (pc as i64 + 4 + (sign_extend((half & 0x7ff) as u32, 11) as i64) * 2)
                        as u32,
                }
            } else if half & 0xf800 == 0xf000 {
                let low = half_at(image, offset + 2)?;
                if low & 0xf800 != 0xf800 {
                    return Some(Ins {
                        addr: pc,
                        size: 2,
                        kind: Kind::Unknown(half),
                        text: text_of(&Kind::Unknown(half)),
                    });
                }
                let displacement = sign_extend(
                    (((half & 0x7ff) as u32) << 12) | (((low & 0x7ff) as u32) << 1),
                    23,
                );
                let target = (pc as i64 + 4 + displacement as i64) as u32;
                let kind = Kind::Bl { target };
                return Some(Ins {
                    addr: pc,
                    size: 4,
                    text: text_of(&kind),
                    kind,
                });
            } else {
                Kind::Unknown(half)
            }
        }
    };
    Some(Ins {
        addr: pc,
        size: 2,
        text: text_of(&kind),
        kind,
    })
}

/// Decodes the code reachable from `entry` inside `[entry, entry + span)`.
/// Pool words and padding are skipped; the result is address ordered.
pub fn decode_window(image: &[u8], entry: u32, span: u32) -> Vec<Ins> {
    let end = entry + span;
    let inside = |address: u32| address >= entry && address < end;
    let mut queue = vec![entry];
    let mut seen = std::collections::BTreeMap::new();
    let mut head = 0;
    while head < queue.len() {
        let mut pc = queue[head];
        head += 1;
        while inside(pc) && !seen.contains_key(&pc) {
            let Some(ins) = decode_one(image, pc) else {
                break;
            };
            let size = ins.size;
            let mut stop = false;
            match ins.kind {
                Kind::B { target } => {
                    if inside(target) {
                        queue.push(target);
                    }
                    stop = true;
                }
                Kind::Bcond { target, .. } => {
                    if inside(target) {
                        queue.push(target);
                    }
                }
                Kind::Bx(_) | Kind::Pop { pc: true, .. } => stop = true,
                Kind::Unknown(_) => stop = true,
                _ => {}
            }
            seen.insert(pc, ins);
            if stop {
                break;
            }
            pc += size;
        }
    }
    // A function that follows a return without any branch reaching it still
    // belongs to the window: resume decoding at each `push {.., lr}` after a
    // return so multi-function modules decode completely.
    // Words the decoded loads read from the pool are data, never a prologue.
    let pool: std::collections::BTreeSet<u32> = seen
        .iter()
        .filter(|(_, x)| matches!(x.kind, Kind::LdrPool { .. }))
        .filter_map(|(pc, _)| {
            let half = half_at(image, (*pc - base()) as usize)?;
            Some((*pc & !3) + 4 + u32::from(half & 0xff) * 4)
        })
        .flat_map(|word| [word, word + 2])
        .collect();
    let mut cursor = entry;
    while cursor + 2 <= end {
        if seen.contains_key(&cursor) {
            cursor += seen[&cursor].size;
            continue;
        }
        if pool.contains(&cursor) {
            cursor += 2;
            continue;
        }
        let offset = (cursor - base()) as usize;
        let half = half_at(image, offset).unwrap_or(0);
        let after_return = seen
            .range(..cursor)
            .next_back()
            .is_some_and(|(_, x)| matches!(x.kind, Kind::Bx(_) | Kind::Pop { pc: true, .. }));
        if half & 0xff00 == 0xb500 && after_return {
            let extra = decode_window(image, cursor, end - cursor);
            // Table words can look like a prologue: only a run that decodes
            // cleanly and returns is code.
            let clean = extra.iter().all(|x| !matches!(x.kind, Kind::Unknown(_)))
                && extra
                    .iter()
                    .any(|x| matches!(x.kind, Kind::Bx(_) | Kind::Pop { pc: true, .. }));
            if clean {
                for ins in extra {
                    seen.entry(ins.addr).or_insert(ins);
                }
                continue;
            }
            break;
        }
        cursor += 2;
    }
    seen.into_values().collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn image(halves: &[u16]) -> Vec<u8> {
        halves.iter().flat_map(|half| half.to_le_bytes()).collect()
    }

    #[test]
    fn decodes_common_forms() {
        let image = image(&[0xb510, 0x2001, 0x4800, 0x4700, 0x1234, 0x5678]);
        let ins = decode_window(&image, OVERLAY_BASE, 12);
        let kinds: Vec<Kind> = ins.into_iter().map(|ins| ins.kind).collect();
        assert_eq!(
            kinds,
            vec![
                Kind::Push { lr: true },
                Kind::MovImm { rd: 0, imm: 1 },
                Kind::LdrPool {
                    rd: 0,
                    word: 0x5678_1234
                },
                Kind::Bx(0),
            ]
        );
    }

    #[test]
    fn decodes_branches_and_memory() {
        let image = image(&[0xd001, 0xe7fd, 0xf000, 0xf802, 0x6813, 0x7053, 0x5c88]);
        let base = OVERLAY_BASE;
        assert_eq!(
            decode_one(&image, base).unwrap().kind,
            Kind::Bcond {
                cond: Cond::Eq,
                target: base + 6
            }
        );
        assert_eq!(
            decode_one(&image, base + 2).unwrap().kind,
            Kind::B { target: base }
        );
        assert_eq!(
            decode_one(&image, base + 4).unwrap().kind,
            Kind::Bl { target: base + 12 }
        );
        assert_eq!(
            decode_one(&image, base + 8).unwrap().kind,
            Kind::Load {
                width: Width::Word,
                rd: 3,
                rn: 2,
                offset: Offset::Imm(0)
            }
        );
        assert_eq!(
            decode_one(&image, base + 10).unwrap().kind,
            Kind::Store {
                width: Width::Byte,
                rd: 3,
                rn: 2,
                offset: Offset::Imm(1)
            }
        );
        assert_eq!(
            decode_one(&image, base + 12).unwrap().kind,
            Kind::Load {
                width: Width::Byte,
                rd: 0,
                rn: 1,
                offset: Offset::Reg(2)
            }
        );
    }
}
