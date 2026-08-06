#include "types.h"

/*
 * resource_3b2 owner at 0x02002848, 314 bytes: code 0x02002848-0x02002981 and
 * a two-byte alignment `movs r0, r0` at 0x02002982.  No literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r7}` (saving r8), an 8-byte
 * outgoing-argument frame, and the single epilogue at 0x02002976 ending
 * `pop {r0} / bx r0` — so the owner is **void**.  Five arms `b.n` to it.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2848 --json` (18 sites, 4 distinct targets):
 *   0x02002852, 0x0200285e, 0x02002868, 0x02002872, 0x0200287c, 0x0200294a
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x020028a4, 0x020028ae, 0x020028ce, 0x020028da, 0x020028fa, 0x02002906,
 *   0x0200291e, 0x0200292e, 0x0200293e
 *                              -> prologue 0x02001774 -> Func_02001774
 *   0x02002944                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x02002962, 0x02002972     -> veneer 0x02003028 -> Func_080091c0
 *
 * The Z-axis FORWARD leg for slot 9 — the mirror of 0x02002700, with the same
 * three companions (slots 19, 14, 16, all tested by tile column against the
 * 9..11 band) and the same transposed 3 x 1 repaints, but the row table runs
 * upward 8, 11, 12, 14, 15, 18 and the ids are passed unnegated.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on register lifetime: r5 holds slot 14's tile column through the
 * decision tree and is then REASSIGNED to `slot9Column - 1` for the repaints.
 *
 * NOTE on call sites: unlike its siblings this owner writes each arm's id at
 * its own `bl`, so there are nine distinct sites — but two of them are entered
 * from more than one arm.  The id-48 site at 0x020028ce serves both row 8's
 * companion-14 arm and row 11's companion-19 arm, and the id-96 site at
 * 0x020028ae is both row 8's companion-19 arm and the fall-through after its
 * id-80 call.  The C reproduces the first with a `goto` into the row-11 arm and
 * the second as a plain fall-through, so exactly nine call expressions appear.
 *
 * UNCERTAINTY: the third argument of Func_02001774 is an opaque transition id.
 */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_020058dc();                    /* scene entity by selector */
u8 *Func_020058e8();
u8 *Func_020058f2();
u8 *Func_020058fc();
u8 *Func_02005906();
u8 *Func_020059d4();
void Func_020058fe();                   /* wait N frames */
void Func_0200598c();                   /* collision repaint, six arguments */
void Func_0200599c();

/* This overlay's transition starter at 0x02001774, one name per site. */
void Func_0200401a();
void Func_02004024();
void Func_02004044();
void Func_02004050();
void Func_02004070();
void Func_0200407c();
void Func_02004094();
void Func_020040a4();
void Func_020040b4();

void Func_02002848(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Func_020058dc(9) + 8) >> 20;
    row = *(s32 *)(Func_020058e8(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Func_020058f2(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_020058fc(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_02005906(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            Func_0200401a(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        Func_02004024(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            Func_02004044(9, 0, 48);
        } else {
            Func_02004050(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            Func_02004070(9, 0, 32);
        } else {
            Func_0200407c(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        Func_02004094(9, 0, 80);
    } else if (row == 15) {
        Func_020040a4(9, 0, 64);
    } else if (row == 18) {
        Func_020040b4(9, 0, 16);
    }

    Func_020058fe(2);

    column -= 1;
    Func_0200598c(column, row, 3, 1,
                  column, *(s32 *)(Func_020059d4(9) + 16) >> 20);
    Func_0200599c(0, 0, 3, 1, column, row);
}
