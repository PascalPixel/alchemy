#include "types.h"

void Func_08004620(s32);
s32 Func_080046c4(s32);

void Func_0800473c(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Func_08004620(value);
    Func_080046c4(0x03001F7A - count);
}
