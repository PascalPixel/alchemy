/// @file
/// Filling a rectangle of a map layer's cells with one collision code.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"

extern struct MapCell gMapGridCellBuffer[];

/// Sets the collision code of each cell in a layer rectangle to value.
/// Layers 3 and above use the shared buffer.
s32 Map_FillLayerRectCollision(u32 layer, u32 x, u32 y, u32 width, u32 height,
                               s32 value) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapGridCellBuffer;
  cells += x + (y << 7);
  // x and y, no longer needed, count the rows and the columns.
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}
