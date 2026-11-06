// Near miss, differ 33 of 99 (tla-en, 2026-11-06), with the first cell's index through a
// 64-bit temporary (a tagged device, below; plain, 67). Left: the pool order at the fill call (the
// native loads the routine's address first), and in the ring's first block row and column in r0, r2
// for the native r2, r0, so the second block's column = row copy (adds r0, r2, #0) is gone and its
// fresh 63 is loaded before the first test. The native looks like a 63 held in a pseudo that gets
// no register (reload puts the constant into both compares and loads it fresh for the second
// row); edge set once per ring or per call takes r9 or the stack instead (76 to 119). Also no
// better from 33: column declared first, row = row - y, a second 64-bit index (139), the swap
// through a temporary (111); the fill through a pointer, Call3 or a value local (33).
// Before, plain: Near miss, differ 67 of 99 (tla-en, 2026-11-06). Permuter, 6 November: the first cell's
// index through a long long temporary (new = row * 64 + column; cell = &grid[new]) gives
// differ 33; an s32 or u32 temporary gives 67, s16 69. Not kept: a 64-bit index is no form
// this source has, but it says the first index wants a wider or separate computation.
// Before: x and the error in r4, r5 for r5, r6, the second row's 63 held from before the first compare. Fills a 64 by 64 grid of levels, each ring a quarter circle drawn with the
// midpoint algorithm, its level from the cosine of its radius.
// The register rotation is gone: row set to 63 and then lowered by y stops GCSE merging the second 63 - x, as the native rebuilds it.
// Left: the native loads 63 for the second row only after the first test (cmp r0, #63); here edge's 63 stays in a register
// from before that compare, and takes r6, so x and the error rotate to r4, r5 from r5, r6, and y's zero, reloaded through r6, floats past the calls.
// A literal 63 in the compare folds to cmp #62; column + 1 < 64 (100); column - 63 < 0 (97); size - 1 from a local (75).
// level * scale for scale * level: 71 to 67. The second block's 63 is CSE's copy of edge, live across the skipped cell[1] store; a
// 63 no register keeps (edge set once outside the inner loop, compared after GCSE folds it) gives both cmp #63 and a fresh 63
// in the second block, but the short temporaries then take r2, r3, x takes r4 and scale leaves the stack (102). Comparing
// with row before row -= y (134), one edge for every 63 (122), error and x updates reworded (67 to 68): no better.
// Also no better: grid as u8 (*)[64] (69); column and row from 63 - x and 63 - y (128); the second column as 63 - y (123).
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

    level = level * scale / 0x10000;
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
      {
        /* FAKEMATCH(temporary): the index through a 64-bit temporary; as
         * cell = &grid[row * 64 + column], 67 of 99 instructions differ in
         * tla-en. */
        s64 index = row * 64 + column;

        cell = &grid[index];
      }
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
