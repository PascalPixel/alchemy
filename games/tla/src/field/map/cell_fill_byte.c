#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

/// Sets the collision code of each cell in a layer rectangle to value.
/// Layers 3 and above use the shared buffer.
s32 Map_FillLayerRectByte2(u32 layer, u32 x, u32 y, u32 width, u32 height,
                           s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapCellBuffer;
  cells += x + (y << 7);
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}
