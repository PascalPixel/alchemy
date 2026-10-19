// Near miss, differ 94 of 114 (2026-10-18), first pass: The Broken Seal's
// Map_RenderMetatileColumn with the layer window of 0802abbc and its
// CopyMetatileHalfColumn helper. Left: allocation and order in the window
// setup; the native adds the layer's 0x108 after forming the BG map address
// and keeps 0x3f80's mask in lr, 0x3c0 in r10.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

#define Ram_MapBlocks ((u8 *)0x02020000)

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

static __inline__ void CopyMetatileHalfColumn(u32 *cell, u8 *screenMap,
                                              u32 screenRowOffset,
                                              u32 screenColumnOffset,
                                              u32 tileHalf) {
  u16 *metatileEntry;
  u8 *screenTile;
  u32 index = ((*cell << 21) >> 19) + tileHalf;

  metatileEntry = (u16 *)Ram_MapBlocks;
  metatileEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *metatileEntry;
  metatileEntry = (u16 *)(Ram_MapBlocks + 4);
  metatileEntry += index;
  *(u16 *)(screenTile + 64) = *metatileEntry;
}

void Func_0802ac90(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapLayerWindow *window =
      (struct MapLayerWindow *)&Ram_HeapSlots->mapWork->layers[layer];
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 maskY = window->maskY;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 originY = window->originY;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 tileHalf;
  u32 metatileIndex;

  if (maskY != 0x3f80) {
    sourceRow = (sourceRow - originY) & maskY;
    cells = (u32 *)Ram_MapCellBuffer + originY;
  }
  maskX = window->maskX;
  sourceColumn = (tileX / 2) & 0x7f;
  screenColumnOffset = tileX & 30;
  originX = window->originX;
  sourceColumn &= maskX;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  tileHalf = tileX & 1;
  for (metatileIndex = 0; metatileIndex <= 10; metatileIndex++) {
    CopyMetatileHalfColumn(cells + sourceRow + sourceColumn, screenMap,
                           screenRowOffset, screenColumnOffset, tileHalf);
    sourceRow = (sourceRow + 128) & maskY;
    screenRowOffset = (screenRowOffset + 64) & 0x3c0;
  }
}
