/*
 * Setup step in resource_3c4. One six-argument placement, then two identical
 * follow-ups that pin an overlay at the 12.20 grid cell of slots 8 and 9: the
 * slot's +8 and +16 words are each shifted right by 20 and passed as the two
 * stack arguments of the placement call.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */
#include "types.h"

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

void Func_02005452();
void Func_020032ce();
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
void Func_0200547e();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();
void Func_020054a2();

void FieldScene_PlaceAndPinSlots8And9(void)
{
    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Func_02005452(73, 38, 5, 5, p5, p6);
    }
    Func_020032ce(9, 8);

    {
        s32 col = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Func_0200547e(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Func_020054a2(2, 36, 1, 1, col, row);
    }
}
