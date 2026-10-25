// Draft, differ 147 of 99: fills a 64 by 64 grid of levels, each ring a
// quarter circle drawn with the midpoint algorithm, its level from the cosine
// of its radius. Left: the native recomputes count - 1 inside the loop (this
// draft hoists it) and keeps the frame at 4 bytes; register choice follows.
#include "types.h"
#include "iwram_call.h"

s32 Math_Cosine(s32 angle);

void Func_02000138(u8 *grid, s32 count, s32 scale) {
  s32 i;

  Iwram_FillWords(grid, 0x1000, 0x01010101);
  for (i = 0; i != count - 1; i++) {
    s32 level =
        ((Math_Cosine((i << 14) / (count - 1)) * 15) >> 16) * scale / 0x10000;
    s32 x, y, error;

    if (level > 15)
      level = 15;
    if (level <= 0)
      level = 1;
    x = i;
    error = i;
    for (y = 0; x >= y; y++) {
      s32 column = 63 - x;
      s32 row = 63 - y;
      u8 *cell = &grid[row * 64 + column];

      cell[0] = level;
      if (column < 63)
        cell[1] = level;
      cell = &grid[column * 64 + row];
      cell[0] = level;
      if (row < 63)
        cell[1] = level;
      error = error - y * 2 - 1;
      if (error < 0) {
        error = error + x * 2 - 2;
        x--;
      }
    }
  }
}
