// Near miss, differ 133 (170 before statements reordered 2026-10-26) of 238 (2026-10-18), first pass: The Broken Seal's
// Map_SetCameraCenter for this game's layers: no phase masks, and the layer
// window (masks and origins at layer +40 to +46) of the renderers 0802abbc
// and 0802ac90; it draws 11 block rows. Left: the frame slots (the work at
// sp+24 and the view pointers at sp+16 and sp+12 in the native) and the
// register choice that follows.
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

void Func_0802af9c(s32 x, s32 y) {
  u32 layerIndex;
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *layer = (struct MapLayerWindow *)work->layers;

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
    if (((u8 *)work)[0x104 + layerIndex] != 0) {
      u8 *screenMap;
      u32 *cells;
      u32 maskX;
      u32 maskY;
      u32 originX;
      u32 originY;
      u32 sourceRow;
      u32 screenRowOffset;
      s32 halfX;
      u32 row;

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
      maskY = layer->maskY;
      maskX = layer->maskX;
      originX = layer->originX;
      originY = layer->originY;
      cells = (u32 *)Ram_MapCellBuffer;
      screenMap = (u8 *)0x06002800 + (layerIndex << 11);
      if (maskX != 0x7f)
        cells = (u32 *)Ram_MapCellBuffer + originX;
      if (maskY != 0x3f80)
        cells += originY;
      sourceRow = ((y / 2) & 0x7f) << 7;
      screenRowOffset = (y & 30) << 5;
      if (maskY != 0x3f80)
        sourceRow = (sourceRow - originY) & maskY;
      halfX = x / 2;
      for (row = 0; row <= 10; row++) {
        u32 sourceColumn = halfX & 0x7f;
        u32 screenColumnOffset = x & 30;
        u32 column;

        if (maskX != 0x7f)
          sourceColumn = (sourceColumn - originX) & maskX;
        for (column = 0; column <= 15; column++) {
          u32 metatileOffset = (cells[sourceRow + sourceColumn] << 21) >> 18;
          u8 *screenTile =
              screenMap + (screenRowOffset + screenColumnOffset) * 2;

          *(u32 *)screenTile = *(u32 *)((u8 *)0x02020000 + metatileOffset);
          sourceColumn++;
          sourceColumn &= maskX;
          *(u32 *)(screenTile + 64) =
              *(u32 *)((u8 *)0x02020004 + metatileOffset);
          screenColumnOffset += 2;
          screenColumnOffset &= 30;
        }
        sourceRow = (sourceRow + 128) & maskY;
        screenRowOffset = (screenRowOffset + 64) & 0x3c0;
      }
      layer++;
    }
  }
}
