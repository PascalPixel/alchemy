#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08004080();
s32 UiIcon_DrawWithFlags(s32, s32, s32, s32, s32);
s32 UiIcon_LoadResourceIntoSlot(s32 arg0, s32 arg1);

s32 UiIcon_CreateWithLoadedResource(s32 x, s32 y, s32 z, s32 resource_id)
{
    s32 slot;

    slot = Func_08004080();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        UiIcon_DrawWithFlags(slot, 0x40000000, x, y, z);
    }
}
