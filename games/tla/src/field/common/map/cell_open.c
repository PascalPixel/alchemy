#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

typedef s32 (*TerrainHeightFn)(u8 *shape, s32 x, s32 z);
extern TerrainHeightFn gMap_TerrainHeightHandlers[16];

// The height of the ground at (px, pz) (16.16 pixels) on the given layer,
// less base; a layer past 2 reads the shape grid. With no map, or while the
// event mode is 3, it is 0.
s32 Func_0802d71c(s32 px, s32 pz, s32 layer, s32 base) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *shapes;
  u8 *code;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (layer <= 2)
    shapes = map->layers[layer].terrainIds;
  else
    shapes = gMapShapeGrid;
  code = Ram_MapCollision + shapes[x / 16 + ((z / 16) << 7)] * 4;
  /* FAKEMATCH(barrier): an empty asm, which keeps the shape kind's load
   * ahead of the 15 mask as in the twins below, where the mask register is
   * not free; as the plain call, 2 of 66 instructions differ in tla-en. */
  __asm__ volatile("");
  return gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15) - base;
}

// How the ground at (px, pz) (16.16 pixels) on the given layer stands to
// height: 2 when the cell has no tile, 1 when the ground rises more than 8
// pixels above it, -1 when it falls more than 12 below, else 0. A layer past
// 2 reads the cell buffer and shape grid. With no map, or while the event
// mode is 3, every point is level (0).
s32 Func_0802d7b0(s32 px, s32 pz, s32 layer, s32 height) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
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
  if (layer <= 2) {
    cells = (u8 *)map->layers[layer].cells;
    shapes = map->layers[layer].terrainIds;
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
  rise =
      gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15) - height;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

// As Func_0802d7b0 for the point position (16.16 pixels, read as its
// whole-pixel halves) on the object's layer (the byte at 34), against the
// object's height (the word at 20).
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
