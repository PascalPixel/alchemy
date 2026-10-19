#include "resource.h"
#include "types.h"

#include "vram_tab.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

/// Frees a cache entry and the VRAM it holds; returns -1 for an index out of
/// range.
s32 Resource_ResetEntry(u32 resourceIndex) {
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[resourceIndex];
  u32 offset;

  if (resourceIndex >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  offset = entry->offset;
  /* FAKEMATCH: an empty asm reading the offset gives its load a second
   * dependent, so sched2 takes it inside the split constant 0xffff; plainly,
   * 2 instructions differ in tla-en. */
  asm("" : "+l"(entry) : "l"(offset));
  if (offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(resourceIndex);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

s32 VramCache_ReleaseVram(u32 resourceIndex) {
  u16 *entry = (u16 *)&gResourceTableEntries[resourceIndex];

  if (resourceIndex > 95)
    return -1;
  if (*entry > 16) {
    s32 value;

    VramCache_FreeBlocks(resourceIndex);
    value = 1;
    *entry = value;
  }
  return 0;
}

/// Loads size bytes from src into the object VRAM of cache entry no, reusing
/// its blocks when the size is unchanged (src 0 loads nothing, -1 clears);
/// returns the tile number, or 0 when it does not fit.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramBlockCacheEntry *entry;
  s32 offset;
  void *dst;

  {
    struct VramBlockCacheEntry *table = gResourceTableEntries;

    /* FAKEMATCH: two empty asms reading the address give its load two more
     * dependents, so it comes before the copy of no to r5; plainly, 2
     * instructions differ in tla-en. */
    asm("" : "+r"(src) : "l"(table));
    /* FAKEMATCH: the second of the pair above. */
    asm("" : "+l"(no) : "l"(table));
    entry = &table[no];
  }
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > 8192)
    return 0;
  if (entry->size > 16) {
    if (entry->size != size) {
      Resource_ResetEntry(no);
      offset = VramCache_AllocateBlocks(no, size);
    } else {
      offset = entry->offset;
    }
  } else {
    offset = VramCache_AllocateBlocks(no, size);
  }

  if (offset != -1) {
    dst = (void *)(0x06010000 + offset);
    entry->size = size;
    entry->offset = offset;
    if (src != 0) {
      if (src == (const void *)-1)
        Iwram_ClearWords(dst, size);
      else
        Dma_Set(src, dst, (size >> 2) | (DMA_ENABLE | DMA_32BIT) << 16,
                REG_DMA3);
    }
    return (u32)offset >> 5;
  }
  return 0;
}
