#include "types.h"

s32 Func_08079b24(s32, s32);
s32 Func_08079c30(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    s32 shifted;
    s32 zero;

    result = (s32)((u32)Func_08079b24(no, 0) *
        (u32)value * (u32)multiplier);
    zero = 0;
    do {
        if (result < zero) {
            result = (s32)((u32)result + 0xFFFF);
        }
        shifted = result >> 0x10;
        return shifted;
    } while (zero);
}
