// Matches (tla-it, 2026-11-07).
// The Spanish and Italian form (es 0x080e4b6c, it 0x080dbb6c); no other
// edition links it under this name. Reading the blocks and cell buffer through
// Ram_MapBlocks and Ram_MapCellBuffer and the mode through an s8 index fixed
// the allocation (it left differ 10).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

void *Map_GetCellAtPosition(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if ((s8)event[394] == 3) {
    cells = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                                (((z / 0x200000) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = work->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / 0x100000 + (z / 0x100000) * 128];
  }
  return cells;
}
