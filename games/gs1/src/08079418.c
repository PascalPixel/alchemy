#include "types.h"
extern u8 Data_02000040[];

s32 Func_08079418(u32 value)
{
    u32 shifted = value << 20;
    u32 shift;
    u32 mask;
    s32 result;

    shift = value & 4;
    mask = 15;
    value = shifted >> 23;
    result = Data_02000040[value];
    result &= mask << shift;
    return result >> shift;
}

