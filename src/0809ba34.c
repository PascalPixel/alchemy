#include "effect_0809b11c.h"

u32 Func_0809ba34(struct EffectSlot *effect) {
    u32 value;

    if (effect->flag41 == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}
