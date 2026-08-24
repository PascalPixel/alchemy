#include "types.h"

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern struct ResourceTableEntry ResourceTableEntries[];
s32 Resource_ClearSlotReferences(s32 resource_id);

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *resource_entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95) {
        return -1;
    }
    if (resource_entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        resource_entry->flags |= 0xffff;
        resource_entry->value = 0;
    }
    return 0;
}
