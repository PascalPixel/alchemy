#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

#include "types.h"
#include "edition.h"
#include "static_assert.h"

enum VramBlockLimits {
  VRAM_CACHE_ENTRY_COUNT = 96,
  VRAM_BLOCK_COUNT = 512,
  VRAM_BLOCK_BYTES = 64,
  VRAM_BLOCK_OWNER_FREE = 0xff,
  VRAM_CACHE_OFFSET_FREE = 0xffff
};

extern u8 gResourceBlockOwners[VRAM_BLOCK_COUNT];

struct VramBlockCacheEntry {
  u16 size;
  u16 offset;
};

STATIC_ASSERT(sizeof(struct VramBlockCacheEntry) == 4);
STATIC_ASSERT(offsetof(struct VramBlockCacheEntry, offset) == 2);

#if GAME_TLA
extern struct VramBlockCacheEntry gResourceTableEntries[];
#else
extern struct VramBlockCacheEntry gVramBlockCache[];
#endif

#endif
