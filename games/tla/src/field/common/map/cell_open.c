#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

typedef s32 (*TerrainHeightFn)(u8 *shape, s32 x, s32 z);
extern TerrainHeightFn gMap_TerrainHeightHandlers[16];

// Probes the ground under the point position (16.16 pixels, read as its
// whole-pixel halves) on the object's layer (the byte at 34): 2 when the
// cell has no tile, 1 when the ground rises more than 8 pixels above the
// object's height (the word at 20), -1 when it falls more than 12 below,
// else 0. With no map, or while the event mode is 3, every point is level
// (0).
s32 Func_0802d87c(u8 *object, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (object[34] <= 2) {
    cells = (u8 *)map->layers[object[34]].cells;
    shapes = map->layers[object[34]].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15) -
         *(s32 *)(object + 20);
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

// As Func_0802d87c, against the object's word at 12 and with no -1: 2 when
// the cell has no tile, 1 when the ground rises more than 8 pixels above
// it, else 0.
s32 Func_0802d958(u8 *object, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (object[34] <= 2) {
    cells = (u8 *)map->layers[object[34]].cells;
    shapes = map->layers[object[34]].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  if (gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15) -
          *(s32 *)(object + 12) >
      8 << 16)
    return 1;
  return 0;
}

s32 Func_0802da24(s32 *position) {
  s32 x = position[0] / 65536;
  s32 z = (u32)((position[2] & 0xfff00000) - position[1]) >> 16;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  cell = *(u8 **)(map + 0x1a8);
  cell += (x / 16 + (z / 16) * 128) * 4;
  return (cell[2] != 255) - 1;
}
