#include "effect_runtime.h"

s32 Func_0808e4b4(s32, s32, void *);

s32 BattleEffect_HasMatchingEvent5(s32 effectId) {
    s32 local;
    s32 result = Func_0808e4b4(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}
