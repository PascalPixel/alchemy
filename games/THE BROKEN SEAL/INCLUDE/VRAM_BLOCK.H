#ifndef GUARD_VRAM_BLOCK_H
#define GUARD_VRAM_BLOCK_H

#include "TYPES.H"
#include "RESOURCE.H"
#include "VRAM_TAB.H"

/* Loads a table graphic into a VRAM cache slot, not a RAM destination. */
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot);

#endif
