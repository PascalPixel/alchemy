#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void Func_0803a2b0(s32 x, s32 y, u32 width, u32 height, s32 whole) {
  struct UiRenderWork *canvas =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;
  u32 column;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  cursor += 32;
  if (whole == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (column = 1; column < width - 1; column++)
        *cursor++ =
            ((0x127 + row + (column - 1) * (height - 2)) & 0xfff) | 0xf000;
      cursor++;
      cursor += 32 - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (column = 0; column < width; column++)
        *cursor++ = ((0x127 + row + column * (height - 2)) & 0xfff) | 0xf000;
      cursor += 32 - width;
    }
  }
  canvas->dirty = 1;
}
