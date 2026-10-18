// Draft, differ 2 of 76: copies 15 columns of a map into a wrapping 2 KB
// tilemap by DMA, 31 rows of 64 bytes each, from the row the position
// selects, and records the position in the three scroll words. Left:
// instruction order only: the native copies the position (mov r1, r8) for
// its shift ahead of the last word's pool load, the draft after it. The two
// halfword words are one chained assignment (written apart, differ 14), and
// the source step comes before the wrapped offset's (the other way, differ 4).
#include "types.h"
#include "dma.h"
#include "display_scroll.h"

extern u32 Data_02007518;
extern u16 gData_02007520;
extern u32 gData_0200751c;

void Func_081a1294(const u8 *source, u8 *destination, s32 position) {
  u32 start;
  s32 column;
  s32 row;
  u32 offset;
  u32 from;

  start = (Data_02007518 >> 19) << 6;
  gData_02007520 = gScrollTarget = position;
  gData_0200751c = position << 16;
  for (column = 0; column < 15; column++) {
    from = (position / 8) << 6;
    offset = start & 0x7ff;
    for (row = 0; row < 31; row++) {
      Dma_Set(source + from, destination + offset, 0x84000010,
              (volatile u32 *)0x040000d4);
      from += 64;
      offset = (offset + 64) & 0x7ff;
    }
    source += 0x1000;
    destination += 0x800;
  }
}
