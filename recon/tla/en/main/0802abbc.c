// Near miss, differ 125 of 97 (2026-10-18), first pass: The Broken Seal's
// Map_RenderMetatileRow with this game's layer window (masks and origins at
// layer +40 to +46 that wrap the source cells). Left: allocation. The native
// keeps tileX in r6, the cell base in ip, the BG map in r10 and the masks 127
// and 30 in lr and r0 across the window tests, with no frame; this spills
// one word and uses r11. The block tables read through the constants
// 0x02020000 and 0x02020004 inside the loop as the native does (as symbols
// they hoist). Six minutes of permute: no better.
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

void Func_0802abbc(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapLayerWindow *window =
      (struct MapLayerWindow *)&Ram_HeapSlots->mapWork->layers[layer];
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 maskY = window->maskY;
  u32 originY = window->originY;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 metatileIndex;

  if (maskY != 0x3f80) {
    cells = (u32 *)Ram_MapCellBuffer + originY;
    sourceRow = (sourceRow - originY) & maskY;
  }
  maskX = window->maskX;
  sourceColumn = ((tileX / 2) & 0x7f) & maskX;
  originX = window->originX;
  screenColumnOffset = tileX & 30;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  for (metatileIndex = 0; metatileIndex <= 15; metatileIndex++) {
    u32 *sourceCell = cells;
    u32 *metatileRows;
    u8 *screenTile;
    u32 metatileWordOffset;

    sourceCell += sourceRow + sourceColumn;
    metatileWordOffset = (*sourceCell << 21) >> 20;
    metatileRows = (u32 *)Ram_MapBlocks;
    metatileRows += metatileWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *metatileRows;
    metatileRows = (u32 *)(Ram_MapBlocks + 4);
    metatileRows += metatileWordOffset;
    sourceColumn++;
    *(u32 *)(screenTile + 64) = *metatileRows;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
    sourceColumn &= maskX;
  }
}
