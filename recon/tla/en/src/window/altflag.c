#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);

struct WindowTileWork {
  u8 unk000[2];
  u8 alt;
  u8 unk003[0xe35];
  u8 table[256];
};

void Text_UseExtraTiles(s32 alt) {
  struct WindowTileWork *work =
      (struct WindowTileWork *)Ram_HeapSlots->windowTiles;
  s32 i;
  s32 fill;
  u8 *p;

  if (alt) {
    work->alt = 1;
    for (i = 128; i < 256; i++)
      work->table[i] = 0;
  } else {
    work->alt = 0;
    fill = 0;
    p = work->table + 128;
    for (i = 0; i < 128; i++)
      *p++ = fill;
  }
}

void Func_08041c50(void) {}

/// Draws a divider from (x1, y1) to (x2, y2), one of which must agree,
/// joining it to the frame and divider tiles already on the tilemap.
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2,
                              u32 y2) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor;
  u32 pos;
  u32 tile;

  if (x1 == x2) {
    if (y1 == y2)
      return;
    if (y1 > y2) {
      pos = y1;
      y1 = y2;
      y2 = pos;
    }
    Window_ClearTextTilesInRect((s16)win->x + x2, (s16)win->y + y1, 1,
                                       y2 - y1);
    cursor = work->tilemap[(s16)win->y + y1] + (s16)win->x + x2;
    for (pos = y1; pos <= y2; pos++) {
      tile = *cursor;
      if (pos == y1) {
        switch (tile) {
        case 0xf009:
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
        case 0xf018:
          break;
        case 0xf011:
          tile = 0xf018;
          break;
        case 0xf00a:
          tile = 0xf00d;
          break;
        case 0xf00e:
          tile = 0xf009;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      } else if (pos == y2) {
        switch (tile) {
        case 0xf00a:
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
        case 0xf019:
          break;
        case 0xf014:
          tile = 0xf019;
          break;
        case 0xf009:
          tile = 0xf00d;
          break;
        case 0xf00e:
          tile = 0xf00a;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      } else {
        switch (tile) {
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
          break;
        case 0xf009:
        case 0xf00a:
        case 0xf00e:
          tile = 0xf00d;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      }
      *cursor = tile;
      cursor += 32;
    }
  } else if (y1 == y2) {
    if (x1 == x2)
      return;
    if (x1 > x2) {
      pos = x1;
      x1 = x2;
      x2 = pos;
    }
    Window_ClearTextTilesInRect((s16)win->x + x1, (s16)win->y + y1,
                                       x2 - x1, 1);
    cursor = work->tilemap[(s16)win->y + y1] + (s16)win->x + x1;
    for (pos = x1; pos <= x2; pos++) {
      tile = *cursor;
      if (pos == x1) {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00b:
        case 0xf00d:
        case 0xf01a:
          break;
        case 0xf016:
          tile = 0xf01a;
          break;
        case 0xf00c:
          tile = 0xf00d;
          break;
        case 0xf00f:
          tile = 0xf00b;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      } else if (pos == x2) {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00c:
        case 0xf00d:
        case 0xf01b:
          break;
        case 0xf017:
          tile = 0xf01b;
          break;
        case 0xf00b:
          tile = 0xf00d;
          break;
        case 0xf00f:
          tile = 0xf00c;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      } else {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00d:
          break;
        case 0xf00b:
        case 0xf00c:
        case 0xf00f:
          tile = 0xf00d;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      }
      *cursor = tile;
      cursor++;
    }
  }
}
