#include "izumo_ruins.h"

s32 Func_02000eac(u32 layer, s32 x, s32 y) {
  struct IzumoRuinsMapWork *map =
      (struct IzumoRuinsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 Func_02000ee8(u32 layer, s32 x, s32 y, s32 value) {
  struct IzumoRuinsMapWork *map =
      (struct IzumoRuinsMapWork *)Ram_HeapSlots->mapWork;
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
