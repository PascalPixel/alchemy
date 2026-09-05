#include "effect_runtime.h"

#define BattleEffect_HasTrigger Func_0808e990

s32 Func_0808e14c(u16);

u32 BattleEffect_HasTrigger(u16 effectId) {
    s32 result;

    result = Func_0808e14c(effectId);
    return (u32) ((0 - result) | result) >> 0x1F;
}
