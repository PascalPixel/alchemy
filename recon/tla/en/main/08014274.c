#include "RESOURCE.H"
#include "TYPES.H"

extern u8 ResourceBlockOwners[];

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
