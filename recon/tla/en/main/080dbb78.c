// Matches (tla-en, 2026-11-07).
// The world map's blocks and the cell buffer read through Ram_MapBlocks and
// Ram_MapCellBuffer, and the event work's mode through an s8 index: the empty
// asm the draft needed before (differ 10) is gone.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

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
