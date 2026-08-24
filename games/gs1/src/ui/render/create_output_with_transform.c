#include "render_input.h"
#include "types.h"

s32 Resource_FindFreeSlot(void);
void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    u8 *result;

    count = Resource_FindFreeSlot();
    if (count == 0x60) {
        return NULL;
    }
    Func_08019ee4(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result[15] = 251;
    return result;
}
