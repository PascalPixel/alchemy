// Near miss, differ 2 of 66 (2026-10-18): the height of the ground at a point
// on a layer less base, written as its matched twins Func_0802d7b0 and
// Func_0802d87c (cell_open.c). Left: one instruction order; the native reads
// the kind (ldrb r0, [r0]) before it builds the 15 mask (movs r2, #15). Here
// the mask is free to go first, as Collision's base sits in r3; in the twins
// it sits in r2 and holds the mask back. Pinning the mask behind a barrier
// placed in the index (differ 16 to 26) or reading the kind into a local
// (differ 45) loses the code pointer's constant 0x0202c001.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];
extern s32 (*const gMap_TerrainHeightHandlers[])(u8 *shape, s32 x, s32 z);

s32 Func_0802d71c(s32 px, s32 pz, s32 layer, s32 base) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *state = Ram_HeapSlots->eventWork;
  u8 *grid;
  u8 *code;

  if (work == NULL)
    return 0;
  if (*(s8 *)(state + 0x18a) == 3)
    return 0;
  if (layer <= 2)
    grid = work->layers[layer].terrainIds;
  else
    grid = gMapShapeGrid;
  code = Ram_MapCollision + grid[x / 16 + ((z / 16) << 7)] * 4;
  return gMap_TerrainHeightHandlers[*code++ & 15](code, x & 15, z & 15) - base;
}
