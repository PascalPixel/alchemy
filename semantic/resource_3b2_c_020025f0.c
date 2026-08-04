typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x020025f0, 272 bytes: code 0x020025f0-0x020026ff,
 * no literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x020026f8 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Three arms `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 25f0 --json` (17 sites, 4 distinct targets):
 *   0x020025f6, 0x02002600, 0x0200260a, 0x02002614, 0x020026ce
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x02002632, 0x0200263c, 0x02002658, 0x02002664, 0x0200267a, 0x02002686,
 *   0x0200269c, 0x020026ac, 0x020026bc
 *                              -> prologue 0x02001774 -> Func_02001774
 *   0x020026c8                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x020026e4, 0x020026f4     -> veneer 0x02003028 -> Func_080091c0
 *
 * This is the Z-AXIS member of the transition family (0x02002a98, 0x02002b80,
 * 0x02002c4c, 0x02002d0c, 0x02002dd0).  Where those switch on the actor's tile
 * COLUMN and pass the id as Func_02001774's dx, this one switches on slot 18's
 * tile ROW and passes it as dz (dx is always 0), and its two collision
 * repaints are the transposed rectangle (3 x 1 instead of 1 x 3, anchored at
 * `x - 1`).  The two companions consulted are slots 19 and 14, both by tile
 * column, both against the 6..8 band.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 19's tile column through the whole
 * decision tree and is then REASSIGNED to `slot18Column - 1` for the two
 * repaints; the two values are tracked separately here.
 *
 * NOTE on call sites: two of the nine Func_02001774 sites are each reached from
 * TWO arms of the decision tree — the id-32 site at 0x0200267a (rows 9 and 12)
 * and the id-80 site at 0x0200269c (rows 9 and 14).  Writing a call per arm
 * would inject two phantom calls into the multiset, so those two arms `goto`
 * the shared site exactly as the assembly branches to it.
 *
 * UNCERTAINTY: the third argument of Func_02001774 is an opaque transition id,
 * as in the rest of the family.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_020025f0(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    column = *(s32 *)(Func_0808a080(18) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    companion19Column = *(s32 *)(Func_0808a080(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0808a080(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Func_02001774(18, 0, 64);
        Func_02001774(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Func_02001774(18, 0, 48);
        } else {
            Func_02001774(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Func_02001774(18, 0, 32);
        } else {
            Func_02001774(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Func_02001774(18, 0, 80);
    } else if (row == 15) {
        Func_02001774(18, 0, 64);
    } else if (row == 18) {
        Func_02001774(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Func_080000c0(2);

    column -= 1;
    Func_080091c0(column, row, 3, 1,
                  column, *(s32 *)(Func_0808a080(18) + 16) >> 20);
    Func_080091c0(0, 0, 3, 1, column, row);
}
