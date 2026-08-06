/* Return leg of the slot-14 transition beat: negated ids. */
#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_02005c10();                    /* scene entity by selector */
u8 *Func_02005c1a();
u8 *Func_02005c24();
u8 *Func_02005c2e();
u8 *Func_02005ca4();
void Func_02005bce();                   /* wait N frames */
void Func_02005c5a();                   /* collision repaint, six arguments */
void Func_02005c6a();

/* This overlay's transition starter at 0x02001774. */
void Func_0200437e();

void Func_02002b80(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005c10(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005c1a(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005c24(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005c2e(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200437e(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -64, 0);
        } else {
            Func_0200437e(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -48, 0);
        } else {
            Func_0200437e(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005bce(2);

    rowM1 = row - 1;
    Func_02005c5a(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005ca4(14) + 8) >> 20, rowM1);
    Func_02005c6a(0, 0, 1, 3, column, rowM1);
}
