#include "effect_runtime.h"

s32 Func_080030f8(s32);
s32 Func_08009080(s32, s32);
s32 Func_0808ef70(s32, s32);
s32 Func_08092054(u32);
s32 Func_080f9010(s32);

s32 Func_0808e078(s32 effect, s32 target, s32 mode) {
    s32 object;
    s32 result;

    object = Func_08092054(target);
    result = 0;
    if (object != 0) {
        Func_080f9010(0x7C);
        Func_08009080(object, 4);
        Func_080030f8(0xC);
        result = Func_0808ef70(effect, mode);
    }
    return result;
}
