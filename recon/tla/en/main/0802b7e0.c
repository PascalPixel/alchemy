#include "types.h"
#include "resource.h"

struct MapTableEntry {
    u16 resource;
    u8 unknown_02[10];
};

extern const struct MapTableEntry Data_0802f380[];
extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *result)
{
    u8 *header;
    s32 offset;
    u32 table;
    s32 *size;

    /* FAKEMATCH: the one-pass block ends the scheduling block: the native loads the table between the two halves of the row offset (one reordered row, differ 2 as map * 12 + table). */
    do {
        offset = map << 1;
        table = (u32)Data_0802f380;
    } while (0);
    offset += map;
    size = result;

    header = Resource_GetTableEntry(*(const u16 *)(offset * 4 + table) + (u32)&ResourceId_Map000);
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
