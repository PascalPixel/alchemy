#include "types.h"

extern s32 Func_0200039e(s32, s32, s32, s32, s32, s32);
extern void Func_020003b4(s32);

void Func_020000f8(void)
{
    s32 width = 4;
    s32 height = 9;

    Func_0200039e(25, 9, 1, 1, width, height);
    Func_020003b4(0x201);
}
