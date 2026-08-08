//! `selfTest()` -- the `--self-test` path, verbatim.

use rtl_align::align;
use rtl_insn::parse_insns;
use thumb_disasm::parse_instruction;

pub const FIXTURE_DUMP: &str = concat!(
    "(insn 15 10 17 (set (reg:SI 3 r3)\n",
    "        (plus:SI (reg/v:SI 3 r3)\n",
    "            (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))\n",
    "(call_insn 19 17 21 (parallel [\n",
    "            (set (reg:SI 0 r0)\n",
    "                (call (mem:SI (symbol_ref:SI (\"Func_02001508\")) 0)\n",
    "                    (const_int 0 [0x0])))\n",
    "        ] ) 245 {*call_value_insn} (nil))\n",
);

/// Returns the line the TypeScript logs, or the message it throws.
pub fn self_test() -> Result<String, String> {
    let instructions = vec![
        parse_instruction(0, "adds\tr3, r3, r2"),
        parse_instruction(2, "pop\t{r5}"),
        parse_instruction(4, "bl\t0x11f4"),
    ];
    let insns = parse_insns(FIXTURE_DUMP);
    if insns.len() != 2 {
        return Err(format!(
            "expected 2 insns from fixture, got {}",
            insns.len()
        ));
    }
    let alignment = align(&insns, &instructions);
    if alignment.pairs.len() != 2 {
        return Err(format!(
            "expected 2 aligned pairs, got {}",
            alignment.pairs.len()
        ));
    }
    let (insn_index, instruction_index) = alignment.pairs[0];
    if instructions[instruction_index].offset != 0 || insns[insn_index].uid != 15.0 {
        return Err("wrong first pair".to_string());
    }
    // The unmatched `pop` (offset 2) must not have derailed the alignment: the
    // call at offset 4 must still find its call_insn.
    let (insn_index, instruction_index) = alignment.pairs[1];
    if instructions[instruction_index].offset != 4 || insns[insn_index].uid != 19.0 {
        return Err("gap tolerance failed".to_string());
    }
    if alignment.unmatched_instructions.len() != 1
        || instructions[alignment.unmatched_instructions[0]].offset != 2
    {
        return Err("expected the pop to be reported unmatched".to_string());
    }
    Ok("self-test=ok tool=candidate-explain".to_string())
}
