#include "ride.h"
#include "event_inline.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

s32 Func_0200012c(u32 layer, s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += x + (y << 7);
  return (cell->collisionCode << 8) | cell->attributeB;
}

s32 Func_02000168(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}
