#ifndef ALCHEMY_VRAM_CACHE_H
#define ALCHEMY_VRAM_CACHE_H

/// @file
/// The sprite VRAM block cache and its owners.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

/// The sprite VRAM cache's sizes and its free marks. Sprite tile memory, from
/// OBJ_VRAM0, is split into blocks that the cache entries own.
enum VramCacheLimits {
  /// Cache entries; VramCache_FindFreeEntry returns this when none is free.
  VRAM_CACHE_ENTRY_COUNT = 96,
  VRAM_BLOCK_COUNT = 512, ///< blocks of sprite VRAM
  VRAM_BLOCK_BYTES = 64,
  VRAM_BLOCK_OWNER_FREE = 0xff, ///< gVramCacheBlockOwners of a free block
  /// VramCacheEntry.offset of a free entry.
  VRAM_CACHE_OFFSET_FREE = 0xffff
};

/// The cache entry that owns each block, or VRAM_BLOCK_OWNER_FREE.
extern u8 gVramCacheBlockOwners[VRAM_BLOCK_COUNT];

/// A sprite slot of the VRAM cache: the size of its block and where the block
/// starts in sprite VRAM.
struct VramCacheEntry {
  u16 size;   ///< bytes
  u16 offset; ///< bytes from the start of sprite VRAM
};

STATIC_ASSERT(sizeof(struct VramCacheEntry) == 4);
STATIC_ASSERT(offsetof(struct VramCacheEntry, offset) == 2);

/// The cache entries by number: the slot a sprite or loaded picture holds.
extern struct VramCacheEntry gVramCacheEntries[];

#endif
