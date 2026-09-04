#include "types.h"

#define SceneEffect_SetBlendAlpha0607 Func_020018d0

/* Complete blend-alpha setter through return and its two pool words. */
void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}
