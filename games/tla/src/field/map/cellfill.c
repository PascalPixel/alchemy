#include "types.h"
#include "system/ram_buffer.h"
#include "map.h"

/// Sets the collision code of each cell in a layer rectangle to value.
void Map_FillCellRectByte2(s32 layer, s32 x, s32 y, u32 width, u32 height,
                           s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells = map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += x + (y << 7);
  for (row = 0; row < height; row++) {
    struct MapCell *cell = cells + (row << 7);

    for (column = 0; column < width; column++) {
      cell->collisionCode = value;
      cell++;
    }
  }
}
