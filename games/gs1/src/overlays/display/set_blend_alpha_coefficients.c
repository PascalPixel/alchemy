#include "types.h"

#define SetBlendAlphaCoefficients Func_020018c0

void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    *(u16 *)0x04000052 = coefficient;
}
