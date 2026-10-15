#include "types.h"
#include "ram_buffer.h"

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

// レイヤーのセル矩形の 3 バイト目を value で埋める
void Map_FillCellRectByte2(s32 layer, s32 x, s32 y, u32 width, u32 height,
                           s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  u8 *cells = map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += (x + (y << 7)) << 2;
  for (row = 0; row < height; row++) {
    u8 *cell = cells + (row << 9);

    for (column = 0; column < width; column++) {
      cell[2] = value;
      cell += 4;
    }
  }
}
