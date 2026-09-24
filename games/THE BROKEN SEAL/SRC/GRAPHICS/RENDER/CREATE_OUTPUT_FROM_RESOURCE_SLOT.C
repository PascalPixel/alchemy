#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RESOURCE.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Resource_LoadIndexedEntryToBuffer(s32 input, s32 arg1);

s32 Ui_CreateOutputFromResourceSlot(
    struct RenderInput *input,
    s32 arg1,
    s32 arg2,
    s32 resource)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        Resource_LoadIndexedEntryToBuffer(resource, slot);
        RenderOutput_Create(slot, 0x40000000, input, arg1, arg2);
    }
}
