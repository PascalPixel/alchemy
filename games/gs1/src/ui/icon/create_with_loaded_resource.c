#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Resource_FindFreeSlot();
s32 UiIcon_DrawWithFlags(s32, s32, s32, s32, s32);
s32 UiIcon_LoadResourceIntoSlot(s32 arg0, s32 arg1);

s32 UiIcon_CreateWithLoadedResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;

    temp_r0 = Resource_FindFreeSlot();
    if (temp_r0 != 0x60) {
        UiIcon_LoadResourceIntoSlot(arg3, temp_r0);
        UiIcon_DrawWithFlags(temp_r0, 0x40000000, arg0, arg1, arg2);
    }
}
