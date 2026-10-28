#include "local.h"

void Func_0200063c(struct Actor *object) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    WaitFrames(1);
    busy = object->velocityY;
    count--;
    if (busy == 0) {
      break;
    }
  }
}

s32 Func_02000658(u32 layer, s32 x, s32 y) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
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

s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
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
