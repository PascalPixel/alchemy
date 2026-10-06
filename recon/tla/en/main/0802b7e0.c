#include "TYPES.H"
#include "RESOURCE.H"

struct MapTableEntry {
    u16 resource;
    u8 unknown_02[10];
};

extern const struct MapTableEntry Data_0802f380[];
extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size)
{
    u8 *header = Resource_GetTableEntry(
        PERM_GENERAL(Data_0802f380[map].resource, (map + Data_0802f380)->resource, ((u16 *)Data_0802f380)[map * 6], *(u16 *)((u8 *)Data_0802f380 + map * 12), *(u16 *)(map * 12 + (u8 *)Data_0802f380), (&Data_0802f380[map])->resource, *(&Data_0802f380[map].resource), ((const struct MapTableEntry *)((s32)Data_0802f380 + map * 12))->resource, ((const struct MapTableEntry *)(map * 12 + (s32)Data_0802f380))->resource) + (u32)&ResourceId_Map000);

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
