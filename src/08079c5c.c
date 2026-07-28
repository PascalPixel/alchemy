#include "types.h"

s32 Func_08079b24(s32, s32);
s32 Func_08079c5c(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    u32 product;

    product = (u32)Func_08079b24(
        (s32)((u32)no * 2 - 0xC8), 0) * (u32)value;
    product = (u32)multiplier * product;
    result = (s32)product;
    if (result < 0) {
        result = (s32)((u32)result + 0xFFFF);
    }
    return result >> 0x10;
}
