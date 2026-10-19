// Draft, differ 68 over 59 instructions (was 74): ☀️'s VramCache_AllocateBlocks
// with ⚓️'s bound on the run's end. The shape matches; the registers do not:
// the native keeps the block count in r1, the id in r5 and the position in r4,
// with the owner table twice (r7, and a copy in r6 the first test and the
// scan read). Reading the owners through a local for the first test and the
// scan gives the two copies (74 to 68). Measured: two return -1 exits in
// place of the shared result (72); four minutes of permuter reached 37 only
// by assigning the scanned owner byte to the run's end.
#include "resource.h"
#include "vram_tab.h"

s32 VramCache_AllocateBlocks(u32 id, u32 size) {
  u8 *owners;
  s32 result;
  s32 pos;
  u32 end;
  u32 i;

  size /= VRAM_BLOCK_BYTES;
  if (id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  owners = gResourceBlockOwners;
  pos = 0;
  for (;;) {
    result = -1;
    if (pos >= VRAM_BLOCK_COUNT)
      goto done;
    if (owners[pos] != VRAM_BLOCK_OWNER_FREE)
      goto occupied;
    end = pos + size;
    if (end > VRAM_BLOCK_COUNT)
      goto done;
    result = pos;
    while (pos < end) {
      if (owners[pos] != VRAM_BLOCK_OWNER_FREE)
        goto occupied;
      pos++;
    }
    for (i = 0; i < size; i++)
      gResourceBlockOwners[result + i] = id;
    goto found;
  occupied:
    pos += (u32)gResourceTableEntries[gResourceBlockOwners[pos]].size /
           VRAM_BLOCK_BYTES;
  }
found:
  result *= VRAM_BLOCK_BYTES;
done:
  return result;
}
