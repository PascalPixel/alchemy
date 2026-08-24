#include "render_input.h"
#include "types.h"

s32 Resource_FindFreeSlot();
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
void *RenderOutput_CreateFromTable(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 temp_r0;
    void *var_r0;

    temp_r0 = Resource_FindFreeSlot();
    var_r0 = NULL;
    if (temp_r0 != 0x60) {
        RenderResource_LoadTableEntry(arg0, 0, temp_r0);
        var_r0 = RenderOutput_Create(temp_r0, 0x40000000, arg1, arg2, arg3);
    }
    return var_r0;
}
