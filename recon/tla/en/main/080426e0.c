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
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;

  if ((flags & 1) == 0)
    flags &= ~2;
  if (flags & 2) {
    bias = 5;
    first = 0;
  }
  i = first;
  while (gWindow_PartyColumnOffsets[i] >= 0) {
    col = gWindow_PartyColumnOffsets[i] + bias;
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
  }
  if (canvas->menuState) {
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
