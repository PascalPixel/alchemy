#ifndef ALCHEMY_RESOURCE_390_TABLE_H
#define ALCHEMY_RESOURCE_390_TABLE_H

#include "types.h"
#include "static_assert.h"

struct Resource390TableEntry {
  u16 unk00;
  u8 unk02[2];
  s32 unk04;
  u8 unk08[0x0e];
  u8 unk16;
};

STATIC_ASSERT(offsetof(struct Resource390TableEntry, unk04) == 0x04);
STATIC_ASSERT(offsetof(struct Resource390TableEntry, unk16) == 0x16);
STATIC_ASSERT(sizeof(struct Resource390TableEntry) == 0x18);

#endif
