#include "render_input.h"
#include "types.h"
#include "scene.h"
#include "abi/graphics/resource/create_frame.h"

s32 Resource_FindFreeSlot(void);
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
void *RenderResource_CreateFrame(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Resource_FindFreeSlot();
    entity = NULL;
    if (index != 0x60) {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = Sys_Run(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}
