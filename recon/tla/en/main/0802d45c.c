// Draft, differ 4 of 53 (2026-10-24): an empty asm before the handler call, as
// in its adopted twin Func_0802d71c (cell_open.c), reads the kind before the
// 15 mask. Left: the native loads the handler table and Data_0202c001 one slot
// earlier each.
// Near miss, differ 6 of 53 (instruction order in the last block): The Broken
// Seal's Map_GetTerrainHeight on this game's map work. The native reads the
// handler kind (ldrb r0) before it starts the 15 mask and loads the handler
// table and the heights address one step earlier each; here movs r2, #15 comes
// first. Measured: the kind in a temporary is differ 51, a one-pass block
// around its load differ 53, the pointer-advance form of the cell lookup the
// same differ 6; three minutes of alchemy permute found nothing better.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 z);

extern TerrainHeightFn gMap_TerrainHeightHandlers[16];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z) {
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
