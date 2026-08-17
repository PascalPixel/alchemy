#include "types.h"

extern void Func_02000f92(s32, s32, s32, s32, s32, s32);
extern void Func_020010d8(s32, s32, s32);

void Func_02000260(void)
{
    s32 width = 3;
    s32 height = 26;

    Func_02000f92(3, 32, 1, 1, width, height);
    Func_020010d8(102, 0x380000, 0x1A80000);
}
