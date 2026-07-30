/*
 * resource_3c4 owner at 0x020013e0, 66 bytes: code 0x020013e0-0x02001421,
 * ending on the alignment halfword at 0x02001422 before the prologue at
 * 0x02001424.
 *
 * A two-placement cutscene step.  It opens the scripted-action bracket, places
 * two actors with the six-argument placement service, runs the party push
 * routine at 0x020000c4 and the local step at 0x02002480, then closes the
 * bracket.
 *
 * r5 is loaded once with 25 and re-stored into [sp,#0] for both placements,
 * which is why the owner saves r5 while its siblings 0x02001318/0x02002300 do
 * not; the fifth argument is the same value at both sites.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`).  The disassembler prints
 * 0x020044da, 0x02004496, 0x020044a8, 0x020014d4, 0x02003894 and 0x02004514.
 * All six are artefacts: the two placement sites carry bit-identical `bl`
 * halfwords yet print targets 0x12 apart, which is the signature of the bug.
 * The real targets are veneers 0x020030f4 (-> Func_0808a018), 0x0200309c
 * (-> Func_080091c0, twice) and 0x020030fc (-> Func_0808a020), plus the
 * prologues 0x020000c4 and 0x02002480.
 *
 * Previously skipped because 0x020014d4 looked like an in-image address that
 * is not a function start; it was the decoding artefact.
 *
 * The epilogue is `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0`, so the owner is
 * void.
 */
typedef signed int s32;

void Func_0808a018();      /* scripted-action bracket: open */

/* Four register arguments plus two stack words. */
void Func_080091c0();

void Func_020000c4(void);  /* the party push routine (this overlay) */
void Func_02002480(void);  /* local cutscene body (this overlay) */
void Func_0808a020();      /* scripted-action bracket: close */

void Func_020013e0(void)
{
    Func_0808a018();
    Func_080091c0(89, 49, 3, 2, 25, 49);
    Func_080091c0(89, 51, 8, 5, 25, 51);
    Func_020000c4();
    Func_02002480();
    Func_0808a020();
}
