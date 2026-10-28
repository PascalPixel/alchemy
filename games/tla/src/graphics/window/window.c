#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "ram_buffer.h"
#include "window.h"
#include "io_reg.h"

void WindowSystem_UploadTilemap(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 flags;
  u8 *src;
  u8 *dst;

  flags = work->dirty;
  if (flags != 0) {
    if (!work->menuBusy) {
      src = (u8 *)work->tilemap;
      dst = (u8 *)0x06002000;
      if (flags & 1)
        flags = 63;
      flags &= 63;
      flags >>= 1;
      do {
        if (flags & 1)
          Dma_Set(src, dst, 0x84000040, REG_DMA3);
        flags >>= 1;
        src += 256;
        dst += 256;
      } while (flags);
      work->dirty = flags;
    }
  }
}

void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  s32 tile;
  u32 bottom;
  u32 row;

  tile = 240;
  bottom = y + height;
  tile <<= 8;
  if (bottom > 20)
    height = 20 - y;
  if (width <= 1)
    width = 2;
  if (width > 30)
    width = 30;
  if (height <= 1)
    height = 2;
  if (height > 30)
    height = 30;

  Window_ClearTextTilesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    do {
      u32 column;

      if (canvas->menuState != 0) {
        if ((u32)(y + row) > 16)
          tile = 0xF07F;
        else
          tile = 0xF000;
      }
      column = 0;
      if (column < width) {
        do {
          *cursor++ = tile;
          column++;
        } while (column < width);
      }
      row++;
      cursor += 32 - width;
    } while (row < height);
  }
  canvas->dirty = 1;
}
