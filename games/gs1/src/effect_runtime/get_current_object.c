#include "effect_runtime.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

s32 EffectRuntime_GetCurrentObject(void) {
    s32 slot;

    slot = EffectRuntime_FindSlotByObject();
    if (slot == -1) {
        return 0;
    }
    { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; return *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
}
