#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

/// @file
/// The VRAM cache: numbered entries, each holding a run of 64-byte blocks of
/// sprite VRAM for a sprite or a loaded picture.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

/// The VRAM cache's sizes: its entries, the 64-byte blocks of sprite VRAM it
/// hands out, and the markers of a free block and a free entry.
enum VramBlockLimits {
  VRAM_CACHE_ENTRY_COUNT = 96,
  VRAM_BLOCK_COUNT = 512,
  VRAM_BLOCK_BYTES = 64,
  VRAM_BLOCK_OWNER_FREE = 255,
  VRAM_CACHE_OFFSET_FREE = 65535,
  /// The largest size an entry has while it holds no blocks:
  /// VramCache_ReleaseVram leaves 1, which keeps the entry taken.
  VRAM_CACHE_SIZE_NO_BLOCKS = 16,
  VRAM_CACHE_LOAD_MAX = 8192 ///< the most bytes VramCache_Load takes at once
};

/// The window sprites' glyph tiles (WindowWork.glyphTiles), a cache entry
/// kept for this one use.
#define VRAM_CACHE_ENTRY_GLYPH_SHEET 95

/// The cache entry that owns each block, or VRAM_BLOCK_OWNER_FREE.
extern u8 gVramCacheBlockOwners[VRAM_BLOCK_COUNT];

/// A sprite slot of the VRAM cache: the size of its block and where the block
/// starts in sprite VRAM.
struct VramCacheEntry {
  u16 size;
  u16 offset;
};

STATIC_ASSERT(sizeof(struct VramCacheEntry) == 4);
STATIC_ASSERT(offsetof(struct VramCacheEntry, offset) == 2);

/// The cache entries by number: the slot a sprite or loaded picture holds.
extern struct VramCacheEntry gVramCacheEntries[];

#endif
