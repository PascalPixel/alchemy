#ifndef ALCHEMY_RESOURCE_390_TABLE_H
#define ALCHEMY_RESOURCE_390_TABLE_H

#include "types.h"

struct Resource390TableEntry
{
    u16 unk00;
    u8  unk02[2];
    s32 unk04;
    u8  unk08[0x0e];
    u8  unk16;
};

#define RESOURCE390_TABLE_ENTRY_OFFSET(field) ((u32)&(((struct Resource390TableEntry *)0)->field))
typedef char
    Resource390TableEntry_unknown_04_offset[RESOURCE390_TABLE_ENTRY_OFFSET(unk04) == 0x04 ? 1 : -1];
typedef char
    Resource390TableEntry_unknown_16_offset[RESOURCE390_TABLE_ENTRY_OFFSET(unk16) == 0x16 ? 1 : -1];
typedef char Resource390TableEntry_size[sizeof(struct Resource390TableEntry) == 0x18 ? 1 : -1];
#undef RESOURCE390_TABLE_ENTRY_OFFSET

#endif
