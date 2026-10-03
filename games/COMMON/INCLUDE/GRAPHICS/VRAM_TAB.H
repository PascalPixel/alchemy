#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* A resource cache record keeps its load size/state and VRAM byte offset. */
struct VramBlockCacheEntry {
    u16 size;
    u16 offset;
};

LAYOUT_SIZE_GUARD(VramBlockCacheEntry_Size, struct VramBlockCacheEntry, 4);
LAYOUT_OFFSET_GUARD(VramBlockCacheEntry_Offset, struct VramBlockCacheEntry, offset, 2);

extern struct VramBlockCacheEntry ResourceTableEntries[];

#endif
