#include "effect_runtime.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(void *, s32);
s32 Func_080091e0(void *, s32);
s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode7AndLaunch(void) {
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        WaitFrames(0x12);
        if (object != NULL) {
            Object_SetMode(object, 7);
        }
        Audio_PlayCue(0x92);
        if (object != NULL) {
            M2C_FIELD(object, s32 *, 0x28) = 0x80000;
            Func_080091e0(object, 1);
        }
    }
}
