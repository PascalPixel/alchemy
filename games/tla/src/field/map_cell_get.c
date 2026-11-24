/// @file
/// A map cell's collision code and attributes, read on a layer or from the
/// cell buffer.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"

/// Returns a cell's collision code and attributes as a halfword.
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = Ram_MapCellBuffer;
  cell += (x + (z << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}
