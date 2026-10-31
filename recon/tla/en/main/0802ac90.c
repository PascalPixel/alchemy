// Near miss, differ 2 of 114 (tla-en, 2026-10-30): scheduler order only. The
// Broken Seal's Map_RenderCellPictureColumn with the layer window of
// Func_0802abbc (adopted in field/map/layer_render.c, whose forms this follows)
// and its CopyCellPictureHalfColumn helper (94 before, 2026-10-26). Left: the
// native loads the second cell picture row's literal (Data_02020004) one slot
// later, after `add r3, r9`, just before the first store. In sched2 that
// literal's reload has priority 9 (load latency 3 into the add, ldrh, strh
// chain) against 8 for the screen address add, so it goes first; the native
// order needs it at 8 or less, where the add wins the tie by its three
// dependents. The instructions, registers and dependences are otherwise the
// same, so the gap is the scheduler's cost model, not the source. An empty asm
// at each line of the function measured 2 or worse; computing the screen tile
// first (10), array indexing (77), both loads before the stores (52), a u16
// screen pointer with [32], `screenTile += 64` and `screenTile = screenMap;
// +=` (2 each) do not move it; permute (4 minutes, 2 jobs) best differ 2.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct MapLayerWindow {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 originX, originY;
  struct MapCell *cells;
  u8 *terrainIds;
};

static __inline__ void CopyCellPictureHalfColumn(u32 *cell, u8 *screenMap,
                                              u32 screenRowOffset,
                                              u32 screenColumnOffset,
                                              u32 tileHalf) {
  u16 *cellPictureEntry;
  u8 *screenTile;
  u32 index = ((*cell << 21) >> 19) + tileHalf;

  cellPictureEntry = (u16 *)Ram_MapBlocks;
  cellPictureEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *cellPictureEntry;
  cellPictureEntry = (u16 *)(Ram_MapBlocks + 4);
  cellPictureEntry += index;
  *(u16 *)(screenTile + 64) = *cellPictureEntry;
}

void Func_0802ac90(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *window = (struct MapLayerWindow *)&map->layers[layer];
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 maskY = window->maskY;
  u32 originY = window->originY;
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 tileHalf;
  u32 cellPictureIndex;

  if (maskY != 0x3f80) {
    sourceRow = (sourceRow - originY) & maskY;
    cells = (u32 *)Ram_MapCellBuffer + originY;
  }
  maskX = window->maskX;
  originX = window->originX;
  sourceColumn = (tileX / 2) & 0x7f;
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  tileHalf = tileX & 1;
  for (cellPictureIndex = 0; cellPictureIndex <= 10; cellPictureIndex++) {
    CopyCellPictureHalfColumn(cells + sourceRow + sourceColumn, screenMap,
                           screenRowOffset, screenColumnOffset, tileHalf);
    sourceRow = (sourceRow + 128) & maskY;
    screenRowOffset = (screenRowOffset + 64) & 0x3c0;
  }
}
