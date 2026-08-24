#include "types.h"

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 ResourceBlockOwners[];
extern struct ResourceTableEntry ResourceTableEntries[];

void Resource_InitializeTable(void)
{
    u32 limit = 0x1ff;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = 0xff;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct ResourceTableEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->flags |= 0xffff;
            resource_entry->value = 0;
            resource_entry++;
            count++;
        } while (count <= 95);
    }
}
