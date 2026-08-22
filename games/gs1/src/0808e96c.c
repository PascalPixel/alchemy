#include "effect_runtime.h"

s32 Func_0808e4b4(s32, u16, void *);

s32 Func_0808e96c(s32 effectId) {
    s32 local;
    s32 result = Func_0808e4b4(0x70000005, effectId, &local);
    return (u32)((-result) | result) >> 31;
}
