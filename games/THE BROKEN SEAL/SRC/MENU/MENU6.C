#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

extern u8 Data_03001f2c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
extern s32 Resource_FindFreeEntry(void);
extern s32 VramBlock_LoadCached(s32, s32, u8 *);
extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

s32 Func_080153d0(s32, s32, s32 *, s32 *, s32);
s32 Runtime_ReleaseHeapBlock(s32);
s32 Resource_ResetEntry(u16);

/* ui/icon/load_resource_into_slot.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    void *active_state;

    active_state = *(void **)((u32)&Data_03001f2c);
    first_slot = Resource_FindFreeEntry();
    FIELD_AT_OFFSET(active_state, s16, 0x392) = (s16)first_slot;
    if (first_slot != -1) {
        VramBlock_LoadCached(first_slot, 0x80, Data_080aed4c);
    }
    second_slot = Resource_FindFreeEntry();
    FIELD_AT_OFFSET(active_state, s16, 0x394) = (s16)second_slot;
    if (second_slot != -1) {
        VramBlock_LoadCached(second_slot, 0x80, Data_080aedcc);
    }
}

/* menu/res/reset_two_resource_entries.c */
void Menu_ResetTwoResourceEntries(void)
{
    void *state;

    state = *(void **)((u32)&Data_03001f2c);
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x392));
    Resource_ResetEntry(FIELD_AT_OFFSET(state, u16 *, 0x394));
}

/* ui/icon/icon_load_resource_into_slot.c */
s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    s32 allocation;

    allocation = Runtime_AllocateBlock(0x11, 0x608);
    selected_slot = slot;
    Func_080153d0(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, allocation + 0x400);
    Runtime_ReleaseHeapBlock(0x11);
    return buffer;
}
