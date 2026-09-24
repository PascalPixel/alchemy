#ifndef GUARD_VRAM_BLOCK_H
#define GUARD_VRAM_BLOCK_H

#include "TYPES.H"

struct VramBlockCacheEntry {
    u16 size;
    u16 offset;
};

extern struct VramBlockCacheEntry gVramBlockCache[];

#endif
