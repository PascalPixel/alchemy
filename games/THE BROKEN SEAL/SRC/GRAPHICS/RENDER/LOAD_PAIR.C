#include "TYPES.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"

/* graphics/resource/RenderOutput_LoadPair.c */
extern s32 gRom[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    register s32 resource_address;
    void *staging_buffer;

    staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    if ((resource_address = gRom[group_index], resource_index <= 0x5F)) {
        Resource_DecodeByteLz((const void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

/* graphics/resource/RenderOutput_CreatePair.c */
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

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(arg0, index);
    /* Two 32x16 OBJ sprites; the right half starts eight tiles later. */
    first = RenderOutput_Create(index, 0x80004000, arg1, arg2, arg3);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
