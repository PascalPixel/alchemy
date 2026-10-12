#include "ov697.h"

void Func_02000218(s32 side, s32 phase)
{
    if (side == 0) {
        if (phase == 3)
            Map_Stamp(0, 120, 1, 2, 23, phase);
        Map_Stamp(127, 3, 1, 2, 57, 3);
        Map_CopyCells(0, 127, 1, 1, 23, 8);
    } else {
        if (phase == 3)
            Map_Stamp(0, 120, 1, 2, 9, phase);
        Map_Stamp(127, 3, 1, 2, 43, 3);
        Map_CopyCells(1, 127, 1, 1, 9, 8);
    }
}

void Func_020002a4(s32 side, s32 phase)
{
    if (side == 0) {
        if (phase == 3)
            Map_Stamp(0, 120, 1, 2, 28, phase);
        Map_Stamp(127, 3, 1, 2, 62, 3);
        Map_CopyCells(1, 127, 1, 1, 28, 8);
    } else {
        if (phase == 3)
            Map_Stamp(0, 120, 1, 2, 4, phase);
        Map_Stamp(127, 3, 1, 2, 38, 3);
        Map_CopyCells(0, 127, 1, 1, 4, 8);
    }
}
