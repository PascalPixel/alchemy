/// @file
/// Finds a map cell by position.

#include "types.h"
#include "ram_buffer.h"
#include "map.h"

/// Returns the map cell at position x, z in the given layer, read from the
/// world map's blocks or the cell buffer by the event work's mode.
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if ((s8)event[394] == 3) {
    cells = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                               (((z / 0x200000) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = map->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / 0x100000 + (z / 0x100000) * 128];
  }
  return cells;
}
