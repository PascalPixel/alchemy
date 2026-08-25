#include "types.h"

extern u8 *Data_03001ebc;

void Func_02001ba0(s32, s32, s32);
void Func_02001bc2(s32, s32, s32);
void Func_02001c56(s32);

void Func_02000a80(s32 x, s32 y, s32 delay)
{
    s32 zero = 0;

    Func_02001ba0(zero, 0x8000, 0x4000);
    Func_02001bc2(zero, x, y);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_02001c56(delay);
}
