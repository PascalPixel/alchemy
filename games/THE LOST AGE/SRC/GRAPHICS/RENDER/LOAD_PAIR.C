#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"

void Resource_DecodeByteLzInRam(void *source, void *destination);

extern s32 RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = Runtime_AllocateBlock(56, 0x400);
    s32 resource_address = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 0x5F) {
        Resource_DecodeByteLzInRam((void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(56);
    }
}
