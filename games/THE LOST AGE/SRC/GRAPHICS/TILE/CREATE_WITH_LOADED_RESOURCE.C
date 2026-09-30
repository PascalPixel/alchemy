#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"

s32 RenderOutput_CreateFar(s32, s32, s32, s32, s32);
void Ability_LoadGlyphFar(s32, s32, s32 *, s32 *, s32);

/* ☀️'s, but ⚓️ borrows heap block 0x44 and reaches the glyph loader far. */
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

s32 UiIcon_CreateWithLoadedResource(s32 x, s32 y, s32 z, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, x, y, z);
    }
}
