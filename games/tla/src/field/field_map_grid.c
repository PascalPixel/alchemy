/// @file
/// Layered maps: decoding their tilemaps and palette, loading, drawing and
/// scrolling, copying cells, and the edits flags apply.
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field.h"
#include "field/map.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/resource_slot.h"
#include "system/ram_buffer.h"
#include "macros.h"
#include "system/io_write.h"
#include "field/flag_call.h"
#include "system/system.h"
#include "system/bg_scroll.h"
#include "field/mapcopy.h"
#include "game/game_flags.h"

// A cell word: its cell picture number in the low 12 bits, the rest its
// attributes.
#define MAPGRID_CELL_PICTURE_ID_MASK 0x00000fff ///< the cell picture number
#define MAPGRID_ATTRIBUTE_MASK 0xfffff000       ///< the cell's attributes
/// The map's 128 by 128 cell words.
#define MAPGRID_CELLS ((u32 *)Ram_MapGridCellBuffer)
/// A layer's visible area in cells: the screen's 15 by 10, and the row and
/// column partly scrolled in.
#define MAPGRID_VISIBLE_WIDTH 16
#define MAPGRID_VISIBLE_HEIGHT 12
/// A layer's BG map in cells, 16 by 16 (32 by 32 tiles), and the bytes of one
/// row of its tiles.
#define MAPGRID_SCREEN_CELLS 16
#define MAPGRID_SCREEN_ROW_BYTES 64

/// Remaps byteCount bytes in place through gSpriteColorRemapTables.
void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount) {
  u8 *remapTable = (u8 *)gSpriteColorRemapTables;

  while (byteCount-- != 0) {
    *bytes = remapTable[*bytes];
    bytes++;
  }
}

// Map: unpack decoded block data into gCellPictures.

/// Decoded buffer [1] selects the method; halfword data starts at [2].
/// size: length including the method byte (even means no data).
/// Method 0: copy unchanged.
/// Method 1: high bytes followed by low bytes; reconstruct by XOR with prior
/// values. Method 2: reconstruct by XOR with prior values.
void TileMap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gCellPictures;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapGridCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

/// Converts the map cell buffer's 32 KB in place with the ARM routine copied
/// to RAM, keeping a copy of the buffer for it to read.
void TileMap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 32768;
  saved = Heap_AllocEwram(size);
  Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Render_ConvertMapCode, Render_ConvertMapCodeSize);
  routine(((u8 *)gMapGridCellBuffer + 32768), Ram_MapCellBuffer, saved);
  Heap_Release(routine);
  Heap_Release(saved);
}

/// A layout's resource, counted in gMaps from the empty map.
#define MAPGRID_RESOURCE(offset)                                               \
  Resource_GetTableEntry((offset) + RESOURCE_EMPTY_MAP)

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void Map_CopyPalette(s32 map, void *destination) {
  const struct MapResources *entry;
  u16 *buffer;
  s16 backdrop;

  entry = &gMaps[map];
  buffer = Heap_Alloc(BG_PLTT_SIZE);
  backdrop = *(s16 *)BG_PLTT;
  Decompress_TaggedLz(MAPGRID_RESOURCE(entry->palette), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, DMA_COPY_WORDS(448 / 4), REG_DMA3);
  Heap_Release(buffer);
}

/// Loads layout index from gMaps: its cell pictures, collision (cleared to -1
/// first), cells, shapes, tileset animations, blend script and script; sets up
/// the three background layers' origins, parallax and wrap from the layout
/// header; loads the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS
/// says they are loaded; and starts the layer scrolling
/// (MapGrid_UpdateLayerScroll).
s32 MapGrid_LoadMap(s32 index) {
  struct MapResources *sceneResources;
  struct MapWork *work;
  struct MapHeader *sceneHeader;
  struct MapLayer *layer;
  struct MapHeaderLayer *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  ScreenFade_FromBlack(0);
  sceneResources = (struct MapResources *)&gMaps[index];
  // A field map's work ends after its three layers; the world map's goes on.
  work = HeapSlot_AllocEwram(HEAP_SLOT_MAP_GRID, MAPGRID_FIELD_WORK_SIZE);
  Iwram_ClearWordsEntry(work, MAPGRID_FIELD_WORK_SIZE);
  sceneHeader = (struct MapHeader *)MAPGRID_RESOURCE(sceneResources->header);
  encodedData = (u8 *)sceneHeader + sceneHeader->cellPictures;
  TileMap_DecodeStagedBuffer(
      Decompress_TaggedLz(encodedData, Ram_MapCellBuffer + 1));
  Dma_Fill32(-1, Ram_MapCollision, 8192);
  encodedData = (u8 *)sceneHeader + sceneHeader->collision;
  Decompress_TaggedLz(encodedData, Ram_MapCollision);
  encodedData = (u8 *)sceneHeader + sceneHeader->cells;
  Decompress_TaggedLz(encodedData, Ram_MapCellBuffer);
  TileMap_ConvertBuffer();
  encodedData = (u8 *)sceneHeader + sceneHeader->shapes;
  Decompress_TaggedLz(encodedData, gMapShapeGrid);
  if (sceneHeader->animation != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->animation;
    Decompress_TaggedLz(encodedData, MAPGRID_ANIMATION_SCRIPT);
    TilesetAnim_StartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
  }
  if (sceneHeader->blend != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->blend;
    Decompress_TaggedLz(encodedData, MAPGRID_BLEND_SCRIPT);
    BlendScript_Start((u16 *)MAPGRID_BLEND_SCRIPT);
  }
  encodedData = (u8 *)sceneHeader + sceneHeader->script;
  Decompress_TaggedLz(encodedData, Ram_MapCollision + 8192);
  work->minX = toFIXED(sceneHeader->cameraBounds[0] * 8);
  work->minZ = toFIXED(sceneHeader->cameraBounds[1] * 8);
  work->maxX = toFIXED(sceneHeader->cameraBounds[2] * 8);
  work->maxZ = toFIXED(sceneHeader->cameraBounds[3] * 8);
  viewX = &work->viewX;
  *viewX = 0;
  viewY = &work->viewZ;
  *viewY = 0;
  work->enabled[0] = sceneHeader->priority[0];
  work->enabled[1] = sceneHeader->priority[1];
  work->enabled[2] = sceneHeader->priority[2];
  work->basePriority = 2;
  if (work->maxX == 0)
    work->maxX = toFIXED(MAPGRID_SIZE * MAPGRID_CELL_PIXELS);
  if (work->maxZ == 0)
    work->maxZ = toFIXED(MAPGRID_SIZE * MAPGRID_CELL_PIXELS);
  layer = work->layers;
  layerSource = sceneHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scaleX;
    s32 cell;
    u8 *shapeGrid;
    u32 maskY;

    layer->offsetX = toFIXED(x * 8);
    layer->offsetY = toFIXED(y * 8);
    layer->scaleY = layerSource->scrollY * 4096;
    layer->speedX = layerSource->speedX * 4096;
    layer->speedY = layerSource->speedY * 4096;
    layer->maskX = layerSource->periodX & 127;
    // Read before the phase stores, which the original makes after it.
    maskY = layerSource->periodY & 127;
    layer->phaseX = 0;
    layer->phaseY = 0;
    layer->maskY = maskY << 7;
    x >>= 1;
    y = (y >> 1) * 128;
    layer->originY = y;
    scaleX = layerSource->scrollX * 4096;
    cell = y + x;
    shapeGrid = gMapShapeGrid;
    layer->cells = (struct MapCell *)Ram_MapCellBuffer + cell;
    layer->scaleX = scaleX;
    layer->originX = x;
    layer->terrainIds = shapeGrid + cell;
    layer->x = Iwram_MulFX(*viewX, scaleX) + layer->offsetX;
    layer->y = Iwram_MulFX(*viewY, layer->scaleY) + layer->offsetY;
    layerSource++;
    layer++;
  }
  work->dispcnt = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->dispcnt = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->dispcnt |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->dispcnt |= DISPCNT_BG1_ON;
  Io_Write16(work->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5),
             &REG_BG3CNT);
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_Get(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
    GameFlag_Clear(FLAG_KEEP_LAYER_GRAPHICS);
  } else {
    graphicsBuffer = Heap_Alloc(BG_CHAR_SIZE);
    if (graphicsBuffer != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Decompress_TaggedLz(MAPGRID_RESOURCE(sceneResources->palette),
                          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, 448);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(sceneResources->tiles[0]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(sceneResources->tiles[1]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(sceneResources->tiles[2]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(sceneResources->animationTiles),
                               gMapGridAnimTiles);
      Heap_Release(graphicsBuffer);
    }
  }
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  Io_Write16(displayFlags, &REG_DISPCNT);
  Task_Add(MapGrid_UpdateLayerScroll, TASK_PASS_FRAME + 5);
  return 2;
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// Centres the camera on the point the map follows (origin: x, height, z), or
/// on (0, 0) when there is none, and scrolls the layers there.
void MapGrid_CenterOnFollowedPoint(void) {
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  s32 x = 0;
  s32 height = 0;
  s32 z = 0;
  s32 *followedPosition = work->origin;

  if (followedPosition != NULL) {
    const s16 *rectList;
    const s16 *rect;

    x = *followedPosition++;
    height = *followedPosition++;
    z = *followedPosition++;
    rectList = work->cameraBoxes;
    if (rectList != NULL) {
      for (rect = rectList; rect[0] != -1; rect += 4) {
        FIXED left = toFIXED(rect[0]) - toFIXED(120);
        FIXED top = toFIXED(rect[1]) + height - toFIXED(64);
        FIXED right = toFIXED(rect[2]) + toFIXED(120);
        FIXED bottom = toFIXED(rect[3]) + height + toFIXED(96);

        if (x >= left && z >= top && x <= right && z <= bottom) {
          FIXED nearX = left;
          FIXED nearZ;

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
  MapGrid_SetCameraCenter(x, z - height);
  MapGrid_UpdateLayerScroll();
}

/// Draws a row of 16 cell pictures (32 tiles) of a layer's cells into its BG
/// map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles and the cells wrap at the layer's window, which The
/// Broken Seal's routine does not have. Called by
/// MapGrid_UpdateLayerScroll when a layer scrolls onto a new row of cell
/// pictures.
void MapGrid_RenderCellPictureRow(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *window = &map->layers[layer];
  u32 originY = window->originY;
  u8 *screenMap = (u8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 maskY = window->maskY;
  u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 cellPictureIndex;

  // A window narrower than the 128 by 128 cells starts at its origin.
  if (maskY != MAPGRID_ROW_MASK) {
    sourceRow = (sourceRow - originY) & maskY;
    cells += originY;
  }
  maskX = window->maskX;
  sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
  originX = window->originX;
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != MAPGRID_SIZE - 1) {
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

/// Copies one column (tileHalf 0 or 1) of a cell's 2 by 2 picture tiles into
/// the screen map at the row and column offsets.
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

/// Draws a column of 11 cell pictures of a layer's cells, one tile wide, into
/// its BG map from tile (tileX, tileY), as The Broken Seal's routine of the
/// same name; tileX's low bit picks the picture's left or right half.
void MapGrid_RenderCellPictureColumn(u32 layer, s32 tileX, s32 tileY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *window = &map->layers[layer];
  u8 *screenMap = (u8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 maskY = window->maskY;
  u32 originY = window->originY;
  u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn;
  u32 screenColumnOffset;
  u32 maskX;
  u32 originX;
  u32 tileHalf;
  u32 cellPictureIndex;

  if (maskY != MAPGRID_ROW_MASK) {
    sourceRow = (sourceRow - originY) & maskY;
    cells = (u32 *)Ram_MapCellBuffer + originY;
  }
  maskX = window->maskX;
  originX = window->originX;
  sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
  screenColumnOffset = tileX & 30;
  sourceColumn &= maskX;
  if (maskX != MAPGRID_SIZE - 1) {
    sourceColumn = (sourceColumn - originX) & maskX;
    cells += originX;
  }
  tileHalf = tileX & 1;
  for (cellPictureIndex = 0; cellPictureIndex <= 10; cellPictureIndex++) {
    CopyCellPictureHalfColumn(cells + sourceRow + sourceColumn, screenMap,
                              screenRowOffset, screenColumnOffset, tileHalf);
    sourceRow = (sourceRow + MAPGRID_SIZE) & maskY;
    screenRowOffset = (screenRowOffset + 64) & (30 << 5);
  }
}

/// Each frame on a layered map: centres the view on the point the map follows
/// (origin), clamped to the map's bounds and shaken by any decaying screen
/// shake; then scrolls each of the three layers by its parallax scale and
/// automatic scroll (wrapping at its period), draws the cell picture column or
/// row that scrolls into view, and sets its BG scroll.
void MapGrid_UpdateLayerScroll(void) {
  struct MapWork *work;
  struct MapLayer *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  work = Ram_HeapSlots->mapGrid;
  origin = work->origin;
  layer = work->layers;
  if (origin == 0)
    return;
  baseX = *origin++ - FIELD_VIEW_FOCUS_X;
  {
    s32 height = *origin++;

    baseY = *origin - height - FIELD_VIEW_FOCUS_Y;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - FIELD_VIEW_WIDTH;
  minY = work->minZ + work->shakeY;
  maxY = work->maxZ - work->shakeY - FIELD_VIEW_HEIGHT;
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
    baseX += Iwram_MulFX(work->shakeX, shakeRoll);
    work->shakeX = Iwram_MulFX(work->shakeX, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRoll = Random16();

    shakeRoll -= Random16();
    baseY += Iwram_MulFX(work->shakeY, shakeRoll);
    work->shakeY = Iwram_MulFX(work->shakeY, work->shakeDecay);
  }
  {
    s32 step = work->viewZ - baseY;

    if (step > toFIXED(16)) {
      baseY = work->viewZ - toFIXED(16);
      step = work->viewZ - baseY;
    }
    if (step < -toFIXED(16))
      baseY = work->viewZ + toFIXED(16);
  }
  work->viewX = baseX;
  work->viewZ = baseY;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++, layer++) {
    baseX = Iwram_MulFX(work->viewX, layer->scaleX);
    baseY = Iwram_MulFX(work->viewZ, layer->scaleY);
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
    if ((layer->x ^ baseX) & toFIXED(8)) {
      if (layer->x < baseX)
        MapGrid_RenderCellPictureColumn(layerIndex, x + DISPLAY_WIDTH / 8, y);
      else
        MapGrid_RenderCellPictureColumn(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & toFIXED(16)) {
      if (layer->y < baseY)
        MapGrid_RenderCellPictureRow(layerIndex, x, y + DISPLAY_HEIGHT / 8);
      else
        MapGrid_RenderCellPictureRow(layerIndex, x, y);
    }
    layer->x = baseX;
    layer->y = baseY;
    gBgScroll[3 - layerIndex].x = FIXEDtoINT(baseX);
    gBgScroll[3 - layerIndex].y = FIXEDtoINT(baseY);
  }
}

/// Copies a cell's 2 by 2 picture tiles into the screen map at the row and
/// column offsets.
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

/// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
/// redraws every enabled layer's BG map around it.
void MapGrid_SetCameraCenter(FIXED x, FIXED y) {
  u32 layerIndex;
  u8 *screenMap;
  u32 *cells;
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = work->layers;
  u32 originX;

  x -= FIELD_VIEW_FOCUS_X;
  y -= FIELD_VIEW_FOCUS_Y;
  if (x < work->minX)
    x = work->minX;
  if (x > work->maxX - FIELD_VIEW_WIDTH)
    x = work->maxX - FIELD_VIEW_WIDTH;
  if (y < work->minZ)
    y = work->minZ;
  if (y > work->maxZ - FIELD_VIEW_HEIGHT)
    y = work->maxZ - FIELD_VIEW_HEIGHT;
  work->viewX = x;
  work->viewZ = y;
  for (layerIndex = 0; layerIndex <= 2; layerIndex++) {
    if (work->enabled[layerIndex] != 0) {
      u32 maskX, maskY, originY;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellPictureRow, cellPictureColumn;

      x = Iwram_MulFX(work->viewX, layer->scaleX);
      y = Iwram_MulFX(work->viewZ, layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x += layer->phaseX;
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y += layer->phaseY;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      x /= toFIXED(8);
      y /= toFIXED(8);
      screenMap =
          (u8 *)BG_SCREEN_BLOCK(5) + layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
      maskY = layer->maskY;
      maskX = layer->maskX;
      originY = layer->originY;
      originX = layer->originX;
      cells = (u32 *)Ram_MapCellBuffer;
      if (maskX != MAPGRID_SIZE - 1)
        cells = (u32 *)Ram_MapCellBuffer + originX;
      if (maskY != MAPGRID_ROW_MASK)
        cells += originY;
      sourceRow = ((y / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
      screenRowOffset = (y & 30) << 5;
      if (maskY != MAPGRID_ROW_MASK)
        sourceRow = (sourceRow - originY) & maskY;
      for (cellPictureRow = 0; cellPictureRow <= 10; cellPictureRow++) {
        sourceColumn = (x / 2) & (MAPGRID_SIZE - 1);
        screenColumnOffset = x & 30;
        if (maskX != MAPGRID_SIZE - 1)
          sourceColumn = (sourceColumn - originX) & maskX;
        for (cellPictureColumn = 0; cellPictureColumn <= 15;
             cellPictureColumn++) {
          CopyCellPictureToScreen(cells + sourceRow + sourceColumn, screenMap,
                                  screenRowOffset, screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & maskX;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + MAPGRID_SIZE) & maskY;
        screenRowOffset = (screenRowOffset + 64) & (30 << 5);
      }
      layer++;
    }
  }
}

/// Copies the cell picture numbers of a rectangle of cells from (srcX, srcY) to
/// (dstX, dstY), width by height cells, keeping each destination's attributes,
/// and redraws the copied cells that are on screen.
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                        u32 width, u32 height) {
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * MAPGRID_SIZE + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * MAPGRID_SIZE + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapGrid->layers;
  struct MapGridTilePosition tile[3];
  s32 i;
  struct MapGridTilePosition *pos = tile;
  u32 y;
  u32 x;

  // The three layers' visible areas: their scroll positions in cells.
  for (i = 0; i < 3; i++) {
    pos->x = FIELD_CELL(layer->x);
    pos->y = FIELD_CELL(layer->y);
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & MAPGRID_CELL_PICTURE_ID_MASK;
      u32 column, row, mapCell, offset, bytes;

      *dst = (*dst & MAPGRID_ATTRIBUTE_MASK) | cell;
      column = x & (MAPGRID_SCREEN_CELLS - 1);
      dst++;
      i = 0;
      row = y & (MAPGRID_SCREEN_CELLS - 1);
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
        if (pos->x <= (s32)x && pos->x + MAPGRID_VISIBLE_WIDTH > (s32)x &&
            pos->y <= (s32)y && pos->y + MAPGRID_VISIBLE_HEIGHT > (s32)y) {
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
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;
          break;
        }
        // bytes, reused for the step to the next layer's BG map.
        bytes = MAPGRID_LAYER_SCREEN_BYTES;
        offset += bytes;
        pos++;
      }
    }
    src += MAPGRID_SIZE - width;
    dst += MAPGRID_SIZE - width;
  }
}

/// Plays a map edit (struct MapGridCellAnimationStep entries, ended by a source
/// of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of cell
/// picture numbers to (destinationX, destinationY) and waits its delay in
/// frames.
void MapGrid_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapGridCellAnimationStep *step =
      (struct MapGridCellAnimationStep *)steps;
  u32 source = step->source;
  /* FAKEMATCH(temporary): the end test reads a copy of source, with updates
   * that cancel out; as if (source != MAPGRID_ANIMATION_STEPS_END). */
  u32 test = source;

  if (step) {
    source ^= test;
    source ^= test;
  }
  if (test != MAPGRID_ANIMATION_STEPS_END) {
    u32 end = MAPGRID_ANIMATION_STEPS_END;
    // Each step's rectangle and delay, read a step at a time beside it.
    s16 *rectangle = (s16 *)(steps + 1);

    while (1) {
      s32 sourceY = rectangle[0];
      s32 width = rectangle[1];
      s32 height = rectangle[2];
      s32 delay = rectangle[3];

      rectangle += 5;
      MapGrid_CopyCellPictureIndicesRect(source, (u16)sourceY, destinationX,
                                         destinationY, (u16)width, (u16)height);
      step++;
      wait((u16)delay);
      source = step->source;
      if (step) {
        source++;
        source--;
      }
      {
        u32 next = source;

        if (step) {
          source++;
          source--;
        }
        if (next == end)
          break;
      }
    }
  }
}

// Map: copy a cell rectangle.

/// Copies a rectangle's cells, including block indices and attributes.
/// srcX, srcY: source top left, in cells.
/// width, height: size in cells.
/// dstX, dstY: destination top left, in cells.
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  MapGrid_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

/// Copies the attributes and shapes of a rectangle of cells from (srcX, srcY)
/// to (dstX, dstY), width by height cells, keeping each destination's
/// picture.
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

/// Copies a rectangle of cells (16x16 pixels each) as they are, from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells.
void MapGrid_CopyCellPictureCellsRect(s32 srcX, s32 srcY, s32 width, s32 height,
                                      s32 dstX, s32 dstY) {
  s32 y;
  u32 *src = (u32 *)gMapGridCellBuffer + (srcY * MAPGRID_SIZE + srcX);
  u32 *dst = &((u32 *)gMapGridCellBuffer)[dstY * MAPGRID_SIZE + dstX];
  struct MapLayer *layer = Ram_HeapSlots->mapGrid->layers;
  struct MapGridTilePosition views[3];
  s32 i;
  struct MapGridTilePosition *view = views;
  s32 x;

  for (i = 0; i < 3; i++) {
    view->x = FIELD_CELL(layer->x);
    view->y = FIELD_CELL(layer->y);
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
        u32 merged;

        cellPictureId = cell & MAPGRID_CELL_PICTURE_ID_MASK;
        // The cell takes the low 11 bits of the source and keeps its own above.
        merged = cell & 0x7ff;
        merged |= *dst & 0xfffff800;
        *dst = merged;
        dst++;
        view = views;
        column = x & (MAPGRID_SCREEN_CELLS - 1);
        i = 0;
        row = y & (MAPGRID_SCREEN_CELLS - 1);
        n = row << 5;
        n += column;
        offset = n * 4;
        for (; i < 3; i += 1) {
          if (view->x <= x && view->x + MAPGRID_VISIBLE_WIDTH > x &&
              view->y <= y && view->y + MAPGRID_VISIBLE_HEIGHT > y) {
            u32 screenAddress, pictureAddress;
            u8 *screenTile, *pictureRows;

            screenAddress = (u32)BG_SCREEN_BLOCK(5);
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
            screenAddress = (u32)BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)pictureRows;
            break;
          }
          // scaled, reused for the step to the next layer's BG map.
          scaled = MAPGRID_LAYER_SCREEN_BYTES;
          offset += scaled;
          view++;
        }
      }
      src += MAPGRID_SIZE - width;
      dst += MAPGRID_SIZE - width;
    }
  }
}

/// Copies a rectangle of map cells as they are, with their shape grid bytes,
/// from (srcX, srcY) to (dstX, dstY), width by height cells.
void Map_CopyRawCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                          s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY * MAPGRID_SIZE) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY * MAPGRID_SIZE) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + row * MAPGRID_SIZE;
    u32 *srcCell = cells + row * MAPGRID_SIZE;
    u8 *dstShape = dstShapes + row * MAPGRID_SIZE;
    u8 *srcShape = srcShapes + row * MAPGRID_SIZE;

    for (col = 0; col < width; col++) {
      *dstCell++ = *srcCell++;
      *dstShape++ = *srcShape++;
    }
  }
}

/// Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
/// REG_DISPCNT) from layerFlags, keeping its other flags.
void MapGrid_SetShownLayers(u32 layerFlags) {
  struct MapWork *state = Ram_HeapSlots->mapGrid;
  u32 shown = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
  u32 dispcnt = state->dispcnt;

  dispcnt =
      (dispcnt & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      shown;
  state->dispcnt = dispcnt;
}

// A map edit's flag word: the flag in its low 12 bits, and a bit that makes it
// copy the cells' attributes rather than their pictures.
#define MAP_EDIT_FLAG_MASK 0xfff
#define MAP_EDIT_ATTRIBUTES 0x1000

/// Plays every edit of the map whose flag is set, as Map_ApplyFlagEdits does,
/// bit 12 of an edit's flag word choosing attributes over pictures.
void Map_ApplySetFlagEdits(void) {
  u8 *cursor = gMapSceneEditsOffset;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gMapScene + (u16)Map_ReadEditU16(&cursor);
  for (;;) {
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
      return;
    if (GameFlag_Get(entry & MAP_EDIT_FLAG_MASK) != 0) {
      srcX = *cursor++;
      srcY = *cursor++;
      dstX = *cursor++;
      dstY = *cursor++;
      width = *cursor++;
      height = *cursor++;
      if (entry & MAP_EDIT_ATTRIBUTES)
        Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
      else
        MapGrid_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width,
                                           height);
    } else {
      cursor += 6;
    }
  }
}

/// Reads the little-endian halfword at *cursor, which may be unaligned, and
/// moves the cursor past it.
u32 Map_ReadEditU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

/// Sets flag (its low 12 bits) and plays its map edits on the cells' pictures.
void Map_SetFlagAndApplyEdits(s32 flag) {
  flag &= MAP_EDIT_FLAG_MASK;
  GameFlag_Set(flag);
  Map_ApplyFlagEdits(flag, 0);
}

/// Sets flag (its low 12 bits) and plays its map edits on the cells'
/// attributes.
void Map_SetFlagAndApplyAttributeEdits(s32 flag) {
  flag &= MAP_EDIT_FLAG_MASK;
  GameFlag_Set(flag);
  Map_ApplyFlagEdits(flag, 1);
}

/// Plays the map's edits for flag: each copies a rectangle of cells (their
/// pictures, or with attributes their attributes and shapes) from one place
/// of the map to another. The map's extra data lists the edits, each a flag
/// then source x and y, destination x and y, width and height in bytes, ended
/// by 0xffff.
void Map_ApplyFlagEdits(s32 flag, s32 attributes) {
  u8 *cursor = gMapSceneEditsOffset;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gMapScene + (u16)Map_ReadEditU16(&cursor);
  for (;;) {
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
      return;
    if ((entry & MAP_EDIT_FLAG_MASK) == flag) {
      srcX = *cursor++;
      srcY = *cursor++;
      dstX = *cursor++;
      dstY = *cursor++;
      width = *cursor++;
      height = *cursor++;
      if (attributes != 0)
        Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
      else
        MapGrid_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width,
                                           height);
    } else {
      cursor += 6;
    }
  }
}

/// Reads map's four header extents: size[0] and size[2] from its first two
/// bytes, size[1] and size[3] from the next two, where 0 means 256.
void Map_GetHeaderSize(s32 map, s32 *size) {
  u8 *header;

  // Indexed as gMaps[map].header, the load reads [table, offset].
  header = MAPGRID_RESOURCE(
      *(const u16 *)(map * sizeof(struct MapResources) + (u32)gMaps));
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

/// Decodes the current map's cells into the map cell buffer and converts them,
/// then decodes its cell shapes into gMapShapeGrid.
void Map_LoadCellsAndShapes(void) {
  u8 *source;
  // Through a pointer: indexed as the array, the table's address loads first.
  struct MapHeader *header = MAPGRID_RESOURCE(
      ((const struct MapResources *)gMaps)[gMainRoutineWork.mapId].header);

  source = (u8 *)header + header->cells;
  Decompress_TaggedLz(source, Ram_MapCellBuffer);
  TileMap_ConvertBuffer();
  source = (u8 *)header + header->shapes;
  Decompress_TaggedLz(source, gMapShapeGrid);
}

/// Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's cell
/// picture buffer, decompressing it from the area's graphics, and with update
/// set also draws it into the layer's BG map.
s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
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
  decodedTile = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);
  if (tileOffsets[tile] != 0)
    Decompress_ByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);
  else
    Iwram_ClearWordsEntry(decodedTile, 1024);

  sourceTiles = decodedTile;
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, DMA_COPY_WORDS(16), REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  if (update != 0) {
    screenTiles =
        (u16 *)((u32)BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
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
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  return 1;
}
#undef MAP_CELL_TILE_MASK
#undef MAP_CELL_ATTRIBUTE_MASK
