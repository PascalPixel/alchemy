#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "resource.h"
#include "render_input.h"
#include "system.h"
#include "ram_buffer.h"

struct ItemMenuBlocks {
    u8 unknown_000[0x4ee];
    s16 resource_slots[2];
};

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;


void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    struct ItemMenuBlocks *active_state;

    active_state = Ram_HeapSlots->menu_runtime;
    first_slot = Resource_FindFreeEntry();
    active_state->resource_slots[0] = (s16)first_slot;
    if (first_slot != -1) {
        VramBlock_LoadResourceFar(first_slot, 0x80, (s32)&ResourceId_ItemMenuTilesA);
    }
    second_slot = Resource_FindFreeEntry();
    active_state->resource_slots[1] = (s16)second_slot;
    if (second_slot != -1) {
        VramBlock_LoadResourceFar(second_slot, 0x80, (s32)&ResourceId_ItemMenuTilesB);
    }
}

void Menu_ResetTwoResourceEntries(void)
{
    struct ItemMenuBlocks *state = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: ⚓️ loads the state before it builds the slot offset */
    asm volatile("" : "+l"(state));
    Resource_ResetEntry((u16)state->resource_slots[0]);
    Resource_ResetEntry((u16)state->resource_slots[1]);
}

s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    u8 *allocation;

    allocation = Runtime_AllocateBlock(0x44, 0x608);
    selected_slot = slot;
    Ability_LoadGlyphFar(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, (s32)(allocation + 0x400));
    Runtime_ReleaseHeapBlock(0x44);
    return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}
