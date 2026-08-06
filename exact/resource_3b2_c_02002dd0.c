/* Outbound leg of the slot-16 transition beat. */
#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_02005e60();                    /* scene entity by selector */
u8 *Func_02005e6a();
u8 *Func_02005e74();
u8 *Func_02005ec8();
void Func_02005df2();                   /* wait N frames */
void Func_02005e7e();                   /* collision repaint, six arguments */
void Func_02005e8e();

/* This overlay's transition starter at 0x02001774. */
void Func_020045a2();

void Func_02002dd0(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Func_02005e60(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Func_02005e6a(16) + 16) >> 20;
    companionRow = *(s32 *)(Func_02005e74(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            Func_020045a2(16, 32, 0);
        } else {
            Func_020045a2(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        Func_020045a2(16, 80, 0);
    } else if (column == 9) {
        Func_020045a2(16, 64, 0);
    } else if (column == 12) {
        Func_020045a2(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005df2(2);

    row -= 1;
    Func_02005e7e(column, row, 1, 3,
                  *(s32 *)(Func_02005ec8(16) + 8) >> 20, row);
    Func_02005e8e(0, 0, 1, 3, column, row);
}
