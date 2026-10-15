#include "airs_rock_inside.h"

void Func_02000610(void) { Func_02005f2c(0); }

void Func_0200061c(void) { BYTE_AT(Ram_HeapSlots->effectWork, 52) = 1; }

s32 Func_0200062c(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}
