// Near miss, differ 14 of 69: The Broken Seal's UiWindow_ClearInteriorTiles,
// reading the window's position signed (ldrsh) and loading it before the
// rounding, as the native orders it. Left: sub sp, #4 is scheduled before
// the heap slot base (native after it), the second ldrsh one slot late, the
// first argument copied before the others (native last) and the spilled
// height reloaded before the row address add. The twin's order differs 22,
// the old struct-cast draft 91.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

// Clears an area of a window's interior to the window fill. left, top, right
// and bottom are pixels inside the window, and the area covers every tile
// they touch.
void UiWindow_ClearInteriorTiles(const struct UiWindow *window, u32 left,
                                 u32 top, u32 right, u32 bottom) {
  struct UiRenderWork *work;
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
  windowY = (s16)window->y;
  right += 7;
  bottom += 7;
  y = top >> 3;
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
  UiWindow_ClearTileAttributesInRect(left, top, width, height);
  tiles = work->tilemap[0] + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = 0xf020;
    tiles += 32 - width;
  }
  work->dirty = 1;
}
