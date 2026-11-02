// Matches: differ 0 (tla-en, 2026-11-01)
// The Broken Seal's MapGrid_SetCameraCenter for this game's layers: no phase
// masks, and the layer window (masks and origins at layer +40 to +46) of the
// renderers 0802abbc and 0802ac90; it draws 11 block rows. The enable bytes
// sit at work +0x104, read as a struct field because a cast adds an adds; the
// twin calls them enabled. No devices.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"

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

static __inline__ void CopyCellPictureToScreen(u32 *cell, u8 *screenMap,
                                               u32 screenRowOffset,
                                               u32 screenColumnOffset) {
  u32 *cellPictureRows;
  u8 *screenTile;
  u32 index = (*cell << 21) >> 20;

  cellPictureRows = (u32 *)Ram_MapBlocks;
  cellPictureRows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *cellPictureRows;
  cellPictureRows = (u32 *)(Ram_MapBlocks + 4);
  cellPictureRows += index;
  *(u32 *)(screenTile + 64) = *cellPictureRows;
}

void Func_0802af9c(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
  u32 *cells;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *layer = (struct MapLayerWindow *)work->layers;
  u32 originX;

  x -= 0x780000;
  y -= 0x600000;
  if (x < work->minX)
    x = work->minX;
  if (x > work->maxX - 0xf00000)
    x = work->maxX - 0xf00000;
  if (y < work->minZ)
    y = work->minZ;
  if (y > work->maxZ - 0xa00000)
    y = work->maxZ - 0xa00000;
  work->viewX = x;
  work->viewZ = y;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++) {
    if (work->unk0fc[layerIndex + 8] != 0) {
      u32 maskX, maskY, originY;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellPictureRow, cellPictureColumn;

      x = Iwram_MulQ16(work->viewX, layer->scaleX);
      y = Iwram_MulQ16(work->viewZ, layer->scaleY);
      if (layer->speedX != 0) {
        layer->phaseX += layer->speedX;
        x += layer->phaseX;
      }
      if (layer->speedY != 0) {
        layer->phaseY += layer->speedY;
        y += layer->phaseY;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      x /= 0x80000;
      y /= 0x80000;
      screenMap = (u8 *)0x06002800 + (layerIndex << 11);
      maskY = layer->maskY;
      maskX = layer->maskX;
      originY = layer->originY;
      originX = layer->originX;
      cells = (u32 *)Ram_MapCellBuffer;
      if (maskX != 0x7f)
        cells = (u32 *)Ram_MapCellBuffer + originX;
      if (maskY != 0x3f80)
        cells += originY;
      sourceRow = ((y / 2) & 0x7f) << 7;
      screenRowOffset = (y & 30) << 5;
      if (maskY != 0x3f80)
        sourceRow = (sourceRow - originY) & maskY;
      for (cellPictureRow = 0; cellPictureRow <= 10; cellPictureRow++) {
        sourceColumn = (x / 2) & 0x7f;
        screenColumnOffset = x & 30;
        if (maskX != 0x7f)
          sourceColumn = (sourceColumn - originX) & maskX;
        for (cellPictureColumn = 0; cellPictureColumn <= 15;
             cellPictureColumn++) {
          CopyCellPictureToScreen(cells + sourceRow + sourceColumn, screenMap,
                                  screenRowOffset, screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & maskX;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + 128) & maskY;
        screenRowOffset = (screenRowOffset + 64) & 0x3c0;
      }
      layer++;
    }
  }
}
