#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

/// @file
/// The sprite VRAM block cache and its owners.

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

/// A sprite slot of the VRAM cache: the size of its block and where the block
/// starts in OBJ VRAM.
struct VramBlockCacheEntry {
  u16 size;   ///< bytes
  u16 offset; ///< bytes from the start of OBJ VRAM
};

STATIC_ASSERT(sizeof(struct VramBlockCacheEntry) == 4);
STATIC_ASSERT(offsetof(struct VramBlockCacheEntry, offset) == 2);

extern struct VramBlockCacheEntry gVramBlockCache[];

#endif
