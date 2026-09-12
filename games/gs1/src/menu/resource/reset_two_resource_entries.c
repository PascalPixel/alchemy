#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* menu/res/reset_two_resource_entries.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Resource_ResetEntry(u16);

void Menu_ResetTwoResourceEntries(void)
{
    void *state;

    state = *(void **)ADDR_03001F2C;
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x392));
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x394));
}

/* ui/icon/icon_load_resource_into_slot.c */
/* ui/icon/load_resource_into_slot.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    s32 allocation;

    allocation = Runtime_AllocateBlock(0x11, 0x608);
    selected_slot = slot;
    Ui_SetRange(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, allocation + 0x400);
    Ui_Check(0x11);
    return buffer;
}

/* ui/icon/create_with_loaded_resource.c */
s32 Resource_FindFreeSlot();
s32 UiIcon_DrawWithFlags(s32, s32, s32, s32, s32);
s32 UiIcon_LoadResourceIntoSlot(s32 arg0, s32 arg1);

s32 UiIcon_CreateWithLoadedResource(s32 x, s32 y, s32 z, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeSlot();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        UiIcon_DrawWithFlags(slot, 0x40000000, x, y, z);
    }
}
