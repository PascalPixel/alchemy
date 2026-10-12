#include "ov697.h"

void Func_02000cf8(s32 id, s32 side);

void Func_02000d70(void)
{
    Map_CopyCells(11, 64, 11, 8, 11, 22);
    Map_CopyCells(79, 64, 11, 8, 79, 22);
    Func_02000cf8(16, 0);
    Func_02000cf8(17, 0);
    Func_02000cf8(18, 1);
    if (GameFlag_Test(0x253))
        Map_CopyCells(0, 48, 1, 3, 19, 23);
    if (GameFlag_Test(0x254))
        Map_CopyCells(0, 48, 1, 3, 19, 26);
}
