#include "types.h"
#include "ram_buffer.h"
#include "window.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

s32 Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);

// Clears an area of a window's interior to the window fill. left, top, right
// and bottom are pixels inside the window, and the area covers every tile
// they touch.
void Window_ClearInteriorTiles(const struct Window *window, u32 left, u32 top,
                               u32 right, u32 bottom) {
  struct WindowWork *work;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  u16 *tiles;
  u32 row;
  s32 windowX;
  s32 windowY;

  work = UI_WORK;
  x = left >> 3;
  windowX = (s16)window->x;
  y = top >> 3;
  windowY = (s16)window->y;
  right += 7;
  bottom += 7;
  right >>= 3;
  bottom >>= 3;
  y += windowY;
  x += windowX;
  right += windowX;
  bottom += windowY;
  left = x + 1;
  top = y + 1;
  width = right - x;
  height = bottom - y;
  /* FAKEMATCH(signature): Window_ClearTextTilesInRect declared returning
   * s32, though it returns nothing, so the call sets r0 and the scheduler
   * sets r1 and r2 before r0; as void, 2 of 69 instructions differ in
   * tla-en. */
  Window_ClearTextTilesInRect(left, top, width, height);
  tiles = work->tilemap[0] + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = 0xf020;
    tiles += 32 - width;
  }
  work->dirty = 1;
}
