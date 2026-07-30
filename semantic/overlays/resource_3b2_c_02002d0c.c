typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x02002d0c, 196 bytes: code 0x02002d0c-0x02002dcf,
 * no literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02002dc8 ending `pop {r0} / bx r0` — so
 * the owner is **void**.  Three arms `b.n` straight to that epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2d0c --json` (9 sites, 4 distinct targets):
 *   0x02002d12 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002d1c -> veneer 0x02003088 -> Func_0808a080
 *   0x02002d26 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002d30 -> veneer 0x02003088 -> Func_0808a080
 *   0x02002d8c -> prologue 0x02001774 -> Func_02001774 (this overlay)
 *   0x02002d98 -> veneer 0x02002fb8 -> Func_080000c0  (frame wait)
 *   0x02002d9e -> veneer 0x02003088 -> Func_0808a080
 *   0x02002db4 -> veneer 0x02003028 -> Func_080091c0  (collision repaint)
 *   0x02002dc4 -> veneer 0x02003028 -> Func_080091c0
 *
 * The mirror of the slot-16 beat at 0x02002dd0: same slots, same 9..11 row
 * band, but the column table runs the other way (13, 12, 9, 8 instead of
 * 6, 8, 9, 12) and the chosen id is NEGATED at the single call site — this is
 * the return leg of the same transition.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 18's tile row through the decision
 * tree and is then REASSIGNED to `slot16Row - 1` for the two repaints; the two
 * values are tracked separately here.
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

void Func_02002d0c(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 transition;

    column = *(s32 *)(Func_0808a080(16) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(16) + 16) >> 20;
    companion18Row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    companion9Row = *(s32 *)(Func_0808a080(9) + 16) >> 20;

    transition = 0;
    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            transition = 16;
        } else if ((u32)(companion18Row - 9) <= 2) {
            transition = 64;
        } else {
            transition = 112;
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        transition = ((u32)(companion18Row - 9) <= 2) ? 48 : 96;
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        transition = 48;
    } else if (column == 8) {
        transition = 32;
    } else if (column == 6) {
        return;
    }

    /* Single call site; the arms above only choose its magnitude. */
    if (transition != 0) {
        Func_02001774(16, -transition, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(16) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
