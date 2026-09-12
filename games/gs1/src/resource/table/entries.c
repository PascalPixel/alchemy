#include "types.h"

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 ResourceBlockOwners[];
extern struct ResourceTableEntry ResourceTableEntries[];

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id > 0x5f)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = 0xff;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        entry->flags |= 0xffff;
        entry->value = 0;
    }
    return 0;
}

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *entry = (u16 *)&ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (*entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *entry = value;
    }
    return 0;
}
