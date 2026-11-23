/// @file
/// MapCell_SetAttributes, the field's routine that replaces a map cell's
/// collision code and attributes, as the scenes that change their cells keep
/// a copy of it (Treasure Isle's unused): included at its place in each
/// scene's overlay, after the field's map_cell_get.c or before it as the ROM
/// has it.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"

extern struct MapCell gMapGridCellBuffer[];

/// Replaces a cell's collision code and attributes on a layer, or in the cell
/// buffer for a layer past the overhead one, and returns the old value.
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= MAPGRID_LAYER_OVERHEAD)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + z * MAPGRID_SIZE;
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}
