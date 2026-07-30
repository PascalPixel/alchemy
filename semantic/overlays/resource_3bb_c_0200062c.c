typedef signed int s32;

/*
 * resource_3bb owner at 0x0200062c, 112 bytes (0x0200062c-0x0200069b), all
 * code: the body loads no literal pool and no pool sits inside the span.
 *
 * Prologue `push {r5, lr}` / `sub sp, #8` at 0x0200062c, epilogue
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner is void.  The eight bytes of outgoing stack carry the
 * fifth and sixth arguments of each Func_080091c0 call, and r5 is a constant
 * 11 kept live across the calls because it is stored at both slots.
 *
 * All seven `bl` sites are placed and reach two distinct callees, matching the
 * inventory row's calls=7.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations: 0x3e88 -> Func_080091c0 (four sites), 0x3f50 -> Func_0808a080
 * (three sites).  The same pair, with the same six-argument spelling of
 * Func_080091c0, appears in the neighbouring owners 0x02000258 and
 * 0x0200028c, and the byte-exact sibling
 * assets/code/resource_3bb_c_020002c0.c spells that call as
 * Func_02004164(32, 37, 1, 4, 32, 77).
 *
 * Shape: one fixed line, then three lines whose fifth or sixth argument is a
 * field of the record Func_0808a080 returns for participants 15, 16 and 17,
 * shifted right arithmetically by 20.  Records 15 and 16 contribute their word
 * at +8; record 17 contributes its word at +16, and its value moves from the
 * fifth to the sixth argument slot while a literal 18 takes the fifth.
 *
 * Uncertainties: only the record fields at +8 and +16 are asserted, and the
 * shift is signed (`asrs`), so the fields are signed fixed-point quantities
 * with 20 fractional bits.  What Func_080091c0's six arguments mean is not
 * established here.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080091c0();
s32 *Func_0808a080();

void Func_0200062c(void)
{
    s32 *record;

    Func_080091c0(100, 11, 12, 4, 14, 11);

    record = Func_0808a080(15);
    Func_080091c0(13, 28, 1, 4, record[2] >> 20, 11);

    record = Func_0808a080(16);
    Func_080091c0(13, 28, 1, 4, record[2] >> 20, 11);

    record = Func_0808a080(17);
    Func_080091c0(13, 28, 4, 1, 18, record[4] >> 20);
}
