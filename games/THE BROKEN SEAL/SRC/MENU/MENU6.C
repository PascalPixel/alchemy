#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "INVENTORY_MENU.H"

extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

s32 Func_080153d0(s32, s32, s32 *, s32 *, s32);

/* ui/icon/load_resource_into_slot.c */

void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    struct InventoryMenuState *active_state;

    active_state = gMenuWork;
    first_slot = Resource_FindFreeEntry();
    active_state->resource_slots[0] = (s16)first_slot;
    if (first_slot != -1) {
        VramBlock_LoadCached(first_slot, 0x80, Data_080aed4c);
    }
    second_slot = Resource_FindFreeEntry();
    active_state->resource_slots[1] = (s16)second_slot;
    if (second_slot != -1) {
        VramBlock_LoadCached(second_slot, 0x80, Data_080aedcc);
    }
}

/* menu/res/reset_two_resource_entries.c */
void Menu_ResetTwoResourceEntries(void)
{
    struct InventoryMenuState *state;

    state = gMenuWork;
    Resource_ResetEntry((u16)state->resource_slots[0]);
    Resource_ResetEntry((u16)state->resource_slots[1]);
}

/* ui/icon/icon_load_resource_into_slot.c */
s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    s32 allocation;

    allocation = (s32)Runtime_AllocateBlock(0x11, 0x608);
    selected_slot = slot;
    Func_080153d0(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, allocation + 0x400);
    Runtime_ReleaseHeapBlock(0x11);
    return buffer;
}
