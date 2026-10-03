#ifndef GUARD_VRAM_BLOCK_H
#define GUARD_VRAM_BLOCK_H

#include "TYPES.H"
#include "RESOURCE.H"
#include "LAYOUT_GUARD.H"

enum VramBlockLimits {
    VRAM_CACHE_ENTRY_COUNT = 96,
    VRAM_BLOCK_COUNT = 512,
    VRAM_BLOCK_BYTES = 64,
    VRAM_BLOCK_OWNER_FREE = 0xff,
    VRAM_CACHE_OFFSET_FREE = 0xffff
};

struct VramBlockCacheEntry {
    u16 size;
    u16 offset;
};

LAYOUT_SIZE_GUARD(VramBlockCacheEntry_Size, struct VramBlockCacheEntry, 4);
LAYOUT_OFFSET_GUARD(VramBlockCacheEntry_Offset, struct VramBlockCacheEntry, offset, 2);

extern struct VramBlockCacheEntry gVramBlockCache[];
extern u8 ResourceBlockOwners[VRAM_BLOCK_COUNT];

/* Loads a table graphic into a VRAM cache slot, not a RAM destination. */
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot);

#endif
