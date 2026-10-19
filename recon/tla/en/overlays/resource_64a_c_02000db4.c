// Draft, differ 79 of 69: ⚓️'s UiWindow_SetTilemapFlagRect (graphics/window/
// fixed_block.c) as overlay 64A's copy. The native strength-reduces the row
// base (work + y * 64, stepping 64) and adds x * 2 + 8 per row, keeps width
// in r7 and x in r5, and leaves the shifted field in its stack argument slot
// (str [sp, #20], reloaded each pass with the 0xffffefff mask). Here field is
// a volatile parameter (a FAKEMATCH(volatile) device when adopted), which
// gives that slot's store and reloads (the first draft's loops: differ 94
// without it, 82 with it), and the row is a pointer stepped by 32 halfwords
// (79). Left: register choice (the native keeps work in ip, and the counts
// rotate) and the mask's pool reload inside the inner loop. goto loops on
// the first draft measured 87.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void Func_02000db4(const struct UiWindow *object, s32 x, s32 y, s32 width,
                   s32 height, volatile u32 field) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    u16 *row = work->tilemap[y];
    do {
      u16 *pixel = row + x;
      s32 remaining = width;
      while (remaining != 0) {
        *pixel = (*pixel & 0xFFFFEFFF) | field;
        remaining--;
        pixel++;
      }
      height--;
      row += 32;
    } while (height != 0);
    work->dirty = 1;
  }
}
