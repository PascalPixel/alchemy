#include "render_input.h"
#include "types.h"
#include "scene.h"
#include "abi/ui/render/create_output_from_resource_slot.h"


s32 Resource_FindFreeSlot();

s32 Ui_CreateOutputFromResourceSlot(
    struct RenderInput *input,
    s32 arg1,
    s32 arg2,
    s32 resource)
{
    s32 slot;

    slot = Resource_FindFreeSlot();
    if (slot != 0x60) {
        Ui_Apply(resource, slot);
        Ui_Run(slot, 0x40000000, input, arg1, arg2);
    }
}
