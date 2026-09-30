//! Thumb instructions as GAS unified-syntax source that assembles back to
//! the same encoding, with branch targets and pool loads named by the
//! caller. Any encoding the assembler would spell differently stays a
//! `.2byte`.

use super::flow::Image;
use crate::decode::{Ins, Kind};

/// How a listing names what its instructions refer to.
pub trait Refer {
    /// The operand of a branch or call at `site` to `target`.
    fn branch(&self, site: u32, target: u32) -> String;
    /// The label of the pool word at `word`, when the listing defines one.
    fn pool(&self, word: u32) -> Option<String>;
}

pub fn halfword(value: u16) -> String {
    format!(".2byte 0x{value:04x}")
}

/// Whether the assembler would write this encoding back unchanged from its
/// mnemonic: not a high-register form with only low registers (UNPREDICTABLE
/// before ARMv6, where GAS picks another encoding), not an empty or
/// base-writeback-conflicting register list, not a `bx` with its low bits
/// set, and not the ARMv6T2 hint `nop`.
fn spellable(half: u16, kind: &Kind) -> bool {
    match *kind {
        Kind::MovHi { .. } | Kind::AddHi { .. } => half & 0x00c0 != 0,
        Kind::CmpReg { .. } => half & 0xfc00 == 0x4000 || half & 0x00c0 != 0,
        Kind::Bx(_) => half & 0x0087 == 0,
        Kind::Push { lr, list } | Kind::Pop { pc: lr, list } => lr || list != 0,
        Kind::Ldmia { rn, list } => list != 0 && list & (1 << rn) == 0,
        Kind::Stmia { rn, list } => {
            list != 0 && (list & (1 << rn) == 0 || list & ((1 << rn) - 1) == 0)
        }
        Kind::Nop => half == 0x46c0,
        Kind::Unknown(_) => false,
        _ => true,
    }
}

/// One instruction's source line (without indentation).
pub fn instruction(image: Image, ins: &Ins, refer: &dyn Refer) -> String {
    let half = image.half(ins.addr);
    if let Kind::Unknown(value) = ins.kind {
        if value & 0xff00 == 0xdf00 {
            return format!("swi #{}", value & 0xff);
        }
        return halfword(value);
    }
    if !spellable(half, &ins.kind) {
        return halfword(half);
    }
    match ins.kind {
        Kind::B { target } => format!("b {}", refer.branch(ins.addr, target)),
        Kind::Bcond { cond, target } => {
            format!("{} {}", cond.mnemonic(), refer.branch(ins.addr, target))
        }
        Kind::Bl { target } => format!("bl {}", refer.branch(ins.addr, target)),
        Kind::LdrPool { rd, .. } => {
            let word = super::flow::pool_address(ins.addr, half);
            match refer.pool(word) {
                Some(label) => format!("ldr r{rd}, {label}"),
                None => format!("ldr r{rd}, [pc, #{}]", u32::from(half & 0xff) * 4),
            }
        }
        Kind::SpAdjust(amount) if amount < 0 => format!("sub sp, #{}", -amount),
        Kind::SpAdjust(amount) => format!("add sp, #{amount}"),
        Kind::Nop => "nop".into(),
        _ => ins.text.clone(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::relist::flow::tests::halves;

    struct Plain;
    impl Refer for Plain {
        fn branch(&self, _: u32, target: u32) -> String {
            format!("Func_{target:08x}")
        }
        fn pool(&self, word: u32) -> Option<String> {
            (word == 0x0800_0008).then(|| format!(".L_{word:08x}"))
        }
    }

    fn text(values: &[u16]) -> String {
        let mut bytes = halves(values);
        bytes.resize(16, 0);
        let image = Image {
            bytes: &bytes,
            base: 0x0800_0000,
        };
        let ins = image.decode(0x0800_0000).unwrap();
        instruction(image, &ins, &Plain)
    }

    #[test]
    fn named_operands_replace_addresses() {
        assert_eq!(text(&[0xf000, 0xf802]), "bl Func_08000008");
        assert_eq!(text(&[0xd002]), "beq Func_08000008");
        assert_eq!(text(&[0x4801]), "ldr r0, .L_08000008");
        assert_eq!(text(&[0x4802]), "ldr r0, [pc, #8]");
    }

    #[test]
    fn stack_adjustments_and_software_interrupts_read_as_written() {
        assert_eq!(text(&[0xb082]), "sub sp, #8");
        assert_eq!(text(&[0xb002]), "add sp, #8");
        assert_eq!(text(&[0xdf0b]), "swi #11");
    }

    #[test]
    fn encodings_the_assembler_would_respell_stay_halfwords() {
        assert_eq!(text(&[0x4608]), ".2byte 0x4608"); // mov r0, r1 in the high form
        assert_eq!(text(&[0x4508]), ".2byte 0x4508"); // cmp r0, r1 in the high form
        assert_eq!(text(&[0x4701]), ".2byte 0x4701"); // bx r0 with a low bit set
        assert_eq!(text(&[0xbf00]), ".2byte 0xbf00"); // the ARMv6T2 nop hint
        assert_eq!(text(&[0xc903]), ".2byte 0xc903"); // ldmia r1!, {r0, r1}
        assert_eq!(text(&[0xf800]), ".2byte 0xf800"); // a lone bl suffix
        assert_eq!(text(&[0x46c0]), "mov r8, r8");
        assert_eq!(text(&[0x4288]), "cmp r0, r1");
        assert_eq!(text(&[0x4550]), "cmp r0, r10");
    }
}
