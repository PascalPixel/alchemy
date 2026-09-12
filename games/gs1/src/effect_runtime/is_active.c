#include "effect_runtime.h"

u32 EffectRuntime_IsActive(void)
{
    s16 count;
    u32 active = 0;
    struct EffectRuntime *runtime = EFFECT_RUNTIME;

    if (runtime != NULL) {
        count = *(s16 *)((u8 *)runtime + 0xcb8);
        active = (u32)((0 - count) | count);
        active >>= 31;
    }
    return active;
}
