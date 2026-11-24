/// @file
/// Filling a rectangle of a map layer's cells with one collision code, as a
/// pushed actor marks the cells it covers (push_target_push.h). Several
/// scenes' overlays include a copy.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"

/// Sets the collision code of each cell in a layer rectangle to value.
void MapGrid_FillCollisionRectangle(s32 layer, s32 x, s32 y, u32 width,
                                    u32 height, s32 value) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
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
