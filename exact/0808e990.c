#include "effect_runtime.h"

s32 Func_0808e14c(u16);

u32 Func_0808e990(u16 effectId) {
    s32 result;

    result = Func_0808e14c(effectId);
    return (u32) ((0 - result) | result) >> 0x1F;
}
