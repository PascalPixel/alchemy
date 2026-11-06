// Matches: differ 0 of 59 (tla-en, 2026-11-06).
// ☀️'s VramCache_AllocateBlocks with ⚓️'s bound on the run's end. Every
// access goes through gResourceBlockOwners and both failures return -1 in
// place (no shared ret = -1); the scan's bound is ret + blocks, which loop.c
// hoists into ip. Earlier forms through an owners local, or with a shared
// -1 result, measured 57 to 74.
#include "resource.h"
#include "vram_tab.h"

s32 VramCache_AllocateBlocks(u32 slot, u32 size) {
  u32 blocks;
  s32 ret;
  s32 pos;
  u32 i;

  blocks = size / VRAM_BLOCK_BYTES;
  if (slot >= VRAM_CACHE_ENTRY_COUNT) {
    return -1;
  }
  pos = 0;
  for (;;) {
    if (pos >= VRAM_BLOCK_COUNT) {
      return -1;
    }
    if (gResourceBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      if (pos + blocks > VRAM_BLOCK_COUNT) {
        return -1;
      }
      ret = pos;
      while (pos < ret + blocks) {
        if (gResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gResourceBlockOwners[ret + i] = slot;
      }
      break;
    }
  occupied:
    pos += (u32)gResourceTableEntries[gResourceBlockOwners[pos]].size /
           VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}
