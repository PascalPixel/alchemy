#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = Ram_HeapSlots->windowTiles;
  u16 *cursor = &((struct WindowWork *)base)->tilemap[y][x];
  u32 alternate = ((struct WindowWork *)base)->alt;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tile = *cursor++ & 0x3ff;

      if ((tile - 0x80) <= 0x7f ||
          (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->tileAttributes[index] &= 0xfc;
      }
    }
    cursor += 32 - width;
  }
}

void Func_08041b68(void) {
  u8 *base = Ram_HeapSlots->windowTiles;
  u32 alternate = ((struct WindowWork *)base)->alt;
  u16 *cursor = ((struct WindowWork *)base)->tilemap[0];
  s32 width = 30;
  s32 row;
  s32 x;

  for (row = 20; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tile = *cursor++ & 0x3ff;

      if ((tile - 0x80) <= 0x7f ||
          (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->tileAttributes[index] |= 2;
      }
    }
  }

  for (row = 0; row < 256; row++, base++) {
    if (((struct WindowWork *)base)->tileAttributes[0] == 1)
      ((struct WindowWork *)base)->tileAttributes[0] = 0;
  }
}
