// Draft, differ 64 of 91: as Func_0802d87c (0802d87c.c) against the
// object's word at +12, and only the rise test: 1 when the ground under the
// point is more than 8 pixels above it, 2 with no ground, 0 otherwise. No
// ☀️ twin as such. Left as in Func_0802d87c: register choice (the layer
// byte copied from r3, the handler index built in ip) and the tail's
// layout.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];
extern u8 gMapCollision[];
extern u8 Data_0202c001[];

typedef s32 (*TerrainHeightFn)();
extern const TerrainHeightFn gMap_TerrainHeightHandlers[16];

#define TILE_NONE 255

struct StepObject {
  u8 unk00[12];
  s32 top;
  u8 unk10[18];
  u8 layer;
};

struct StepPoint {
  s16 xFraction;
  s16 x;
  s32 y;
  s16 zFraction;
  s16 z;
};

s32 Func_0802d958(struct StepObject *object, struct StepPoint *point) {
  struct MapWork *map;
  u8 *event;
  u8 *cells;
  u8 *shapes;
  s32 column;
  s32 row;
  s32 index;
  s32 shape;
  s32 rise;
  u32 layer;
  s32 result;

  column = point->x;
  row = point->z;
  map = Ram_HeapSlots->mapWork;
  event = Ram_HeapSlots->eventWork;
  result = 0;
  if (map == 0)
    return result;
  if (*(s8 *)(event + 0x18a) == 3)
    return result;

  layer = object->layer;
  if (layer <= 2) {
    cells = (u8 *)map->layers[layer].cells;
    shapes = map->layers[layer].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = column / 16 + ((row / 16) << 7);
  cells += index * 4;
  shapes += index;
  result = 2;
  if (cells[2] == TILE_NONE)
    return result;

  shape = *shapes;
  rise = gMap_TerrainHeightHandlers[gMapCollision[shape * 4] & 15](
             &Data_0202c001[shape * 4], column & 15, row & 15) -
         object->top;
  if (rise > 8 << 16)
    result = 1;
  else
    result = 0;
  return result;
}
