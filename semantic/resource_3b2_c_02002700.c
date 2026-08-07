#include "types.h"

/*
 * resource_3b2 owner at 0x02002700, 326 bytes: code 0x02002700-0x02002845 and
 * a two-byte alignment `movs r0, r0` at 0x02002846.  No literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r7}` (saving r8), an 8-byte
 * outgoing-argument frame, and the single epilogue at 0x0200283a ending
 * `pop {r0} / bx r0` — so the owner is **void**.  Seven arms `b.n` to it.
 *
 * Call targets resolved with `bun tools/lib/overlay_call_targets.ts resource_3b2
 * 2700 --json` (12 sites, 4 distinct targets):
 *   0x0200270a, 0x02002716, 0x02002720, 0x0200272a, 0x02002734, 0x0200280e
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x0200276c, 0x0200279c, 0x020027fc
 *                              -> prologue 0x02001774 -> Func_02001774
 *   0x02002808                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x02002826, 0x02002836     -> veneer 0x02003028 -> Func_080091c0
 *
 * The Z-axis return leg for slot 9 — the same shape as 0x020024c8 (slot 18)
 * with the same downward row table 19, 18, 15, 14, 12, 11 and the transposed
 * 3 x 1 repaints, but the three companions (slots 19, 14 and 16) are tested by
 * tile column against the 9..11 band rather than 6..8, and the fall-through
 * case ends in an UNSIGNED `(u32)row <= 9` guard rather than a row-9 equality.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetimes: r2 first holds slot 16's tile column and is then
 * overwritten with the transition id — every test using it happens before its
 * arm's `movs r2, #N`, so the two lives never overlap.  r5 likewise holds slot
 * 14's tile column and is REASSIGNED to `slot9Column - 1` for the repaints.
 *
 * NOTE on call sites: all six row arms funnel into ONE shared `bl` at
 * 0x020027fc, and the id-48 setter at 0x020027f4 is itself shared by three
 * arms.  Rows 19 and 18 each fire one extra transition first, at 0x0200276c
 * (-80) and 0x0200279c (-96).  The C therefore has exactly three
 * Func_02001774 expressions.
 *
 * UNCERTAINTY: the third argument of Func_02001774 is an opaque transition
 * magnitude; only its sign convention (negated on this leg) is established.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002700(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;
    s32 transition;

    column = *(s32 *)(Func_0808a080(9) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(9) + 16) >> 20;
    companion19Column = *(s32 *)(Func_0808a080(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0808a080(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_0808a080(16) + 8) >> 20;

    transition = 0;
    if (row == 19) {
        if ((u32)(companion19Column - 9) <= 2) {
            transition = 16;
        } else if ((u32)(companion14Column - 9) <= 2) {
            transition = 64;
        } else if ((u32)(companion16Column - 9) <= 2) {
            transition = 112;
        } else {
            /* This arm runs two transitions back to back. */
            Func_02001774(9, 0, -80);
            transition = 96;
        }
    } else if (row == 18) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            transition = 48;
        } else if ((u32)(companion16Column - 9) <= 2) {
            transition = 96;
        } else {
            Func_02001774(9, 0, -96);
            transition = 64;
        }
        transition = ((u32)(companion16Column - 9) <= 2) ? 48 : 112;
    } else if (row == 15) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
    } else if (row == 14) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        transition = ((u32)(companion16Column - 9) <= 2) ? 32 : 96;
    } else if (row == 12) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        transition = 64;
    } else if (row == 11) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        transition = 48;
    } else if ((u32)row <= 9) {
        return;
    }

    if (transition != 0) {
        Func_02001774(9, 0, -transition);
    }

    Func_080000c0(2);

    column -= 1;
    Func_080091c0(column, row, 3, 1,
                  column, *(s32 *)(Func_0808a080(9) + 16) >> 20);
    Func_080091c0(0, 0, 3, 1, column, row);
}
