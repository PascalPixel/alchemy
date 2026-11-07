// Differ 10 of 62 (tla-it, 2026-11-07).
// The Spanish and Italian form (es 0x080e4b6c, it 0x080dbb6c); no other
// edition links it under this name. Control flow, frame and pool match; the
// rest is allocation: the listing keeps gMapBlocks in r6 (after 394) and the
// layer table's 312 in r2, where the draft swaps them.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u32 gMapBlocks[];
extern u8 gMapCellBuffer[];

void *Map_GetCellAtPosition(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if (*(s8 *)(event + 394) == 3) {
    cells = (struct MapCell *)&gMapBlocks[((x / 0x200000) & 31) +
                                          (((z / 0x200000) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = work->layers[layer].cells;
    else
      cells = (struct MapCell *)gMapCellBuffer;
    cells += (x / 0x100000) + ((z / 0x100000) << 7);
  }
  return cells;
}
