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
