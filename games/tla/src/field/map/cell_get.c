#include "types.h"
#include "ram_buffer.h"
#include "map.h"

/* VIEW(cell_get): variable gMapCellBuffer as u8 gMapCellBuffer[];
 * field/map/cell_fill_byte has struct MapCell gMapCellBuffer[]. */
extern u8 gMapCellBuffer__cell_get[] __asm__("gMapCellBuffer");

/// Returns a cell's collision code and attributes as a halfword.
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = gMapCellBuffer__cell_get;
  cell += (x + (z << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}
