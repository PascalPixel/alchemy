#include "types.h"

extern u8 Data_03001b10[];
s32 Resource_ClearSlotReferences(s32 resource_id);

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *resource_entry = (u16 *)(Data_03001b10 + resource_index * 4);

    if (resource_index > 95)
        return -1;
    if (*resource_entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *resource_entry = value;
    }
    return 0;
}
