#include "types.h"

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 Data_03001810[];
extern struct ResourceTableEntry Data_03001b10[];

void Resource_InitializeTable(void)
{
    u32 limit = 0x1ff;
    u8 *occupancy_markers = Data_03001810;
    u32 count = 0;
    u32 empty_marker = 0xff;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct ResourceTableEntry *resource_entry = Data_03001b10;

        count = 0;
        do {
            resource_entry->flags |= 0xffff;
            resource_entry->value = 0;
            resource_entry++;
            count++;
        } while (count <= 95);
    }
}
