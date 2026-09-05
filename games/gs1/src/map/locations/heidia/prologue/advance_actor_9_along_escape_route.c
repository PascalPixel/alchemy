#include "types.h"

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

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

void AdvanceActor9AlongEscapeRoute(void)
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
