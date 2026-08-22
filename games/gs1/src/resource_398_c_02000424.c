#include "types.h"

extern s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
extern void Func_02000d90(s32);

void Func_02000424(void)
{
    s32 width = 8;
    s32 height = 13;

    Func_02000d6a(31, 0, 1, 1, width, height);
    Func_02000d90(0x305);
}
