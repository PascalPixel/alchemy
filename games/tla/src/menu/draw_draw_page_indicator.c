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
#include "io_reg.h"

void Palette_LightenBankHighlight(s32 bank) {
  u16 *palette = (u16 *)(0x05000000 + (bank << 5));
  u32 red;
  u32 green;
  u32 blue;

  Dma_Set((const void *)0x050001e0, palette, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  blue = palette[4] >> 10;
  green = (palette[4] >> 5) & 31;
  red = palette[4] & 31;
  blue += 9;
  if (blue > 31)
    blue = 31;
  green += 9;
  if (green > 31)
    green = 31;
  red += 9;
  if (red > 31)
    red = 31;
  palette[4] = RGB(red, green, blue);
}

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

void Menu_DrawPageIndicator(s32 window, s32 itemCount, s32 pageSize,
                            s32 selectedPage, s32 rightEdge) {
  s32 pageCount;
  s32 page;
  s32 x;
  s32 tile;

  x = rightEdge;
  tile = PAGE_LABEL_FIRST;
  pageCount = itemCount / pageSize;
  if (itemCount % pageSize != 0)
    pageCount++;

  x -= pageCount;
  if (pageCount > 1) {
    UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

    for (page = 0; page < pageCount; page++) {
      if (page == selectedPage)
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2);
      else
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3);
      tile++;
      x++;
    }

    UiWindow_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
  }
}
