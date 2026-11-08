// Differ 13 of 137 (tla-en, 2026-11-08, -mtune=gba): the first entry is read into `offset` before the table's address is set, so the address loads into r7 and copies to r2 as in the native; that read still goes through r7 where the native reads through r2. Left besides: the menu state is copied from r4 into r3 before its test (here tested in r4); the rest is pool offsets. The entry read after the table is set, through `table`, or into a u8, and the state read once after the loop, measure 15 or worse. 10 permute minutes (2026-11-08) reached 11 only by taking the table's address (a dead device).
// 2026-11-08 before the offset local: differ 15 of 137. The column is read through `table` while both tests read the symbol, so gcse's copy of the table's address lives in r7 and is reloaded only after the row loop, as in the native. Reading the state only at the end, a second local for it, or other types for it measure 15 or worse. Also 15 or worse: once-only blocks around the table's or the state's assignments or before the tests, s8, u16, s32 or u32 state, u32 i, the table set before i or at its declaration, a non-const table; every mix of table and symbol in the two tests and the column read.
// 2026-11-05: differ 42 of 137, 20 normalized, 2 with registers ignored (tla-en, 2026-11-05): the guarded goto loop with the menu state read first and again at the loop's exit gives the native's shape. Left: two copies the native has and this lacks, the table's address through r7 into r2 and the state from r4 into r3 before its test; forms with a second local fold away.
// Draft, differ 80 of 137, 2026-10-15: The Broken Seal's
// Window_DrawColumnBorders on this game's window (signed position,
// tilemap rows indexed directly, the column added before it is doubled).
// Remaining: the native reads the menu state into r4 in its first block and
// reads it again at the loop's exit, so the pre-loop exit jumps past the
// reload; this reads it once after the loop and allocates the loop's
// registers differently. A state local read at the start is spilled
// (differ 52); four permuter minutes reached differ 47 with a copy of the
// work pointer in the loop. 2026-10-25: alchemy-gcc's jump pass does copy the
// exit test in front of the loop (the RTL dump of jump shows it), but jump2
// cross-jumps the copy back into the test at the foot, so the loop is
// entered by a branch to it; the native keeps both copies (cmp; blt before
// the loop). With the row's three-way store reduced to two the copies differ
// and both stay.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

extern const s8 gWindow_PartyColumnOffsets[];

void Func_080426e0(struct Window *window, u32 flags) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 first = 1;
  s32 bias = 0;
  u32 max = window->width - 1;
  s32 i;
  u8 state = canvas->menuState;
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;
  const s8 *table;
  s8 offset;

  if ((flags & 1) == 0)
    flags &= ~2;
  if (flags & 2) {
    bias = 5;
    first = 0;
  }
  i = first;
  offset = gWindow_PartyColumnOffsets[i];
  table = gWindow_PartyColumnOffsets;
  if (offset >= 0) {
  next:
    col = table[i] + bias;
    if (col < max) {
      for (row = 0; row != rows; row++) {
        dest = canvas->tilemap[(s16)window->y + row] + ((s16)window->x + col);
        if (row == 0)
          *dest = 0xf018;
        else if (row == rows - 1)
          *dest = 0xf019;
        else
          *dest = 0xf00f;
      }
    }
    i++;
    table = gWindow_PartyColumnOffsets;
    if (gWindow_PartyColumnOffsets[i] >= 0)
      goto next;
    state = canvas->menuState;
  }
  if (state) {
    dest =
        canvas->tilemap[(s16)window->y + window->height - 1] + (s16)window->x;
    col = 1;
    *dest++ = 0xf080;
    while (col < max) {
      col++;
      *dest++ = 0xf081;
    }
    *dest = 0xf082;
  }
  canvas->dirty = 1;
}
