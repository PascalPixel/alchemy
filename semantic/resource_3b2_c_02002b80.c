#include "types.h"

/*
 * resource_3b2 owner at 0x02002b80, 204 bytes: code 0x02002b80-0x02002c4b,
 * no literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02002c44 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Four arms `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2b80 --json` (9 sites, 4 distinct targets):
 *   0x02002b86 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002b90 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002b9a -> veneer 0x02003088 -> Func_0808a080
 *   0x02002ba4 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002c08 -> prologue 0x02001774 -> Func_02001774 (this overlay)
 *   0x02002c14 -> veneer 0x02002fb8 -> Func_080000c0  (frame wait)
 *   0x02002c1a -> veneer 0x02003088 -> Func_0808a080
 *   0x02002c30 -> veneer 0x02003028 -> Func_080091c0  (collision repaint)
 *   0x02002c40 -> veneer 0x02003028 -> Func_080091c0
 *
 * The slot-14 / 12..14-row-band member of the same four-owner family as
 * 0x02002b80 / 0x02002c4c / 0x02002d0c / 0x02002dd0.  It is the return leg
 * (the id is NEGATED at the call), and unlike its slot-16 twin at 0x02002d0c
 * its column-8 arm also guards on the companion row before committing.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 18's tile row through the decision
 * tree and is then REASSIGNED to `slot14Row - 1` for the two repaints.
 *
 * UNCERTAINTY: the second argument of Func_02001774 is an opaque transition
 * magnitude; only its sign convention (negated on this leg) is established.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002b80(void)
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

    transition = 0;
    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            transition = 16;
        } else if ((u32)(companion18Row - 12) <= 2) {
            transition = 64;
        } else {
            transition = 112;
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        transition = ((u32)(companion18Row - 12) <= 2) ? 48 : 96;
    } else if (column == 9) {
        transition = 48;
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
    } else if (column == 8) {
        transition = 32;
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
    } else if (column == 6) {
        return;
    }

    /* Single call site; the arms above only choose its magnitude. */
    if (transition != 0) {
        Func_02001774(14, -transition, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(14) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
