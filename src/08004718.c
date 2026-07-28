#include "types.h"

void Func_080045f0(u32);
s32 Func_080046c4(s32);

void Func_08004718(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Func_080045f0(value);
    Func_080046c4(0x03001F78 - count);
}
