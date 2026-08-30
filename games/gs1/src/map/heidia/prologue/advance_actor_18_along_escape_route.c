#include "types.h"

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_02005680();                    /* scene entity by selector */
u8 *Func_0200568a();
u8 *Func_02005694();
u8 *Func_0200569e();
u8 *Func_02005758();
void Func_02005682();                   /* wait N frames */
void Func_0200570e();                   /* collision repaint, six arguments */
void Func_0200571e();

/* This overlay's transition starter at 0x02001774, one name per site. */
void Func_02003da8();
void Func_02003db2();
void Func_02003dce();
void Func_02003dda();
void Func_02003df0();
void Func_02003dfc();
void Func_02003e12();
void Func_02003e22();
void Func_02003e32();

#define AdvanceActor18AlongEscapeRoute Func_020025f0

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Func_02005680(18) + 8) >> 20;
    row = *(s32 *)(Func_0200568a(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Func_02005694(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0200569e(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Func_02003da8(18, 0, 64);
        Func_02003db2(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Func_02003dce(18, 0, 48);
        } else {
            Func_02003dda(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Func_02003df0(18, 0, 32);
        } else {
            Func_02003dfc(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Func_02003e12(18, 0, 80);
    } else if (row == 15) {
        Func_02003e22(18, 0, 64);
    } else if (row == 18) {
        Func_02003e32(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Func_02005682(2);

    column -= 1;
    Func_0200570e(column, row, 3, 1,
                  column, *(s32 *)(Func_02005758(18) + 16) >> 20);
    Func_0200571e(0, 0, 3, 1, column, row);
}
