// Draft, differ 130 of 99 (tla-en, 2026-11-05): rewritten readable, no read before a set; was 104 with one. Older notes may predate this.
// Draft, differ 81 of 99, skeleton gap 22 removed, 11 added (tla-en, 2026-11-01)
// Fills a 64 by 64 grid of levels, each ring a quarter circle drawn with the
// midpoint algorithm, its level from the cosine of its radius.
// Left, one cause: PRE hoists count - 1 onto the loop's entry edge (subs before
// the first bl pass, then r9), so count dies there. The native rebuilds it at
// the top of each pass (movs r4, #1; negs; add r4, r11; mov r9, r4), count stays
// in r11 and that extra pseudo pushes scale onto the stack (the 4-byte frame), y
// to r10 and edge out of the registers, so its 63 is rematerialized (movs r2,
// #63, movs r4, #63) and compared as an immediate. Here scale keeps r11, edge
// keeps r10, y r6 and error r4, which is nearly all of the differ and the skeleton
// gap. The registers already agreeing: i r7, x r5, grid r8, last r9, level r1.
// A kill of count at the loop top (an asm probe, not kept) does stop the hoist,
// so nothing else in the shape is at fault; plain C found nothing that does.
// Classic GCSE would leave it in the loop and gcse.c takes that only under
// optimize_size: a compiler and option question for Pascal (K3), out of scope.
// Settled forms: goto loops (a for measures 83 or worse, and hoists the same);
// y = 0 before the call (later costs 24); edge as a local, since a literal folds
// to cmp #62; bgt, while the listing has cmp #63; bge (without it 124); second
// column = row, as 63 - y merged by gcse loses the rebuilt 63 - x.
// Tried with no gain, 1 November: count - 1 spelled twice or in the first test,
// a copy of count at the top of the loop, last set in the for's test, edge in an
// inner block (125) or used only by the compares (111), forty declaration
// orders (all 81), cell replaced by indexing, level * scale.
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
    s32 last = count - 1;
    s32 y = 0;
    s32 level = (Math_Cosine((i << 14) / last) * 15) >> 16;
    s32 x = i;
    s32 error = i;

    level = scale * level / 0x10000;
    if (level > 15)
      level = 15;
    if (level <= 0)
      level = 1;
    while (x >= y) {
      s32 column = 63 - x;
      s32 row = 63 - y;
      u8 *cell = &grid[row * 64 + column];

      cell[0] = level;
      if (column < 63)
        cell[1] = level;
      column = row;
      row = 63 - x;
      cell = &grid[row * 64 + column];
      cell[0] = level;
      if (column < 63)
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
