#include "types.h"

#define SetBlendAlphaCoefficients Func_020018c0
#define SceneEffect_SetBlendAlpha0607 Func_020018d0
void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    *(u16 *)0x04000052 = coefficient;
}

/* Complete blend-alpha setter through return and its two pool words. */
void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}
