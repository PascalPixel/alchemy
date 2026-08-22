#ifndef ALCHEMY_RESOURCE_390_TABLE_H
#define ALCHEMY_RESOURCE_390_TABLE_H

#include "types.h"

/* The 0x18-byte entries initialized by Resource390_InitRecords. */
struct Resource390TableEntry {
    u16 unknown_00;
    u8 unknown_02[2];
    s32 unknown_04;
    u8 unknown_08[0x0e];
    u8 unknown_16;
};

#define RESOURCE390_TABLE_ENTRY_OFFSET(field) \
    ((u32)&(((struct Resource390TableEntry *)0)->field))
typedef char Resource390TableEntry_unknown_04_offset[
    RESOURCE390_TABLE_ENTRY_OFFSET(unknown_04) == 0x04 ? 1 : -1
];
typedef char Resource390TableEntry_unknown_16_offset[
    RESOURCE390_TABLE_ENTRY_OFFSET(unknown_16) == 0x16 ? 1 : -1
];
typedef char Resource390TableEntry_size[
    sizeof(struct Resource390TableEntry) == 0x18 ? 1 : -1
];
#undef RESOURCE390_TABLE_ENTRY_OFFSET

#endif
