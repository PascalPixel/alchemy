#include "types.h"
#include "resource.h"
#include "vram_tab.h"

/// Frees every VRAM block and every cache entry.
void VramCache_Reset(void) {
  u32 count;
  /* FAKEMATCH: the split limit 511 pinned to r0 and held by a barrier, so the
   * block owners' address loads between its two instructions; as a plain
   * constant, 2 instructions differ in tla-en. */
  register u32 limit asm("r0") = 128;
  u8 *marker = gResourceBlockOwners;

  /* FAKEMATCH: the barrier of the pin above. */
  asm volatile("" : "+l"(limit));
  count = 0;
  limit = (limit << 1) + 255;
  for (; count <= limit; count++)
    *marker++ = VRAM_BLOCK_OWNER_FREE;
  {
    struct VramBlockCacheEntry *entry = gResourceTableEntries;

    count = 0;
    do {
      entry->offset |= VRAM_CACHE_OFFSET_FREE;
      entry->size = 0;
      entry++;
      count++;
    } while (count < VRAM_CACHE_ENTRY_COUNT);
  }
}

/// Returns the first free cache entry, or VRAM_CACHE_ENTRY_COUNT when all are
/// in use.
s32 Resource_FindFreeEntry(void) {
  s32 ret = VRAM_CACHE_ENTRY_COUNT;
  s32 no;
  struct VramBlockCacheEntry *entry = gResourceTableEntries;

  for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++) {
    if (entry->offset == VRAM_CACHE_OFFSET_FREE) {
      ret = no;
      break;
    }
  }
  return ret;
}

s32 VramCache_AllocEntry(s32 size) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  VramCache_Load(slot, size, 0);
  return slot;
}
