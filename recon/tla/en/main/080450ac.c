#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
void Runtime_RemapBytesByTableFar(void *, s32);
u32 Resource_DecodeByteLz(const void *, void *);
void Runtime_ReleaseHeapBlock(s32);

/* graphics/resource/RenderResource_LoadFrame.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void VramBlock_LoadCached(s32, s32, void *);

void *RenderResource_CreateFrame(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Resource_FindFreeEntry();
    entity = NULL;
    if (index != 0x60) {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}
