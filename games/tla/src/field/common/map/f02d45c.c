#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 z);

extern TerrainHeightFn gMap_TerrainHeightHandlers[16];

s32 Func_0802d45c(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  grid += x / 16 + (z / 16 << 7);
  code = Ram_MapCollision + *grid * 4;
  return gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15);
}
