#include "types.h"
#include "ram_buffer.h"

struct InteriorWindow {
  u8 unk00[12];
  s16 x;
  s16 y;
};

struct WindowTileMap {
  u8 unk0[3];
  u8 dirty;
  u8 unk4[4];
  u16 tiles[1][32];
};

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void UiWindow_ClearInteriorTiles(struct InteriorWindow *window, u32 x, u32 y,
                                 u32 width, u32 height) {
  struct WindowTileMap *map =
      (struct WindowTileMap *)Ram_HeapSlots->windowTiles;
  u32 left = (x >> 3) + window->x;
  u32 top = (y >> 3) + window->y;
  u32 right = ((width + 7) >> 3) + window->x;
  u32 bottom = ((height + 7) >> 3) + window->y;
  u32 column = left + 1;
  u32 row = top + 1;
  u32 columns = right - left;
  u32 rows = bottom - top;
  u16 *tile;
  u32 i;
  u32 j;

  UiWindow_ClearTileAttributesInRect(column, row, columns, rows);
  tile = &map->tiles[row][column];
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++)
      *tile++ = 0xf020;
    tile += 32 - columns;
  }
  map->dirty = 1;
}
