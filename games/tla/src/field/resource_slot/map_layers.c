/// @file
/// Layered maps: drawing and scrolling, copying cells, and the edits flags
/// apply.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_0802af9c(s32 x, s32 y);
void Func_0802ad84(void);

void Func_0802aa74(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 height = 0;
  s32 z = 0;
  s32 *followedPosition = work->origin;

  if (followedPosition != NULL) {
    s16 *rectList;
    s16 *rect;

    x = *followedPosition++;
    height = *followedPosition++;
    z = *followedPosition++;
    rectList = *(s16 **)((u8 *)work + 0xfc);
    if (rectList != NULL) {
      for (rect = rectList; rect[0] != -1; rect += 4) {
        s32 left = (rect[0] << 16) - 0x780000;
        s32 top = (rect[1] << 16) + height - 0x400000;
        s32 right = (rect[2] << 16) + 0x780000;
        s32 bottom = (rect[3] << 16) + height + 0x600000;

        if (x >= left && z >= top && x <= right && z <= bottom) {
          s32 nearX = left;
          s32 nearZ;

          if (ABS(nearX - x) > ABS(right - x))
            nearX = right;
          nearZ = top;
          if (ABS(nearZ - z) > ABS(bottom - z))
            nearZ = bottom;
          if (ABS(nearX - x) <= ABS(nearZ - z))
            x = nearX;
          else
            z = nearZ;
          break;
        }
      }
      followedPosition[-3] = x;
      followedPosition[-1] = z;
    }
  }
  Func_0802af9c(x, z - height);
  Func_0802ad84();
}

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

/// Draws a row of 16 cell pictures (32 tiles) of a layer's cells into its BG
/// map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
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

#include "bg_scroll.h"

u32 Random16(void);


void Func_0802ad84(void) {
  struct MapWork *work;
  struct MapLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  work = Ram_HeapSlots->mapWork;
  origin = work->origin;
  layer = (struct MapLayerScroll *)work->layers;
  if (origin == NULL)
    return;
  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;

    baseY = *origin - height - 0x600000;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - 0xf00000;
  minY = work->minZ + work->shakeY;
  maxY = work->maxZ - work->shakeY - 0xa00000;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  if (baseX < minX)
    baseX = minX;
  if (baseX > maxX)
    baseX = maxX;
  if (baseY < minY)
    baseY = minY;
  if (baseY > maxY)
    baseY = maxY;
  if (work->shakeX != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseX += Iwram_MulQ16(work->shakeX, shakeRoll);
    work->shakeX = Iwram_MulQ16(work->shakeX, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseY += Iwram_MulQ16(work->shakeY, shakeRoll);
    work->shakeY = Iwram_MulQ16(work->shakeY, work->shakeDecay);
  }
  {
    s32 step = work->viewZ - baseY;

    if (step > 0x100000) {
      baseY = work->viewZ - 0x100000;
      step = work->viewZ - baseY;
    }
    if (step < -0x100000)
      baseY = work->viewZ + 0x100000;
  }
  work->viewX = baseX;
  work->viewZ = baseY;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(work->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(work->viewZ, layer->scaleY);
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX += layer->phaseX;
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY += layer->phaseY;
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    x = (u32)baseX >> 19;
    y = (u32)baseY >> 19;
    if ((layer->x ^ baseX) & 0x80000) {
      if (layer->x < baseX)
        Func_0802ac90(layerIndex, x + 30, y);
      else
        Func_0802ac90(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Func_0802abbc(layerIndex, x, y + 20);
      else
        Func_0802abbc(layerIndex, x, y);
    }
    layer->x = baseX;
    layer->y = baseY;
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
  }
}

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


struct TilePos {
  s32 x, y;
};

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                    u32 width, u32 height) {
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos tile[3];
  s32 i;
  struct TilePos *pos = tile;
  u32 y;
  u32 x;

  // The three layers' visible areas: their scroll positions in cells.
  for (i = 2; i >= 0; i--) {
    pos->x = layer->x >> 20;
    pos->y = layer->y >> 20;
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & 0xfff;
      u32 column, row, mapCell, offset, bytes;

      *dst = (*dst & 0xfffff000) | cell;
      column = x & 15;
      dst++;
      i = 0;
      row = y & 15;
      pos = tile;
      // offset is the cell's place in a 32x32 BG map: 128 bytes a cell row,
      // 4 a cell.
      mapCell = row << 5;
      mapCell += column;
      offset = mapCell * 4;
      // The first layer whose visible area (16x12 cells) holds the cell has
      // the cell picture written to its BG map too, and only that one (a map
      // lays its layers out in different parts of the grid); the layers' BG
      // maps follow each other, 2,048 bytes apart, from screen block 5.
      for (; i < 3; i++) {
        if (pos->x <= (s32)x && pos->x + 16 > (s32)x && pos->y <= (s32)y &&
            pos->y + 12 > (s32)y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screenTile, *cellPictureRows;

          // The cell picture's (8 bytes) top two tiles, and its bottom two
          // 64 bytes further on.
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          bytes = cell * 8;
          cellPictureAddress = (u32)Ram_MapBlocks;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;

          cellPictureAddress = (u32)Ram_MapBlocks + 4;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;
          break;
        }
        // bytes, reused for the step to the next layer's BG map.
        bytes = 2048;
        offset += bytes;
        pos++;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}

void wait(s32 frames);

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

void Map_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapCopyCommand *copyCommand = (struct MapCopyCommand *)steps;
  u32 sourceX = copyCommand->source;
  u32 endMarker;
  u32 firstSourceCopy = sourceX;

  // endMarker set after the copy, which puts the ip copy among its
  // instructions; as an initializer, 2 of 53 instructions differ in tla-en.
  endMarker = 0xffff;
  /* FAKEMATCH(dead): the updates that cancel out emit nothing but end CSE
   * before the comparison, keeping its ip copy while source stays in r0;
   * without them, 5 of 53 instructions differ in tla-en. */
  if (copyCommand) {
    sourceX ^= firstSourceCopy;
    sourceX ^= firstSourceCopy;
  }
  if (firstSourceCopy != endMarker) {
    u32 endMarkerCopy = endMarker;
    s16 *rectangleArgs = (s16 *)(steps + 1);

    while (1) {
      s32 sourceY = rectangleArgs[0];
      s32 width = rectangleArgs[1];
      s32 height = rectangleArgs[2];
      s32 delay = rectangleArgs[3];
      rectangleArgs += 5;

      Map_CopyCellPictureIndicesRect(sourceX, (u16)sourceY, destinationX,
                                     destinationY, (u16)width, (u16)height);
      copyCommand++;
      wait((u16)delay);
      sourceX = copyCommand->source;
      /* FAKEMATCH(dead): updates that cancel out between the load and its
       * copy, so CSE does not swap the load into the copy and source stays in
       * r0; without them, 8 of 53 instructions differ in tla-en. */
      if (copyCommand) {
        sourceX++;
        sourceX--;
      }
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH(dead): updates that cancel out after the copy end CSE's
         * knowledge of source, so the comparison keeps its ip copy; without
         * them, 7 of 53 instructions differ in tla-en. */
        if (copyCommand) {
          sourceX++;
          sourceX--;
        }
        if (nextSourceCopy == endMarkerCopy)
          break;
      }
    }
  }
}

// Map: copy a cell rectangle.

#include "mapcopy.h"

/// Copies a rectangle's cells, including block indices and attributes.
/// srcX, srcY: source top left, in cells.
/// width, height: size in cells.
/// dstX, dstY: destination top left, in cells.
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

extern u8 gMapShapeGrid[];

#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell = (*dstCell & MAP_CELL_TILE_MASK) |
                 (*srcCell++ & MAP_CELL_ATTRIBUTE_MASK);
      dstCell++;
      *dstShape = *srcShape;
      dstShape++;
      srcShape++;
    }
  }
}

/* VIEW(f02aa74): variable gMapCellBuffer as extern u8 gMapCellBuffer[]; f027064 has extern struct MapCell gMapCellBuffer[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/field/resource_slot.c:8266: `gMapCellBuffer__f02aa74' undeclared (first use in this function). */
extern u8 gMapCellBuffer__f02aa74[] __asm__("gMapCellBuffer");

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  s32 y;
  u32 *src = (u32 *)gMapCellBuffer__f02aa74 + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)gMapCellBuffer__f02aa74)[dstY * 128 + dstX];
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos views[3];
  s32 i;
  struct TilePos *view = views;
  s32 x;

  for (i = 2; i >= 0; i--) {
    view->x = layer->x >> 20;
    view->y = layer->y >> 20;
    view++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    {
      for (x = dstX; x < dstX + width; x++) {
        u32 cell = *src++;
        u32 cellPictureId;
        u32 offset;
        u32 row;
        u32 n;
        u32 column;
        u32 scaled;
        u32 mask;

        // cellPictureId holds its mask before the masked cell; as cell & 0xfff,
        // 110 of 148 instructions differ in tla-en.
        cellPictureId = 0xfff;
        mask = 0x7ff;
        /* FAKEMATCH(barrier): an empty asm on mask once it holds 0x7ff, which
         * ranks mask above the destination word in local allocation, so mask
         * keeps r3, the word r2 and 0xfffff800 r1; without it, 23 of 145
         * instructions differ in tla-en. */
        __asm__("" : "+r"(mask));
        cellPictureId = cell & cellPictureId;
        mask &= cell;
        mask |= *dst & 0xfffff800;
        *dst = mask;
        dst++;
        view = views;
        column = x & 15;
        i = 0;
        row = y & 15;
        n = row << 5;
        n += column;
        offset = n * 4;
        for (; i < 3; i += 1) {
          if (view->x <= x && view->x + 16 > x && view->y <= y &&
              view->y + 12 > y) {
            u32 screenAddress, pictureAddress;
            u8 *screenTile, *pictureRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            scaled = cellPictureId * 8;
            pictureAddress = (u32)Ram_MapBlocks;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            *(u32 *)screenTile = *(u32 *)pictureRows;

            pictureAddress = (u32)Ram_MapBlocks + 4;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)pictureRows;
            break;
          }
          // scaled, reused for the step to the next layer's BG map.
          scaled = 2048;
          offset += scaled;
          view++;
        }
      }
      src += 128 - width;
      dst += 128 - width;
    }
  }
}

#include "game_flags.h"

/// Copies a rectangle of map cells as they are, with their shape grid bytes,
/// from (srcX, srcY) to (dstX, dstY), width by height cells.
void Func_0802b590(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell++ = *srcCell++;
      *dstShape++ = *srcShape++;
    }
  }
}

void Map_SetShownLayers(u32 v) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  u32 mask = v & 0xe00;
  u32 flags = state->flags;

  flags = (flags & 0xf1ff) | mask;
  state->flags = flags;
}

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplySetFlagEdits(void) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if (GameFlag_Test(entry & 0xfff) != 0) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (entry & 0x1000)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

u32 Map_ReadEditU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 0);
}

void Map_SetFlagAndApplyAttributeEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 1);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if ((entry & 0xfff) == flag) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (attributes != 0)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size) {
  u8 *header;

  // Indexed as gData_0802f380[map][0], the load reads [table, offset].
  header = Resource_GetTableEntry(
      *(const u16 *)(map * 12 + (u32)gData_0802f380) + (u32)&ResourceId_Map000);
  size[0] = header[0];
  if (header[2] == 0)
    size[1] = 256;
  else
    size[1] = header[2];
  size[2] = header[1];
  if (header[3] == 0)
    size[3] = 256;
  else
    size[3] = header[3];
}

struct MapHeader {
  u8 unk00[44];
  s32 cells;
  s32 shapes;
};
void Tilemap_ConvertBuffer(void);

void Map_LoadCellsAndShapes(void) {
  u8 *source;
  struct MapHeader *header = Resource_GetTableEntry(
      *(const u16 *)(gMainRoutineWork.map * 12 + (u32)gData_0802f380) +
      (u32)&ResourceId_Map000);

  source = (u8 *)header + header->cells;
  Resource_DecodeType01(source, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  source = (u8 *)header + header->shapes;
  Resource_DecodeType01(source, gMapShapeGrid);
}


struct PerspectiveTiles {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 quarterTiles[8];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct PerspectiveTiles *perspective =
      (struct PerspectiveTiles *)Ram_HeapSlots->mapWork;
  u32 *tileOffsets = perspective->tiles;
  u16 *shownQuarter;
  u16 *decodedTile;
  u16 *sourceTiles;
  u8 *quarterCells;
  u16 *screenTiles;
  u32 rowIndex;
  u32 columnIndex;
  u32 cellPictureId;

  x &= 1;
  y &= 1;
  shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shownQuarter)
    return 0;
  *shownQuarter = tile;
  decodedTile = HeapSlot_Alloc(56, 0x400);
  if (tileOffsets[tile] != 0)
    Resource_DecodeByteLzInRam((u8 *)tileOffsets + tileOffsets[tile],
                               decodedTile);
  else
    Iwram_ClearWordsEntry(decodedTile, 0x400);

  sourceTiles = decodedTile;
  // The block table at its fixed address: as the symbol gMapBlocks, its
  // address forms in r2 rather than r1 and update reloads into r3, not r2.
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, 0x84000010, REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  if (update != 0) {
    screenTiles = (u16 *)(0x06004000 + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
      for (columnIndex = 0; columnIndex < 16; columnIndex++) {
        u16 *bottomTiles;

        cellPictureId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + cellPictureId * 4);
        bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + cellPictureId * 4);
        screenTiles[32] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      screenTiles += 48;
    }
  }
  HeapSlot_Release(56);
  return 1;
}
#undef MAP_CELL_TILE_MASK
#undef MAP_CELL_ATTRIBUTE_MASK
