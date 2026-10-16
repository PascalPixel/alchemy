// Draft, differ 52 of 90 (2026-10-15), first pass. A map probe the field
// Psynergy helpers use, like The Broken Seal's Map_CheckObjectStep (field.c)
// but taking the point and the height directly. Belongs with
// cell_collision.c's height probes. Left: allocation from the layer test on
// (the native reuses the map pointer for the shape grid) and the tail tests'
// layout.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];
extern u8 gMapCollision[];
extern u8 Data_0202c001[];

// The height curve of each cell kind (mapdata.c), given the cell's
// collision record past its kind byte and the point within the cell.
typedef s32 (*TerrainHeightFn)();
extern const TerrainHeightFn gMap_TerrainHeightHandlers[16];

// A tile id with no ground under it.
#define TILE_NONE 255

// How the ground at (x, z) on the given layer stands to height: 0 when an
// object at that height could step there, 1 when the ground rises more than
// 8 pixels above it, -1 when it falls more than 12 below, 2 when there is no
// ground. On the world map, or with no map, every point is level (0).
s32 Func_0802d7b0(s32 x, s32 z, s32 layer, s32 height) {
  struct MapWork *map;
  u8 *event;
  u8 *cells;
  u8 *shapes;
  s32 column;
  s32 row;
  s32 index;
  s32 shape;
  s32 rise;

  column = x >> 16;
  row = z >> 16;
  map = Ram_HeapSlots->mapWork;
  event = Ram_HeapSlots->eventWork;
  if (map == 0)
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
  index = column / 16 + ((row / 16) << 7);
  if (cells[index * 4 + 2] == TILE_NONE)
    return 2;

  shape = shapes[index];
  rise = gMap_TerrainHeightHandlers[gMapCollision[shape * 4] & 15](
             &Data_0202c001[shape * 4], column & 15, row & 15) -
         height;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}
