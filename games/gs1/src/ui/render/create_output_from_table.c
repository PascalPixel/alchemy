#include "render_input.h"
#include "types.h"

s32 Resource_FindFreeSlot();
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
void *RenderOutput_CreateFromTable(
    s32 table_entry,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 slot;
    void *output;

    slot = Resource_FindFreeSlot();
    output = NULL;
    if (slot != 0x60) {
        RenderResource_LoadTableEntry(table_entry, 0, slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}
