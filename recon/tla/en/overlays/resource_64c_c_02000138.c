// Draft, differ 71 of 99 (tla-en, 5 November). Fills a 64 by 64 grid of levels, each ring a quarter circle drawn with the
// midpoint algorithm, its level from the cosine of its radius.
// The register rotation is gone: row set to 63 and then lowered by y stops GCSE merging the second 63 - x, as the native rebuilds it.
// Left: the native loads 63 for the second row only after the first test (cmp r0, #63); here edge's 63 stays in a register
// from before that compare, and takes r6, so x and the error rotate to r4, r5 from r5, r6, and y's zero, reloaded through r6, floats past the calls.
// A literal 63 in the compare folds to cmp #62; column + 1 < 64 (100); column - 63 < 0 (97); size - 1 from a local (75).
#include "types.h"
#include "call.h"
#include "iwram_call.h"

s32 Math_Cosine(s32 angle);

void Func_02000138(u8 *grid, s32 count, s32 scale) {
  s32 i = 0;

  Iwram_FillWords(grid, 0x1000, 0x01010101);
  if (count == 1)
    return;
  do {
    s32 y = 0;
    s32 last = count - 1;
    s32 level = (Math_Cosine((i << 14) / last) * 15) >> 16;
    s32 x = i;
    s32 error = i;

    level = scale * level / 0x10000;
    if (level > 15)
      level = 15;
    if (level <= 0)
      level = 1;
    while (x >= y) {
      s32 row = 63;
      s32 column = row - x;
      u8 *cell;
      s32 edge;
      row -= y;
      cell = &grid[row * 64 + column];
      cell[0] = level;
      edge = 63;
      if (column < edge)
        cell[1] = level;
      column = row;
      row = 63 - x;
      cell = &grid[row * 64 + column];
      cell[0] = level;
      edge = 63;
      if (column < edge)
        cell[1] = level;
      error = error - y * 2 - 1;
      if (error < 0) {
        error = error + x * 2 - 2;
        x--;
      }
      y++;
    }
    i++;
  } while (i != count - 1);
}
