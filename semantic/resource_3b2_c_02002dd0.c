#include "types.h"

/*
 * resource_3b2 owner at 0x02002dd0, 160 bytes: code 0x02002dd0-0x02002e6f,
 * no literal pool (every constant is built with `movs`).
 *
 * Complete owner: `push {r5, r6, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02002e68 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Three early exits `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2dd0 --json` (8 sites, 4 distinct targets):
 *   0x02002dd6 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002de0 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002dea -> veneer 0x02003088 -> Func_0808a080
 *   0x02002e2c -> prologue 0x02001774 -> Func_02001774 (this overlay)
 *   0x02002e38 -> veneer 0x02002fb8 -> Func_080000c0  (frame wait)
 *   0x02002e3e -> veneer 0x02003088 -> Func_0808a080
 *   0x02002e54 -> veneer 0x02003028 -> Func_080091c0  (collision repaint)
 *   0x02002e64 -> veneer 0x02003028 -> Func_080091c0
 *
 * Behaviour: a doorway/transition beat.  The 16.16 world coordinates are
 * converted to tile units with an arithmetic `>> 20` (>> 16 to integers, >> 4
 * for the 16-pixel tile grid).  The player slot's tile column selects one of
 * four transition ids, with column 8 and column 13 exiting without one; the
 * companion slot's tile row (the 9..11 band) disambiguates columns 6 and 8.
 * Whatever happened, the tail waits two frames and repaints two collision
 * rectangles.
 *
 * The tile row is decremented once before both repaints, so both use the row
 * above the slot's own.
 *
 * UNCERTAINTY: the second argument of Func_02001774 (32, 112, 80, 64, 16) is
 * treated here as an opaque transition id; only the fact that it is a small
 * per-column constant is established.  Its third argument is a constant 0.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002dd0(void)
{
    s32 column;
    s32 row;
    s32 companionRow;
    s32 transition;

    column = *(s32 *)(Func_0808a080(16) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(16) + 16) >> 20;
    companionRow = *(s32 *)(Func_0808a080(9) + 16) >> 20;

    transition = -1;
    if (column == 6) {
        transition = ((u32)(companionRow - 9) <= 2) ? 32 : 112;
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        transition = 80;
    } else if (column == 9) {
        transition = 64;
    } else if (column == 12) {
        transition = 16;
    } else if (column == 13) {
        return;
    }

    /* Single call site; the arms above only choose its argument. */
    if (transition >= 0) {
        Func_02001774(16, transition, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(16) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
