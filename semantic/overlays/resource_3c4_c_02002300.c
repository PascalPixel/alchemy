/*
 * resource_3c4 owner at 0x02002300, 46 bytes: code 0x02002300-0x0200232d,
 * ending on the alignment halfword at 0x0200232e before the prologue at
 * 0x02002330.
 *
 * The sibling of 0x02001318 - halfword-identical apart from the six placement
 * arguments and the local step it runs.  Open the scripted-action bracket,
 * place one actor with the six-argument placement service (four register
 * arguments plus the two stack words 19 and 45), run the party push routine at
 * 0x020000c4 and the local step at 0x020020a4, close the bracket.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`).  The disassembler prints
 * 0x020053fa, 0x020053b6, 0x020023e2, 0x020043c6 and 0x02005422; all five are
 * artefacts.  The real targets are veneers 0x020030f4 (-> Func_0808a018),
 * 0x0200309c (-> Func_080091c0) and 0x020030fc (-> Func_0808a020), plus the
 * prologues 0x020000c4 and 0x020020a4.
 *
 * Previously skipped because 0x020023e2 looked like an in-image address that
 * is not a function start; it was the decoding artefact.
 *
 * The epilogue is `add sp,#8 ; pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;

void Func_0808a018();      /* scripted-action bracket: open */

/* Four register arguments plus two stack words. */
void Func_080091c0();

void Func_020000c4(void);  /* the party push routine (this overlay) */
void Func_020020a4(void);  /* local cutscene body (this overlay) */
void Func_0808a020();      /* scripted-action bracket: close */

void Func_02002300(void)
{
    Func_0808a018();
    Func_080091c0(83, 45, 11, 8, 19, 45);
    Func_020000c4();
    Func_020020a4();
    Func_0808a020();
}
