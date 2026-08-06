#include "types.h"

/* STILL-OPEN: fixed a real bug this session -- the original source read the
 * RAM word via a folded numeric literal `*(s32 *)(0x02000240 + 500)`, which
 * GCC constant-folds into a single direct-address load; the reference wants
 * a symbol + runtime-computed index (`movs r2,#250 / lsls r2,#1 / adds
 * r3,r3,r2`), matching sibling owners (resource_3bb_c_020012f0.c etc.) that
 * read the same word via `Data_02000240[N]` array syntax. Rewriting as
 * `Data_02000240[125]` (s32 element type, 125*4=500 bytes) took adopt from
 * differing_bytes=177/192 to differing_bytes=62/192. The remainder is an
 * r5/r6-class register-allocation swap in the callee's return-value use --
 * alchemist.ts exhausted (tiers: depend-count, model-divergence,
 * original-order, priority, unaligned; 8 compiles, no improving move) and
 * overlay_mode_cohort singles sweep found no closing config. Genuinely
 * unfixed within budget; the Data_02000240 rewrite is a real improvement,
 * keep it for the next pass.
 */

/*
 * resource_3bb owner at 0x02000400, 192 bytes (0x02000400-0x020004bf):
 * 182 bytes of code, two alignment bytes at 0x020004b6, and the two-word
 * literal pool at 0x020004b8.
 *
 * Prologue `push {r5, r6, r7, lr} / sub sp, #8` at 0x02000400, epilogue
 * `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0`: r0 holds the popped
 * return address, so the owner is void.  The eight bytes of outgoing stack
 * carry the fifth and sixth arguments of each Func_080091c0 call.
 *
 * All eight `bl` sites are placed and reach three distinct callees, matching
 * the inventory row's calls=8.  Targets come from
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's annotations: 0x3f50 -> Func_0808a080 (four
 * sites), 0x3e88 -> Func_080091c0 (three), and this overlay's own prologue at
 * file offset 0x0310 (one).
 *
 * The pool word 0x02000240 is below the overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c), so it is a RAM global; the index is
 * built as 250 << 1 = 500, giving the same active-subject word at 0x02000434
 * that Func_020002e8 and Func_02003cf8 read.  0x03001ae8 is an IWRAM word
 * read for two bits, 0x10 and 0x20 - the GBA d-pad Right and Left bits in
 * KEYINPUT order, which is consistent with the +64 / -64 step they select.
 *
 * Shape: take the active subject's record, pick partner 32 or 33 on the
 * subject's z coordinate, and do nothing unless the partner shares the
 * subject's x coordinate.  When it does, the held direction selects a step
 * (-64 when x is past 51, +64 otherwise); with no direction held nothing
 * happens.  Otherwise the step is applied through Func_02000310 and three
 * lines are drawn, the last two reporting the x coordinates of participants
 * 32 and 33.
 *
 * Uncertainties: only the record field at +8 (x) and +16 (z) are asserted, and
 * both shifts are signed (`asrs`), so the fields are signed fixed-point
 * quantities with 20 fractional bits.  The two bits of 0x03001ae8 are read as
 * a held-direction mask on the strength of their values and use, not on a
 * traced writer.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */



/* This overlay's own routine at file offset 0x0310. */


/* RAM global read via symbol + runtime index, matching sibling owners
 * (resource_3bb_c_020012f0.c etc.) that read this same word. */
extern s32 Data_02000240[];

extern u8 * Func_02004360();
extern u8 * Func_02004378();
extern void Func_02000772();
extern void Func_020042fe();
extern u8 * Func_020043cc();
extern void Func_02004318();
extern u8 * Func_020043e6();
extern void Func_02004334();
void Func_02000400(void)
{
    u8 *record;
    s32 partner;
    s32 step;
    s32 x;

    record = Func_02004360(Data_02000240[125]);
    x = *(s32 *)(record + 8) >> 20;

    step = 0;
    partner = 32;
    if ((*(s32 *)(record + 16) >> 20) > 12) {
        partner = 33;
    }

    record = Func_02004378(partner);
    if ((*(s32 *)(record + 8) >> 20) != x) return;

    if (x > 51) {
        if ((*(s32 *)0x03001ae8 & 0x20) != 0) step = -64;
    } else {
        if ((*(s32 *)0x03001ae8 & 0x10) != 0) step = 64;
    }
    if (step == 0) return;

    Func_02000772(partner, step, 0);

    Func_020042fe(120, 10, 5, 6, 48, 10);

    record = Func_020043cc(32);
    Func_02004318(52, 28, 1, 3, *(s32 *)(record + 8) >> 20, 10);

    record = Func_020043e6(33);
    Func_02004334(52, 28, 1, 3, *(s32 *)(record + 8) >> 20, 13);
}
