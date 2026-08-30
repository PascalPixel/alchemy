#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
u32 Func_080053e8(const void *, void *);
extern s32 Data_08031864[];

void RenderResource_LoadPair(s32 group_index, s32 resource_index) {
    register s32 resource_address;
    void *staging_buffer;

    staging_buffer = (void *)Runtime_AllocateBlock(14, 0x400);
    if ((resource_address = Data_08031864[group_index], resource_index <= 0x5F)) {
        Func_080053e8((const void *)resource_address, staging_buffer);
        Resource_CopyData(resource_index, 0x200, staging_buffer);
        Func_08002dd8(14);
    }
}
