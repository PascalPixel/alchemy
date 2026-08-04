#include "types.h"

/*
 * resource_3b2 owner at 0x02002a98, 230 bytes: code 0x02002a98-0x02002b7d and
 * a two-byte alignment `movs r0, r0` at 0x02002b7e.  No literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02002b76 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Two arms `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2a98 --json` (11 sites, 4 distinct targets):
 *   0x02002a9e, 0x02002aa8, 0x02002ab2, 0x02002abc, 0x02002b4c
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x02002ae2, 0x02002b30, 0x02002b40
 *                              -> prologue 0x02001774 -> Func_02001774
 *   0x02002b46                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x02002b62, 0x02002b72     -> veneer 0x02003028 -> Func_080091c0
 *
 * The slot-19 member of the transition family that also contains 0x02002b80,
 * 0x02002c4c, 0x02002d0c and 0x02002dd0.  It differs from the others in three
 * ways: the companion tests are equalities against row 15 rather than a
 * three-row band, the column table has an extra entry (5) and an extra id
 * (128), and the column-3 default arm fires TWO transitions in sequence — 112
 * first, then 48 through the shared call site.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 18's tile row through the decision
 * tree and is then REASSIGNED to `slot19Row - 1` for the two repaints.
 *
 * NOTE on call sites: the ids chosen by the column 3/5/6/8/9 arms all reach one
 * shared `bl` at 0x02002b30; column 12 has its own site at 0x02002b40.  They
 * are spelled as two call expressions here, not one per arm, so the call
 * multiset matches the assembly (3 Func_02001774 sites, 3 C calls).
 *
 * UNCERTAINTY: the second argument of Func_02001774 is an opaque transition id.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002a98(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 transition;

    column = *(s32 *)(Func_0808a080(19) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(19) + 16) >> 20;
    companion18Row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    companion9Row = *(s32 *)(Func_0808a080(9) + 16) >> 20;

    transition = 0;
    if (column == 3) {
        if (companion18Row == 15) {
            transition = 32;
        } else if (companion9Row == 15) {
            transition = 80;
        } else {
            /* This arm runs two transitions back to back. */
            Func_02001774(19, 112, 0);
            transition = 48;
        }
    } else if (column == 5) {
        if (companion18Row == 15) {
            return;
        }
        transition = (companion9Row == 15) ? 48 : 128;
    } else if (column == 6) {
        transition = (companion9Row == 15) ? 32 : 112;
    } else if (column == 8) {
        if (companion9Row == 15) {
            return;
        }
        transition = 80;
    } else if (column == 9) {
        transition = 64;
    }

    if (transition != 0) {
        Func_02001774(19, transition, 0);
    } else if (column == 12) {
        Func_02001774(19, 16, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(19) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
