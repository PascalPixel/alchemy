/* Return leg of the slot-16 transition beat: negated ids. */
#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_02005d9c();                    /* scene entity by selector */
u8 *Func_02005da6();
u8 *Func_02005db0();
u8 *Func_02005dba();
u8 *Func_02005e28();
void Func_02005d52();                   /* wait N frames */
void Func_02005dde();                   /* collision repaint, six arguments */
void Func_02005dee();

/* This overlay's transition starter at 0x02001774. */
void Func_02004502();

void Func_02002d0c(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005d9c(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005da6(16) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005db0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005dba(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            Func_02004502(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -64, 0);
        } else {
            Func_02004502(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -48, 0);
        } else {
            Func_02004502(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        Func_02004502(16, -48, 0);
    } else if (column == 8) {
        Func_02004502(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005d52(2);

    rowM1 = row - 1;
    Func_02005dde(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005e28(16) + 8) >> 20, rowM1);
    Func_02005dee(0, 0, 1, 3, column, rowM1);
}
