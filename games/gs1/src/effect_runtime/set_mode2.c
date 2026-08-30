#include "effect_runtime.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetMode2(void) {
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 2);
        }
    }
}
