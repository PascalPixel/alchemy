// Draft, ARM C (game ARM route, GCC 2.95.2), entered through the Thumb
// veneer Func_08021824 (bx pc): the listing is mixed-mode, which psynergy
// diff cannot compare (it decodes only the two Thumb halfwords). Measured
// instead by disassembling the listing's ARM words and the --arm draft's
// object with objdump and diffing the rows: 60 instructions each, 56 rows
// differ (28 a side), mostly register names. Marks the inner cells of a
// width by height byte grid whose four neighbours are all set (neither 0
// nor 255) as 254, or 255 when empty, then turns every set cell into edge
// (above 253) or inner. No ☀️ twin found. Left: the native's cell pointer
// is a pre-incremented giv (ldrb r3, [r2, #1]!, the neighbours at ~width
// and width - 1 from it) where this draft's pointer is a biv stepped at
// the loop end, and the native holds edge in r9 and the size in sl (7 saved
// registers to 5). As an index into the grid, 60 rows differ; with u8
// arguments the values are masked (and r8, r3, #255).
#include "types.h"

void Func_08021826(u32 width, u32 height, u8 *grid, s32 inner, s32 edge) {
  u32 size = width * height;
  u8 *cell = grid + width + 1;
  u8 *end;
  u32 x;
  u32 y;
  u32 value;

  for (y = 1; y < height - 1; y++, cell += 2) {
    for (x = 1; x < width - 1; x++, cell++) {
      value = cell[1];
      if (value == 0 || value == 255)
        continue;
      value = cell[-1];
      if (value == 0 || value == 255)
        continue;
      value = cell[-width];
      if (value == 0 || value == 255)
        continue;
      value = cell[width];
      if (value == 0 || value == 255)
        continue;
      if (*cell != 0)
        *cell = 254;
      else
        *cell = 255;
    }
  }
  cell = grid;
  end = grid + size;
  while (cell != end) {
    value = *cell++;
    if (value == 0)
      continue;
    if (value > 253)
      cell[-1] = edge;
    else
      cell[-1] = inner;
  }
}
