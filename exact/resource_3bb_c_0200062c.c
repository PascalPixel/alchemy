#include "types.h"

/* STILL-OPEN: adopt --where differing_bytes=10/112. alchemist.ts refused
 * (tiers: class, depend-count, model-divergence, priority) -- matches the
 * documented DEAD-END FINGERPRINT (adjacent r5/r6-class register swap in an
 * asrs/str block), genuinely unfixed, do not hand-permute. */

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

/* Per-site veneers (raw sub_ symbols from the overlay .s), each call site
 * spelled with its own symbol even though several reach the same import. */
void Func_020044ca();
s32 *Func_02004598();
void Func_020044e4();
s32 *Func_020045b2();
void Func_020044fe();
s32 *Func_020045cc();
void Func_0200451a();

void Func_0200062c(void)
{
    s32 field;

    Func_020044ca(100, 11, 12, 4, 14, 11);

    field = Func_02004598(15)[2] >> 20;
    Func_020044e4(13, 28, 1, 4, field, 11);

    field = Func_020045b2(16)[2] >> 20;
    Func_020044fe(13, 28, 1, 4, field, 11);

    field = Func_020045cc(17)[4] >> 20;
    Func_0200451a(13, 28, 4, 1, 18, field);
}
