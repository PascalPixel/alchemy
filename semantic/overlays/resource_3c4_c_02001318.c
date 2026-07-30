/*
 * resource_3c4 owner at 0x02001318, 46 bytes: code 0x02001318-0x02001345,
 * ending on the alignment halfword at 0x02001346 before the prologue at
 * 0x02001348.
 *
 * A cutscene step.  It opens the scripted-action bracket, places one actor
 * with the six-argument placement service (four register arguments plus the
 * two stack words 9 and 38), runs the party push routine at 0x020000c4 and the
 * local step at 0x020023a0, then closes the bracket.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`).  The disassembler prints
 * 0x02004412, 0x020043ce, 0x020013fa, 0x020036da and 0x0200443a; every one is
 * an artefact.  The real targets are veneers 0x020030f4 (-> Func_0808a018),
 * 0x0200309c (-> Func_080091c0) and 0x020030fc (-> Func_0808a020), plus the
 * prologues 0x020000c4 and 0x020023a0.
 *
 * This owner was the headline example of the retired skip rule: 0x020013fa was
 * read as landing inside another function's `bl`, which is exactly what a
 * pc-relative decode of an overlay `bl` produces.  The site is an ordinary
 * call to 0x020000c4.
 *
 * The epilogue is `add sp,#8 ; pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;

void Func_0808a018();      /* scripted-action bracket: open */

/* Four register arguments plus two stack words. */
void Func_080091c0();

void Func_020000c4(void);  /* the party push routine (this overlay) */
void Func_020023a0(void);  /* local cutscene body (this overlay) */
void Func_0808a020();      /* scripted-action bracket: close */

void Func_02001318(void)
{
    Func_0808a018();
    Func_080091c0(73, 38, 5, 5, 9, 38);
    Func_020000c4();
    Func_020023a0();
    Func_0808a020();
}
