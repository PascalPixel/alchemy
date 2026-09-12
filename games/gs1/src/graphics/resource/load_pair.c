#include "types.h"
#include "scene.h"
#include "render_input.h"

/* graphics/resource/load_pair.c */
extern s32 gRom[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    register s32 resource_address;
    void *staging_buffer;

    staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    if ((resource_address = gRom[group_index], resource_index <= 0x5F)) {
        Sys_Apply((const void *)resource_address, staging_buffer);
        Resource_CopyData(resource_index, 0x200, staging_buffer);
        Sys_Do(14);
    }
}

/* graphics/resource/create_pair.c */
s32 Func_08004080(void);
void RenderResource_LoadPair(s32 arg0, s32 arg1);
void *RenderResource_CreatePair(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Func_08004080();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(arg0, index);
    first = RenderOutput_Create(index, 0x80004000, arg1, arg2, arg3);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
