#include "types.h"
#include "resource.h"

struct MapTableEntry {
    u16 resource;
    u8 unknown_02[10];
};

extern const struct MapTableEntry Data_0802f380[];
extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size)
{
    s32 offset;
    const u8 *table = (const u8 *)Data_0802f380;
    u8 *header;

    offset = map << 1;

    /* FAKEMATCH: ⚓️ loads the table between the two halves of the row offset */
    asm volatile("" : "+l"(table), "+l"(offset));
    offset += map;
    offset <<= 2;
    header = Resource_GetTableEntry(*(u16 *)(offset + table) + (u32)&ResourceId_Map000);
    size[0] = header[0];
    if (header[2] == 0)
        size[1] = 256;
    else
        size[1] = header[2];
    size[2] = header[1];
    if (header[3] == 0)
        size[3] = 256;
    else
        size[3] = header[3];
}
