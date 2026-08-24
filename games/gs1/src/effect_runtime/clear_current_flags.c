#include "effect_runtime.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_ClearCurrentFlags(void) {
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != NULL) {
            M2C_FIELD(object, s8 *, 0x54) = 0;
        }
    }
}
