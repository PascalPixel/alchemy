#include "types.h"

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
u8 *Func_0808a080();
void Func_080091c0();

/* This overlay's own routine at file offset 0x0310. */
void Func_02000310();

void Func_02000400(void)
{
    u8 *record;
    s32 x;
    s32 partner;
    s32 step;

    record = Func_0808a080(*(s32 *)(0x02000240 + 500));
    x = *(s32 *)(record + 8) >> 20;

    step = 0;
    partner = 32;
    if ((*(s32 *)(record + 16) >> 20) > 12) {
        partner = 33;
    }

    record = Func_0808a080(partner);
    if ((*(s32 *)(record + 8) >> 20) != x) return;

    if (x > 51) {
        if ((*(s32 *)0x03001ae8 & 0x20) != 0) step = -64;
    } else {
        if ((*(s32 *)0x03001ae8 & 0x10) != 0) step = 64;
    }
    if (step == 0) return;

    Func_02000310(partner, step, 0);

    Func_080091c0(120, 10, 5, 6, 48, 10);

    record = Func_0808a080(32);
    Func_080091c0(52, 28, 1, 3, *(s32 *)(record + 8) >> 20, 10);

    record = Func_0808a080(33);
    Func_080091c0(52, 28, 1, 3, *(s32 *)(record + 8) >> 20, 13);
}
