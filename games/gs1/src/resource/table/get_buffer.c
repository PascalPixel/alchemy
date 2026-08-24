#include "types.h"

extern u16 ResourceTableEntries[];
s32 Resource_CopyData(s32, u16, s32);

s32 Resource_GetBuffer(s32 resource_index, s32 copy_value)
{
    return Resource_CopyData(resource_index, ResourceTableEntries[resource_index * 2], copy_value);
}
