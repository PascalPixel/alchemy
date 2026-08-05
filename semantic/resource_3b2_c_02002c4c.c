#include "types.h"

/*
 * resource_3b2 owner at 0x02002c4c, 190 bytes: code 0x02002c4c-0x02002d09,
 * no literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02002d02 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Three arms `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2c4c --json` (9 sites, 4 distinct targets):
 *   0x02002c52 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002c5c -> veneer 0x02003088 -> Func_0808a080
 *   0x02002c66 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002c70 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002cc6 -> prologue 0x02001774 -> Func_02001774 (this overlay)
 *   0x02002cd2 -> veneer 0x02002fb8 -> Func_080000c0  (frame wait)
 *   0x02002cd8 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002cee -> veneer 0x02003028 -> Func_080091c0  (collision repaint)
 *   0x02002cfe -> veneer 0x02003028 -> Func_080091c0
 *
 * This is the slot-14 sibling of the slot-16 beat at 0x02002dd0: the same
 * column table with the same ids (32/112/80/64/16) and the same two-repaint
 * tail, but it disambiguates with the 12..14 row band instead of 9..11, it
 * consults a second companion (slot 18) inside the column-6 arm, and columns 8
 * and 9 both exit early when the companion is in that band.
 *
 * World coordinates are converted to tile units with an arithmetic `>> 20`
 * (>> 16 to integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 18's tile row through the whole
 * decision tree and is then REASSIGNED to `slot14Row - 1` before the two
 * repaints.  It is two separate values, tracked per use here.
 *
 * UNCERTAINTY: the second argument of Func_02001774 is an opaque transition id,
 * as in the 0x02002dd0 sibling.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002c4c(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 transition;

    s32 permuted_7;
    column = *(s32 *)(Func_0808a080(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_0808a080(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_0808a080(9) + 16) >> 20;

    transition = -1;
    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            transition = 32;
        } else if ((u32)(companion18Row - 12) <= 2) {
            transition = 64;
        } else {
            transition = 112;
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        transition = 80;
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        transition = 64;
    } else if (column == 12) {
        transition = 16;
    } else if (column == 13) {
        return;
    }

    /* Single call site; the arms above only choose its argument. */
    if (transition >= 0) {
        Func_02001774(14, transition, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(14) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
