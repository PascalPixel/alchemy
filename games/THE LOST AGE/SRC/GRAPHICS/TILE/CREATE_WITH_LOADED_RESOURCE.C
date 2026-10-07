#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"


s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    u8 *allocation;

    allocation = Runtime_AllocateBlock(68, 1544);
    selected_slot = slot;
    Ability_LoadGlyphFar(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, (s32)(allocation + 1024));
    Runtime_ReleaseHeapBlock(68);
    return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}
