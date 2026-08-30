#include "types.h"

s32 Func_08002dd8(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_080153d0(s32, s32, s32 *, s32 *, s32);

s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot) {
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    s32 allocation;

    allocation = Runtime_AllocateBlock(0x11, 0x608);
    selected_slot = slot;
    Func_080153d0(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, allocation + 0x400);
    Func_08002dd8(0x11);
    return buffer;
}
