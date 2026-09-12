#include "types.h"
#include "scene.h"
#include "resource.h"
#include "render_input.h"

/* graphics/resource/load_frame.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Resource_CopyData(s32, s32, void *);

extern unsigned char gVal[];

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = GetResource((s32)gVal);

    if (value <= 95) {
        Sys_Apply((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Sys_Apply2(buffer, 768);
        Resource_CopyData(value, size, buffer);
        Sys_Do(14);
    }
}

/* graphics/resource/create_frame.c */
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
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}
