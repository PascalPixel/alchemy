#include "effect_runtime.h"

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 Func_0808ef70(s32, s32);
s32 Object_GetById(u32);
s32 Audio_PlayCue(s32);

s32 Func_0808e078(s32 effect, s32 target, s32 mode) {
    s32 object;
    s32 result;

    object = Object_GetById(target);
    result = 0;
    if (object != 0) {
        Audio_PlayCue(0x7C);
        Object_SetMode(object, 4);
        WaitFrames(0xC);
        result = Func_0808ef70(effect, mode);
    }
    return result;
}
