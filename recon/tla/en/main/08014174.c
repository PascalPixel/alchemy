// Measured 2026-11-05 (tla-en): differ 57 of 59, from the twin's shape with blocks, the second -1, and the scan through owners with the first test through the global (the scan then keeps its walker r2; with both through owners CSE drops the first read). Left: allocation: id r6 where the native has r5, blocks r5 where r1, pos r1 where r4, owners r7 and the hoisted table in ip.
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
  u32 blocks;
  s32 ret;
  s32 pos;
  u32 end;
  u32 i;

  blocks = size / VRAM_BLOCK_BYTES;
  if (id >= VRAM_CACHE_ENTRY_COUNT) {
    return -1;
  }
  owners = gResourceBlockOwners;
  pos = 0;
  for (;;) {
    ret = -1;
    if (pos >= VRAM_BLOCK_COUNT) {
      return ret;
    }
    if (gResourceBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      end = blocks + pos;
      ret = -1;
      if (end > VRAM_BLOCK_COUNT)
        return ret;
      ret = pos;
      while (pos < end) {
        if (owners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        owners[ret + i] = id;
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
