#include "TYPES.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"
u32 Resource_DecodeByteLz(const void *, void *);
void Runtime_ReleaseHeapBlock(s32);

/* graphics/resource/RenderOutput_LoadPair.c */
extern s32 RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    register s32 resource_address;
    void *staging_buffer;

    staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    if ((resource_address = RenderResource_PairSourceTable[group_index], resource_index <= 0x5F)) {
        Resource_DecodeByteLz((const void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}
