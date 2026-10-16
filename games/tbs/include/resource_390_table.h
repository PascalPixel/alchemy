#ifndef ALCHEMY_RESOURCE_390_TABLE_H
#define ALCHEMY_RESOURCE_390_TABLE_H

#include "types.h"
#include "layout_guard.h"

struct Resource390TableEntry {
  u16 unk00;
  u8 unk02[2];
  s32 unk04;
  u8 unk08[0x0e];
  u8 unk16;
};

LAYOUT_OFFSET_GUARD(Resource390TableEntry_unknown_04_offset,
                    struct Resource390TableEntry, unk04, 0x04);
LAYOUT_OFFSET_GUARD(Resource390TableEntry_unknown_16_offset,
                    struct Resource390TableEntry, unk16, 0x16);
LAYOUT_SIZE_GUARD(Resource390TableEntry_size, struct Resource390TableEntry,
                  0x18);

#endif
