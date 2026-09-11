#include "render_input.h"
#include "types.h"
#include "scene.h"
#include "abi/ui/render/create_output_loaded.h"

s32 Resource_FindFreeSlot();

void *RenderOutput_CreateLoaded(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;
    void *result;

    no = Resource_FindFreeSlot();
    result = NULL;
    if (no != 0x60) {
        Ui_Place(arg0, arg1, no);
        result = RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
    }
    return result;
}
