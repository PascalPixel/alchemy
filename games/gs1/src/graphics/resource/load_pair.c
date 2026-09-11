#include "types.h"
#include "scene.h"
#include "abi/graphics/resource/load_pair.h"


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
