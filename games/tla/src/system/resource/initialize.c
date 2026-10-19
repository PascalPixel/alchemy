#include "types.h"
#include "resource.h"
#include "vram_tab.h"

/// Frees every VRAM block and every cache entry.
void VramCache_Reset(void) {
  u32 count;
  /* FAKEMATCH: ⚓️ loads the block owners' address between the first two
   * instructions of the split limit 511; plainly the shift comes first (differ
   * 2), since sched2 ranks the load and the shift equal and takes the shift.
   * The limit's first part is pinned to the r0 the native builds it in, and a
   * barrier holds it until the address is loaded; as a free temporary the limit
   * takes r3 and the address r2 (differ 16). */
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
    u32 offset = entry->offset;
    /* FAKEMATCH: ⚓️ reads the entry's offset between the first two instructions
     * of the split constant 0xffff; plainly the shift comes first (differ 4,
     * once in each copy of the test). The constant's first part is pinned to
     * the r4 the native builds it in (as a free temporary the loop takes other
     * registers, differ 32), and an empty asm that also reads the offset ranks
     * the load ahead of the shift. */
    register u32 free asm("r4") = 0xff00;

    /* FAKEMATCH: the barrier of the pin above. */
    asm("" : "+l"(free) : "l"(offset));
    if (offset == free + 0xff) {
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
