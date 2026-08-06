/* Outbound leg of the slot-14 transition beat. */
#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_02005cdc();                    /* scene entity by selector */
u8 *Func_02005ce6();
u8 *Func_02005cf0();
u8 *Func_02005cfa();
u8 *Func_02005d62();
void Func_02005c8c();                   /* wait N frames */
void Func_02005d18();                   /* collision repaint, six arguments */
void Func_02005d28();

/* This overlay's transition starter at 0x02001774. */
void Func_0200443c();

void Func_02002c4c(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005cdc(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005ce6(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005cf0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005cfa(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200443c(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200443c(14, 64, 0);
        } else {
            Func_0200443c(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 64, 0);
    } else if (column == 12) {
        Func_0200443c(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005c8c(2);

    rowM1 = row - 1;
    Func_02005d18(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005d62(14) + 8) >> 20, rowM1);
    Func_02005d28(0, 0, 1, 3, column, rowM1);
}
