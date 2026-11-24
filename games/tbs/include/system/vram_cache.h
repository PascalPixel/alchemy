#ifndef ALCHEMY_VRAM_CACHE_H
#define ALCHEMY_VRAM_CACHE_H

/// @file
/// The sprite VRAM block cache and its owners.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/// The sprite VRAM cache's sizes and its free marks. Sprite tile memory, from
/// OBJ_VRAM0, is split into blocks that the cache entries own.
enum VramCacheLimits {
  /// Cache entries; VramCache_FindFreeEntry returns this when none is free.
  VRAM_CACHE_ENTRY_COUNT = 96,
  VRAM_BLOCK_COUNT = 512, ///< blocks of sprite VRAM
  VRAM_BLOCK_BYTES = 64,
  VRAM_BLOCK_OWNER_FREE = 255, ///< gVramCacheBlockOwners of a free block
  /// VramCacheEntry.offset of a free entry.
  VRAM_CACHE_OFFSET_FREE = 65535,
  /// The largest size an entry has while it holds no blocks:
  /// VramCache_ReleaseVram leaves 1, which keeps the entry taken.
  VRAM_CACHE_SIZE_NO_BLOCKS = 16,
  /// The most bytes VramCache_Load takes at once: 128 blocks, a quarter of
  /// sprite VRAM.
  VRAM_CACHE_LOAD_MAX = 8192
};

/// The cache entries kept for one use each, loaded by number.
enum VramCacheFixedEntry {
  /// The actors' shared shadow (gActor_ShadowTiles), each sprite animation's
  /// part 1 (ActorSystem_Configure).
  VRAM_CACHE_ENTRY_ACTOR_SHADOW = 93,
  /// The ground mask's solid tiles (FieldEffect_StartGroundMask).
  VRAM_CACHE_ENTRY_GROUND_MASK = 94,
  /// The window sprites' glyph tiles (WindowState.glyphTiles).
  VRAM_CACHE_ENTRY_GLYPH_TILES = 95
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

/// VramCache_Load's source that clears the entry's VRAM instead of copying.
#define VRAM_CACHE_CLEAR ((const void *)-1)

#endif
