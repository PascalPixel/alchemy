#include "TYPES.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"

s32 UiIcon_LoadResourceIntoSlot(s32 arg0, s32 arg1);

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}
