#include "types.h"
#include "ram_buffer.h"
#include "map.h"

/// A map layer as its renderers read it: its scroll, then the window over
/// the map's cells (masks and origins that wrap the source cells).
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

/// Draws a row of 16 cell pictures (32 tiles) of a layer's cells into its BG map
/// (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles and the cells wrap at the layer's window. The Broken
/// Seal's MapGrid_RenderCellPictureRow, with the window. Called by
/// Func_0802ad84 when a layer scrolls onto a new row of cell pictures.
void Func_0802abbc(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayerWindow *window = (struct MapLayerWindow *)&map->layers[layer];
  u32 originY = window->originY;
  u8 *screenMap = (u8 *)0x06002800 + (layer << 11);
  u32 maskY = window->maskY;
  u32 sourceRow = ((tileY / 2) & 0x7f) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 cellPictureIndex;

  // A window narrower than the 128 by 128 cells starts at its origin.
  if (maskY != 0x3f80) {
    sourceRow = (sourceRow - originY) & maskY;
    cells += originY;
  }
  maskX = window->maskX;
  sourceColumn = (tileX / 2) & 0x7f;
  originX = window->originX;
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != 0x7f) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  for (cellPictureIndex = 0; cellPictureIndex <= 15; cellPictureIndex++) {
    u32 *sourceCell;
    u32 *cellPictureRows;
    u8 *screenTile;
    u32 cellPictureWordOffset;

    sourceCell = cells + sourceRow + sourceColumn;
    cellPictureWordOffset = (*sourceCell << 21) >> 20;
    cellPictureRows = (u32 *)Ram_MapBlocks;
    cellPictureRows += cellPictureWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *cellPictureRows;
    cellPictureRows = (u32 *)(Ram_MapBlocks + 4);
    cellPictureRows += cellPictureWordOffset;
    sourceColumn++;
    *(u32 *)(screenTile + 64) = *cellPictureRows;
    sourceColumn &= maskX;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}
