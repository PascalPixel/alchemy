#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

#include "TYPES.H"
#include "EDITION.H"
#include "LAYOUT_GUARD.H"

enum VramBlockLimits {
    VRAM_CACHE_ENTRY_COUNT = 96,
    VRAM_BLOCK_COUNT = 512,
    VRAM_BLOCK_BYTES = 64,
    VRAM_BLOCK_OWNER_FREE = 0xff,
    VRAM_CACHE_OFFSET_FREE = 0xffff
};

extern u8 ResourceBlockOwners[VRAM_BLOCK_COUNT];

/* A resource cache record keeps its load size/state and VRAM byte offset. */
struct VramBlockCacheEntry {
    u16 size;
    u16 offset;
};

LAYOUT_SIZE_GUARD(VramBlockCacheEntry_Size, struct VramBlockCacheEntry, 4);
LAYOUT_OFFSET_GUARD(VramBlockCacheEntry_Offset, struct VramBlockCacheEntry, offset, 2);

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
extern struct VramBlockCacheEntry ResourceTableEntries[];
#else
extern struct VramBlockCacheEntry gVramBlockCache[];
#endif

#endif
