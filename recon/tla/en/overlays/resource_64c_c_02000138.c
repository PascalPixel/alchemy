// Matches: differ 0 (tla-en, 2026-11-08)
// Also 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it; no device. Fills a 64
// by 64 grid of levels, each ring a quarter circle around the corner cell
// (63, 63) drawn with the midpoint algorithm, its level from the cosine of its
// radius.
// From 33 (8 November): the 63s come from three locals that each hold it for
// their whole life (S2's ordinary C). cornerColumn and cornerRow live across
// the calls with so few uses that global allocation leaves them no register,
// so reload loads 63 at each use: once for the first cell's column and row
// (the second load inherited), fresh after the label for the second cell's
// row, and r4, chosen for the first row's two reloads, joins the spill
// registers, which gives the native's r4 at count - 1, the 0xffff bias and the
// second 63. Two locals, not one: with one corner the second row is GCSE's
// copy of the first column (74, as with literal 63s). lastColumn, set in the
// loop, is only compared, so CSE folds it to cmp #63 (a literal folds to
// cmp #62 at tree level; 8). (row << 6) keeps the column = row copy that
// row * 64's synth_mult copy of row lets CSE remove (41). The fill through a
// local pointer loads the routine's address before the arguments, as the
// native's pool order shows (4 through the header).
#include "types.h"
#include "call.h"
#include "iwram_call.h"

s32 Math_Cosine(s32 angle);

void Func_02000138(u8 *grid, s32 count, s32 scale) {
  // Through a local pointer: the routine's address loads before the arguments.
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;
  s32 i = 0;
  // The corner the rings are centred on; each holds 63 for its whole life.
  s32 cornerColumn = 63;
  s32 cornerRow = 63;

  fill(grid, 0x1000, 0x01010101);
  if (count == 1)
    return;
  do {
    s32 y = 0;
    s32 last = count - 1;
    s32 level;
    s32 x = i;
    s32 error = i;

    level = (Math_Cosine((i << 14) / last) * 15) >> 16;
    level = level * scale / 0x10000;
    if (level > 15)
      level = 15;
    if (level <= 0)
      level = 1;
    while (x >= y) {
      s32 lastColumn = 63;
      s32 column = cornerColumn - x;
      s32 row = cornerRow - y;
      u8 *cell;
      // row << 6 is row * 64, spelled as the shift it matches with.
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
        cell[1] = level;
      // The same point mirrored across the diagonal.
      column = row;
      row = cornerRow - x;
      cell = &grid[(row << 6) + column];
      cell[0] = level;
      if (column < lastColumn)
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
