//! Aligns a gcc RTL insn sequence (`rtl-insn`) against a disassembled Thumb
//! instruction sequence (`thumb-disasm`) by CONTENT, not position.
//!
//! WHY this file exists (from `tools/lib/rtl_align.ts`, whose behaviour it
//! reproduces): a strict 1:1 zip breaks near prologues/epilogues, where one RTL
//! insn (`{*epilogue_insns}`) expands into several real instructions, or a
//! synthesized pattern collapses several RTL insns into one. This module
//! instead classifies each side into a shared, coarse instruction class
//! (alu/load/store/call/branch/other) plus, where meaningful, a destination
//! register, and finds the longest common compatible subsequence -- the same
//! idea as a text diff, tolerant of insertions on either side. Unmatched
//! entries on either side are reported, not hidden: an unmatched RTL insn or
//! instruction row is itself useful information (it is exactly where the
//! correlation broke down).
//!
//! PORT NOTE: the TypeScript returns `AlignedPair { insn, instruction }` --
//! object references into the caller's arrays. Rust returns INDICES into the
//! two input slices instead of cloning; `Alignment::pair_refs` recovers the
//! references. Same information, no lifetime tangle, no deep copies. The
//! unmatched lists are likewise indices, and are produced in ascending index
//! order exactly as `Array.prototype.filter` does.

use rtl_insn::{dest_register, RtlExpr, RtlInsn};
use thumb_disasm::{
    is_branch_mnemonic, is_call_mnemonic, is_load_mnemonic, is_store_mnemonic,
    primary_dest_register, Instruction,
};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InsnClass {
    Alu,
    Load,
    Store,
    Call,
    Branch,
    Other,
}

impl InsnClass {
    pub fn as_str(self) -> &'static str {
        match self {
            InsnClass::Alu => "alu",
            InsnClass::Load => "load",
            InsnClass::Store => "store",
            InsnClass::Call => "call",
            InsnClass::Branch => "branch",
            InsnClass::Other => "other",
        }
    }
}

pub fn classify_rtl(insn: &RtlInsn) -> InsnClass {
    if insn.kind == rtl_insn::InsnKind::CallInsn || insn.call_target.is_some() {
        return InsnClass::Call;
    }
    if insn.kind == rtl_insn::InsnKind::JumpInsn {
        return InsnClass::Branch;
    }
    // PORT NOTE: the TS reads `insn.set?.dest` and guards it against both
    // `undefined` (no `set`) and `null`. In the Rust `RtlInsn`, `set` is an
    // `Option<Set>` whose `dest` is always present, so the single `if let`
    // covers both TS guards.
    if let Some(set) = insn.set.as_ref() {
        match &set.dest {
            RtlExpr::Mem { .. } => return InsnClass::Store,
            RtlExpr::Reg { .. } | RtlExpr::Subreg { .. } => {
                let src_is_mem = matches!(&set.src, RtlExpr::Mem { .. });
                let unary_of_mem = match &set.src {
                    RtlExpr::Unary { a, .. } => matches!(a.as_ref(), RtlExpr::Mem { .. }),
                    _ => false,
                };
                return if src_is_mem || unary_of_mem {
                    InsnClass::Load
                } else {
                    InsnClass::Alu
                };
            }
            _ => {}
        }
    }
    InsnClass::Other
}

/// `/^(adds?|subs?|muls|ands?|orrs?|eors?|asrs?|lsls?|lsrs?|movs?|mvns?|negs?|cmps?|tst|bics?)$/`
///
/// PORT NOTE: the regex is anchored at both ends, so this is an exact-membership
/// test, not the prefix test the load/store predicates use. Note `muls` has no
/// `s?` in the original -- bare `mul` is deliberately NOT alu here, and that is
/// reproduced rather than "fixed".
fn is_alu_mnemonic(mnemonic: &str) -> bool {
    matches!(
        mnemonic,
        "add"
            | "adds"
            | "sub"
            | "subs"
            | "muls"
            | "and"
            | "ands"
            | "orr"
            | "orrs"
            | "eor"
            | "eors"
            | "asr"
            | "asrs"
            | "lsl"
            | "lsls"
            | "lsr"
            | "lsrs"
            | "mov"
            | "movs"
            | "mvn"
            | "mvns"
            | "neg"
            | "negs"
            | "cmp"
            | "cmps"
            | "tst"
            | "bic"
            | "bics"
    )
}

/// Order is load-bearing: `pop` is both a branch-adjacent epilogue row and a
/// load mnemonic, and the store test runs before the load test because
/// `strh`-style prefixes would otherwise never be reached.
pub fn classify_instruction(instruction: &Instruction) -> InsnClass {
    let mnemonic = instruction.mnemonic.as_str();
    if is_call_mnemonic(mnemonic) {
        return InsnClass::Call;
    }
    if is_store_mnemonic(mnemonic) {
        return InsnClass::Store;
    }
    if is_load_mnemonic(mnemonic) {
        return InsnClass::Load;
    }
    if is_branch_mnemonic(mnemonic) {
        return InsnClass::Branch;
    }
    if is_alu_mnemonic(mnemonic) {
        return InsnClass::Alu;
    }
    InsnClass::Other
}

/// True when an RTL insn and a real instruction are plausible partners: same
/// coarse class, and -- when both sides expose one -- the same destination
/// register. Neither side exposing a destination register (stores, calls,
/// branches) falls back to class agreement alone, which is weaker but still
/// meaningful: it is what lets a store correlate with its scheduler trace even
/// though "which store" cannot be pinned down further without simulating the
/// memory address.
///
/// PORT NOTE: `destRegister` yields a JS number (`f64` here, because an RTL
/// `(reg:SI 3 r3)` number is parsed with `Number(...)`) while
/// `primaryDestRegister` yields an integer. JS `===` between two numbers is a
/// plain numeric comparison, so the `i64` side is widened to `f64`; every
/// register number is far inside the exactly-representable range, so the widen
/// is lossless and the comparison is identical.
pub fn compatible(insn: &RtlInsn, instruction: &Instruction) -> bool {
    let insn_class = classify_rtl(insn);
    let instruction_class = classify_instruction(instruction);
    if insn_class != instruction_class {
        return false;
    }
    if insn_class == InsnClass::Alu || insn_class == InsnClass::Load {
        if let (Some(rtl_dest), Some(real_dest)) =
            (dest_register(insn), primary_dest_register(instruction))
        {
            return rtl_dest == real_dest as f64;
        }
    }
    true
}

#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub struct Alignment {
    /// `(insn index, instruction index)` pairs, in ascending order.
    pub pairs: Vec<(usize, usize)>,
    pub unmatched_insns: Vec<usize>,
    pub unmatched_instructions: Vec<usize>,
}

impl Alignment {
    pub fn pair_refs<'a>(
        &self,
        insns: &'a [RtlInsn],
        instructions: &'a [Instruction],
    ) -> Vec<(&'a RtlInsn, &'a Instruction)> {
        self.pairs
            .iter()
            .map(|&(i, j)| (&insns[i], &instructions[j]))
            .collect()
    }
}

/// Standard longest-common-compatible-subsequence DP (Needleman-Wunsch without
/// a gap penalty beyond "skip"), scored by [`compatible`]. O(n*m); n and m are
/// function-sized (tens of entries), never a performance concern here.
///
/// PORT NOTE: the TS scores an incompatible cell `-Infinity` and takes
/// `Math.max` of three candidates. Every reachable score is a non-negative
/// integer count, so `-Infinity` can never win a `Math.max` against
/// `score[i-1][j]`/`score[i][j-1]` (both >= 0) -- `i64::MIN` reproduces it
/// exactly while keeping the table integral, which also keeps the traceback's
/// `score[i][j] === score[i-1][j-1] + 1` an exact equality rather than a float
/// comparison. The traceback's tie-breaking order (diagonal, then up, then
/// left) is preserved verbatim, because it decides WHICH of several equally
/// long alignments is reported.
pub fn align(insns: &[RtlInsn], instructions: &[Instruction]) -> Alignment {
    let n = insns.len();
    let m = instructions.len();
    let mut score = vec![vec![0i64; m + 1]; n + 1];
    for i in 1..=n {
        for j in 1..=m {
            let matched = if compatible(&insns[i - 1], &instructions[j - 1]) {
                score[i - 1][j - 1] + 1
            } else {
                i64::MIN
            };
            score[i][j] = matched.max(score[i - 1][j]).max(score[i][j - 1]);
        }
    }
    let mut pairs: Vec<(usize, usize)> = Vec::new();
    let mut matched_insns = vec![false; n];
    let mut matched_instructions = vec![false; m];
    let mut i = n;
    let mut j = m;
    while i > 0 && j > 0 {
        if compatible(&insns[i - 1], &instructions[j - 1]) && score[i][j] == score[i - 1][j - 1] + 1
        {
            pairs.push((i - 1, j - 1));
            matched_insns[i - 1] = true;
            matched_instructions[j - 1] = true;
            i -= 1;
            j -= 1;
        } else if score[i][j] == score[i - 1][j] {
            i -= 1;
        } else {
            j -= 1;
        }
    }
    pairs.reverse();
    Alignment {
        pairs,
        unmatched_insns: (0..n).filter(|&index| !matched_insns[index]).collect(),
        unmatched_instructions: (0..m)
            .filter(|&index| !matched_instructions[index])
            .collect(),
    }
}

// ---------------------------------------------------------------------------
// Reporting
// ---------------------------------------------------------------------------

/// PORT NOTE: `String(someNumber)` on an integral JS number prints no `.0`, and
/// RTL uids are always integral in practice. This reproduces that rendering so
/// a diff against the TypeScript driver is byte-for-byte, and falls back to
/// Rust's own float formatting only for the non-integral values `Number(...)`
/// would have accepted.
pub fn js_number_text(value: f64) -> String {
    if value.is_finite() && value.fract() == 0.0 && value.abs() < 9.007_199_254_740_992e15 {
        format!("{}", value as i64)
    } else if value.is_infinite() {
        (if value > 0.0 { "Infinity" } else { "-Infinity" }).to_string()
    } else if value.is_nan() {
        "NaN".to_string()
    } else {
        format!("{value}")
    }
}

/// One line per pair, then one per unmatched entry on each side. The header
/// counts come first so a truncated diff still shows a disagreement.
pub fn report(alignment: &Alignment, insns: &[RtlInsn], instructions: &[Instruction]) -> String {
    let mut out = String::new();
    out.push_str(&format!(
        "pairs={} unmatched_insns={} unmatched_instructions={}\n",
        alignment.pairs.len(),
        alignment.unmatched_insns.len(),
        alignment.unmatched_instructions.len()
    ));
    for &(i, j) in &alignment.pairs {
        out.push_str(&format!(
            "pair uid={} offset=0x{:x} class={}\n",
            js_number_text(insns[i].uid),
            instructions[j].offset,
            classify_rtl(&insns[i]).as_str()
        ));
    }
    for &i in &alignment.unmatched_insns {
        out.push_str(&format!(
            "insn uid={} class={}\n",
            js_number_text(insns[i].uid),
            classify_rtl(&insns[i]).as_str()
        ));
    }
    for &j in &alignment.unmatched_instructions {
        out.push_str(&format!(
            "instruction offset=0x{:x} mnemonic={} class={}\n",
            instructions[j].offset,
            instructions[j].mnemonic,
            classify_instruction(&instructions[j]).as_str()
        ));
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    use rtl_insn::{InsnKind, Set};
    use thumb_disasm::{parse_instruction, Operand};

    fn alu_insn(uid: f64, dest_reg: i64) -> RtlInsn {
        RtlInsn {
            uid,
            kind: InsnKind::Insn,
            code: Some("set".to_string()),
            call_target: None,
            raw: String::new(),
            dependencies: Vec::new(),
            set: Some(Set {
                dest: RtlExpr::Reg {
                    number: dest_reg as f64,
                    name: format!("r{dest_reg}"),
                },
                src: RtlExpr::Unknown {
                    raw: String::new(),
                },
            }),
        }
    }

    fn load_insn(uid: f64, dest_reg: i64) -> RtlInsn {
        let mut insn = alu_insn(uid, dest_reg);
        insn.set.as_mut().unwrap().src = RtlExpr::Mem {
            address: Box::new(RtlExpr::Reg {
                number: 0.0,
                name: "r0".to_string(),
            }),
        };
        insn
    }

    fn call_insn(uid: f64) -> RtlInsn {
        RtlInsn {
            uid,
            kind: InsnKind::CallInsn,
            code: Some("parallel".to_string()),
            set: None,
            call_target: Some(RtlExpr::Unknown {
                raw: String::new(),
            }),
            raw: String::new(),
            dependencies: Vec::new(),
        }
    }

    fn instr(offset: i64, mnemonic: &str, dest_reg: Option<i64>) -> Instruction {
        Instruction {
            offset,
            mnemonic: mnemonic.to_string(),
            raw: String::new(),
            operands: match dest_reg {
                None => Vec::new(),
                Some(number) => vec![Operand::Reg {
                    name: format!("r{number}"),
                    number,
                    writeback: false,
                }],
            },
        }
    }

    /// The TypeScript `selfTest()` fixture, assembled here rather than asserted
    /// as a tautology: two RTL insns writing r3 then r2, matched against real
    /// instructions with TWO extra epilogue-expansion rows inserted on the real
    /// side that have no RTL counterpart -- the exact failure mode a strict 1:1
    /// position zip cannot survive.
    #[test]
    fn skips_an_epilogue_expansion_gap() {
        let insns = vec![alu_insn(15.0, 3), alu_insn(17.0, 2), call_insn(19.0)];
        let instructions = vec![
            instr(0, "adds", Some(3)),
            instr(2, "pop", None),
            instr(4, "pop", None),
            instr(6, "movs", Some(2)),
            instr(8, "bl", None),
        ];
        let result = align(&insns, &instructions);
        assert_eq!(result.pairs, vec![(0, 0), (1, 3), (2, 4)]);
        assert_eq!(result.unmatched_instructions, vec![1, 2]);
        assert!(result.unmatched_insns.is_empty());
        let refs = result.pair_refs(&insns, &instructions);
        assert_eq!(refs[1].0.uid, 17.0);
        assert_eq!(refs[1].1.offset, 6);
    }

    #[test]
    fn destination_register_mismatch_is_incompatible() {
        assert!(!compatible(&alu_insn(1.0, 3), &instr(0, "adds", Some(4))));
        assert!(compatible(&alu_insn(1.0, 3), &instr(0, "adds", Some(3))));
    }

    /// A class match with no destination register on the real side falls back
    /// to class agreement, which is what lets an unnumbered row still pair.
    #[test]
    fn missing_destination_falls_back_to_class() {
        assert!(compatible(&alu_insn(1.0, 3), &instr(0, "movs", None)));
    }

    #[test]
    fn classifies_both_sides() {
        assert_eq!(classify_rtl(&alu_insn(1.0, 0)), InsnClass::Alu);
        assert_eq!(classify_rtl(&load_insn(1.0, 0)), InsnClass::Load);
        assert_eq!(classify_rtl(&call_insn(1.0)), InsnClass::Call);

        let mut store = alu_insn(2.0, 1);
        store.set.as_mut().unwrap().dest = RtlExpr::Mem {
            address: Box::new(RtlExpr::Reg {
                number: 1.0,
                name: "r1".to_string(),
            }),
        };
        assert_eq!(classify_rtl(&store), InsnClass::Store);

        let mut jump = alu_insn(3.0, 1);
        jump.kind = InsnKind::JumpInsn;
        assert_eq!(classify_rtl(&jump), InsnClass::Branch);

        let mut bare = alu_insn(4.0, 1);
        bare.set = None;
        assert_eq!(classify_rtl(&bare), InsnClass::Other);

        assert_eq!(
            classify_instruction(&parse_instruction(0, "ldr\tr3, [r0, #4]")),
            InsnClass::Load
        );
        assert_eq!(
            classify_instruction(&parse_instruction(0, "str\tr3, [r0, #4]")),
            InsnClass::Store
        );
        assert_eq!(classify_instruction(&instr(0, "bl", None)), InsnClass::Call);
        assert_eq!(classify_instruction(&instr(0, "bne", None)), InsnClass::Branch);
        assert_eq!(classify_instruction(&instr(0, "nop", None)), InsnClass::Other);
    }

    /// A subreg destination still exposes its register, and an RTL load must
    /// not pair with a real ALU row.
    #[test]
    fn subreg_destination_and_cross_class_rejection() {
        let mut sub = alu_insn(5.0, 2);
        sub.set.as_mut().unwrap().dest = RtlExpr::Subreg {
            inner: Box::new(RtlExpr::Reg {
                number: 2.0,
                name: "r2".to_string(),
            }),
        };
        assert_eq!(classify_rtl(&sub), InsnClass::Alu);
        assert!(compatible(&sub, &instr(0, "adds", Some(2))));
        assert!(!compatible(&load_insn(6.0, 2), &instr(0, "adds", Some(2))));
        assert!(compatible(
            &load_insn(6.0, 2),
            &parse_instruction(0, "ldr\tr2, [r0, #0]")
        ));
    }

    /// `mul` is deliberately not in the alu set while `muls` is -- pinning the
    /// original's `muls` (no `s?`) so a later "cleanup" cannot silently widen it.
    #[test]
    fn mul_is_not_alu_but_muls_is() {
        assert_eq!(classify_instruction(&instr(0, "mul", Some(1))), InsnClass::Other);
        assert_eq!(classify_instruction(&instr(0, "muls", Some(1))), InsnClass::Alu);
    }

    #[test]
    fn empty_inputs_produce_an_empty_alignment() {
        let empty = align(&[], &[]);
        assert_eq!(empty, Alignment::default());
        let insns = vec![alu_insn(1.0, 0)];
        let only_insns = align(&insns, &[]);
        assert_eq!(only_insns.unmatched_insns, vec![0]);
        assert!(only_insns.pairs.is_empty());
    }

    /// The report is what the differential harness diffs, so its shape is
    /// pinned here too.
    #[test]
    fn report_lists_pairs_then_both_unmatched_sides() {
        let insns = vec![alu_insn(15.0, 3), alu_insn(21.0, 7)];
        let instructions = vec![instr(0, "adds", Some(3)), instr(2, "pop", None)];
        let text = report(&align(&insns, &instructions), &insns, &instructions);
        assert_eq!(
            text,
            "pairs=1 unmatched_insns=1 unmatched_instructions=1\n\
             pair uid=15 offset=0x0 class=alu\n\
             insn uid=21 class=alu\n\
             instruction offset=0x2 mnemonic=pop class=load\n"
        );
    }

    #[test]
    fn js_number_text_matches_string_of_number() {
        assert_eq!(js_number_text(15.0), "15");
        assert_eq!(js_number_text(-3.0), "-3");
        assert_eq!(js_number_text(1.5), "1.5");
        assert_eq!(js_number_text(f64::INFINITY), "Infinity");
        assert_eq!(js_number_text(f64::NAN), "NaN");
    }
}
