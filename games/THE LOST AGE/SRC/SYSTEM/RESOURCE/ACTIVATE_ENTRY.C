#include "TYPES.H"

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern struct ResourceTableEntry ResourceTableEntries[];

s32 Resource_ClearSlotReferences(s32 resource_id);

/* Marks one of the 96 resource entries in use, releasing its slot
   references first when it held a load state above 16. */
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
