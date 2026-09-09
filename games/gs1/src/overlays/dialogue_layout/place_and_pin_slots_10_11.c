/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */
#include "types.h"

typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

void Func_020054c2();
void Func_0200333e();
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
void Func_020054ee();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02005512();

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Func_020054c2(93, 30, 6, 5, k5, k6);
    }
    Func_0200333e(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Func_020054ee(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Func_02005512(2, 36, 1, 1, col20, row);
    }
}
