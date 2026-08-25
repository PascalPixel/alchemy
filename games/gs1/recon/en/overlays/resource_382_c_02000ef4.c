#include "types.h"

extern u8 *Data_03001ebc;

void Func_020028f4(s32, s32, s32);
void Func_02002916(s32, s32, s32);
void Func_020029ca(s32);

void Func_02000ef4(s32 x, s32 y, s32 continuation)
{
    Func_020028f4(0, 0x8000, 0x4000);
    Func_02002916(0, x, y);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_020029ca(continuation);
}
