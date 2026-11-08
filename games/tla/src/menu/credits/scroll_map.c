#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "display_scroll.h"

extern u32 Data_02007518;
extern u16 gData_02007520;
extern u32 gData_0200751c;

/// Copies 15 columns of a scrolling picture's map into a wrapping 2 KB
/// tilemap by DMA, 31 rows of 64 bytes each, from the row the position
/// selects, and records the position in the three scroll words. The
/// scrolling picture scene's callback (Func_081a133c) and its picture loader
/// (Func_081a15dc) call it.
void Func_081a1294(const u8 *source, u8 *destination, s32 position) {
  u32 start;
  s32 column;
  s32 row;
  u32 offset;
  u32 from;

  start = (Data_02007518 >> 19) << 6;
  gScrollTarget = gData_02007520 = position;
  {
    /* FAKEMATCH(pin): the word's address in r2, where the native loads it;
     * as a plain pointer with the barrier, 6 of 76 instructions differ in
     * tla-en. */
    register u32 *word asm("r2") = &gData_0200751c;

    /* FAKEMATCH(barrier): clobbering r1 loads the address before the
     * position is copied to r1 for its shift; as the pin alone, 4 of 76
     * instructions differ in tla-en. */
    __asm__("" : "+l"(word) : : "r1");
    *word = position << 16;
  }
  for (column = 0; column < 15; column++) {
    from = (position / 8) << 6;
    offset = start & 0x7ff;
    for (row = 0; row < 31; row++) {
      Dma_Set(source + from, destination + offset, DMA_COPY_WORDS(16),
              REG_DMA3);
      from += 64;
      offset = (offset + 64) & 0x7ff;
    }
    source += 0x1000;
    destination += 0x800;
  }
}
