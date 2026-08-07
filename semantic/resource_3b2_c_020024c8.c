#include "types.h"

/*
 * resource_3b2 owner at 0x020024c8, 294 bytes: code 0x020024c8-0x020025ed and
 * a two-byte alignment `movs r0, r0` at 0x020025ee.  No literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r7}` (saving r8), an 8-byte
 * outgoing-argument frame, and the single epilogue at 0x020025e2 ending
 * `pop {r0} / bx r0` — so the owner is **void**.  Seven arms `b.n` to it.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 24c8 --json` (11 sites, 4 distinct targets):
 *   0x020024d2, 0x020024de, 0x020024e8, 0x020024f2, 0x020024fc, 0x020025b6
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x0200252e, 0x020025a4     -> prologue 0x02001774 -> Func_02001774
 *   0x020025b0                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x020025ce, 0x020025de     -> veneer 0x02003028 -> Func_080091c0
 *
 * The Z-axis RETURN leg for slot 18 — the mirror of 0x020025f0 (same slot, same
 * transposed 3 x 1 repaints, ids negated, and the row table runs downward
 * 19, 18, 15, 14, 12, 11, 9 instead of upward).  Three companions are consulted
 * by tile column against the 6..8 band: slots 19, 14 and 16.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetimes — this owner reuses r2 for two unrelated things:
 * it first holds slot 16's tile column and is then overwritten with the
 * transition id.  Every `subs r3, r2, #6` test happens before its arm's
 * `movs r2, #N`, so the two lives never overlap; they are separate variables
 * here.  Likewise r5 holds slot 14's tile column through the decision tree and
 * is REASSIGNED to `slot18Column - 1` for the repaints.
 *
 * NOTE on call sites: all seven row arms funnel into ONE shared `bl` at
 * 0x020025a4; the row-19 default additionally fires -64 at 0x0200252e before
 * falling into it with 96, and the id-32 setter at 0x0200259c is itself shared
 * by the row-14 and row-11 arms.  The C therefore has exactly two
 * Func_02001774 expressions, not one per arm.
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

void Func_020024c8(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;
    s32 transition;

    column = *(s32 *)(Func_0808a080(18) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    companion19Column = *(s32 *)(Func_0808a080(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0808a080(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_0808a080(16) + 8) >> 20;

    transition = 0;
    if (row == 19) {
        if ((u32)(companion19Column - 6) <= 2) {
            transition = 16;
        } else if ((u32)(companion14Column - 6) <= 2) {
            transition = 64;
        } else if ((u32)(companion16Column - 6) <= 2) {
            transition = 112;
        } else {
            /* This arm runs two transitions back to back. */
            Func_02001774(18, 0, -64);
            transition = 96;
        }
    } else if (row == 18) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 6) <= 2) {
            transition = 48;
        } else if ((u32)(companion16Column - 6) <= 2) {
            transition = 96;
        } else {
            transition = 144;
        }
    } else if (row == 15) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        transition = ((u32)(companion16Column - 6) <= 2) ? 48 : 96;
    } else if (row == 14) {
        transition = ((u32)(companion16Column - 6) <= 2) ? 32 : 80;
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
    } else if (row == 12) {
        if ((u32)(companion16Column - 6) <= 2) {
            return;
        }
    } else if (row == 11) {
        transition = 48;
        if ((u32)(companion16Column - 6) <= 2) {
            return;
        }
    } else if (row == 9) {
        transition = 32;
        return;
    }

    if (transition != 0) {
        Func_02001774(18, 0, -transition);
    }

    Func_080000c0(2);

    column -= 1;
    Func_080091c0(column, row, 3, 1,
                  column, *(s32 *)(Func_0808a080(18) + 16) >> 20);
    Func_080091c0(0, 0, 3, 1, column, row);
}
