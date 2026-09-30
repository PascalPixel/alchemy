#include "TYPES.H"

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Resource_DecodeByteLzInRam(void *source, void *destination);
s32 VramBlock_LoadCached(s32 entry_no, s32 mode, void *data);

extern s32 RenderResource_PairSourceTable[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    register s32 resource_address;
    void *staging_buffer;

    staging_buffer = Runtime_AllocateBlock(56, 0x400);
    if ((resource_address = RenderResource_PairSourceTable[group_index], resource_index <= 0x5F)) {
        Resource_DecodeByteLzInRam((void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(56);
    }
}
