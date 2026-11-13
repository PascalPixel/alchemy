/// @file
/// Layered maps: loading, drawing and scrolling, and copying cells.
// Part of field.c.

#include "../field_internal.h"

// A cell word: its cell picture number in the low 12 bits, the rest its
// attributes.
#define MAPGRID_CELL_PICTURE_ID_MASK 0x00000fff ///< the cell picture number
#define MAPGRID_ATTRIBUTE_MASK 0xfffff000       ///< the cell's attributes
/// The map's 128 by 128 cell words.
#define MAPGRID_CELLS ((u32 *)Ram_MapGridCellBuffer)
/// Where a layout's tileset animation and blend scripts are decoded, in the
/// space after the shapes.
#define MAPGRID_ANIMATION_SCRIPT (Ram_MapGridShapes + 0x1000)
#define MAPGRID_BLEND_SCRIPT (Ram_MapGridShapes + 0x1e00)
/// A layer's visible area in cells: the screen's 15 by 10, and the row and
/// column partly scrolled in.
#define MAPGRID_VISIBLE_WIDTH 16
#define MAPGRID_VISIBLE_HEIGHT 12
/// The bytes between one layer's BG map and the next, from screen block 5.
#define MAPGRID_LAYER_SCREEN_BYTES 2048

/// Remaps byteCount bytes in place through gSpriteColorRemapTables.
void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount) {
  u8 *remapTable = (u8 *)gSpriteColorRemapTables;

  while (byteCount-- != 0) {
    *bytes = remapTable[*bytes];
    bytes++;
  }
}

/// Unpacks the staged cell picture data into gCellPictures.
void TileMap_DecodeStagedBuffer(s32 size) {
  u16 *destinationWords;
  u16 *sourceWords;
  s32 wordCount;
  s32 i;

  sourceWords = (u16 *)(Ram_MapGridCellBuffer + 2);
  destinationWords = (u16 *)gCellPictures;
  wordCount = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapGridCellBuffer)[1]) {
    case CELL_PICTURE_PACKING_RAW:
      for (i = 0; i < wordCount; i++)
        *destinationWords++ = *sourceWords++;
      break;
    case CELL_PICTURE_PACKING_SPLIT_DELTA: {
      // The high bytes, then the low bytes, each word XORed with the last.
      u32 previousWord = 0;
      u8 *highBytes = Ram_MapGridCellBuffer + 2;
      u8 *lowBytes = highBytes + wordCount;

      for (i = 0; i < wordCount; i++) {
        u32 decodedWord = ((*highBytes++ << 8) | *lowBytes++) ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
      }
      break;
    }
    case CELL_PICTURE_PACKING_DELTA: {
      s32 previousWord = 0;

      for (i = 0; i < wordCount; i++) {
        s32 decodedWord = *sourceWords++ ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
      }
      break;
    }
    }
  }
}

typedef void (*TilemapConvertFn)(void *dst, const void *src, const void *saved);

/// Unused: gives each map cell whose cell picture number is 0xfff (a
/// placeholder) a number of its own, counting up from 0 in map order.
UNUSED void TileMap_NumberPlaceholderCellPictures(void) {
  u32 *cell = (u32 *)gMapGridCellBuffer;
  u32 remaining = 128 * 128;
  u32 mask = MAPGRID_CELL_PICTURE_ID_MASK;
  s32 nextPlaceholder = -1;

  for (; remaining != 0; remaining--) {
    u32 value = *cell++;
    u32 cellPictureId = value & mask;

    if (cellPictureId == mask) {
      if (nextPlaceholder != cellPictureId)
        nextPlaceholder++;
      value = value + nextPlaceholder - cellPictureId;
      cell[-1] = value;
    }
  }
}

/// Converts the map's cells in place with Tile_ConvertMapGrid, run from RAM.
void TileMap_ConvertBuffer(void) {
  u8 *savedCells;
  TilemapConvertFn convertGrid;

  savedCells = Heap_AllocEwram(32 * 1024);
  Iwram_CopyWords(savedCells, Ram_MapGridCellBuffer, 32 * 1024);
  RamCode_Load(convertGrid, Tile_ConvertMapGrid, Tile_ConvertMapGridCodeSize);
  convertGrid((u8 *)gMapGridCellBuffer + 32 * 1024, Ram_MapGridCellBuffer,
              savedCells);
  Heap_Release(convertGrid);
  Heap_Release(savedCells);
}

/// Copies one tile column of a cell picture into a BG map: its top and
/// bottom tile entries, choosing the left or right half with tileHalf.
static __inline__ void
MapGrid_CopyCellPictureHalfColumn(u32 *cell, u8 *screenMap, u32 screenRowOffset,
                                  u32 screenColumnOffset, u32 tileHalf) {
  u16 *entry;
  u8 *screenTile;
  // A cell picture is 8 bytes of gCellPictures: its top two tile entries, then
  // its bottom two. The cell's low 12 bits times 4 index its first halfword.
  u32 index = ((*cell << 20) >> 18) + tileHalf;

  /* FAKEMATCH(temporary): entry holds each row's table, then steps to the
   * picture; as one expression, 30 of 81 instructions differ in tbs-en.
   */
  entry = (u16 *)Ram_CellPictures;
  entry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *entry;
  entry = (u16 *)(Ram_CellPictures + 4);
  entry += index;
  *(u16 *)(screenTile + 64) = *entry;
}

/// Copies a whole cell picture into a BG map: its top two tile entries
/// and its bottom two, a map row (64 bytes) below.
static __inline__ void MapGrid_CopyCellPictureToScreen(u32 *cell, u8 *screenMap,
                                                       u32 screenRowOffset,
                                                       u32 screenColumnOffset) {
  u32 *rows;
  u8 *screenTile;
  // The cell's low 12 bits times 2 index the picture's first word.
  u32 index = (*cell << 20) >> 19;

  /* FAKEMATCH(temporary): as in MapGrid_CopyCellPictureHalfColumn, rows
   * steps from each row's table to the picture; as one expression, 35 of
   * 66 instructions differ in tbs-en. */
  rows = (u32 *)Ram_CellPictures;
  rows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *rows;
  rows = (u32 *)(Ram_CellPictures + 4);
  rows += index;
  *(u32 *)(screenTile + 64) = *rows;
}

/// One background layer's setup in a layout's header: its origin, its
/// parallax and drift, and where it wraps.
struct MapLayer {
  u8 x;       ///< 8-pixel tiles
  u8 y;       ///< 8-pixel tiles
  s8 scrollX; ///< sixteenths of the view's motion
  s8 scrollY; ///< sixteenths of the view's motion
  s8 speedX;  ///< 1/16 pixels per frame
  s8 speedY;  ///< 1/16 pixels per frame
  u8 periodX; ///< 8-pixel tiles
  u8 periodY; ///< 8-pixel tiles
};

/// A layout's header: the camera's bounds, the layers' priorities, tile
/// bases and setup, and the offsets of its packed tiles, shapes, tilemap,
/// tileset animation, blend script and treasure spots.
struct MapHeader {
  /// The camera's bounds in 8-pixel tiles: left, top, right and bottom.
  u8 cameraBounds[4];
  /// Each layer's BG priority, 1 to 3; 0 leaves the layer off.
  u8 priority[3];
  u8 charBase[3]; ///< each layer's BG char block
  u16 reserved;   ///< 0 in every layout of both games; nothing reads it
  struct MapLayer layers[3];
  // Offsets from the header's start: type-01 packed data, but for the
  // treasure spots, which are read in place.
  s32 cellPictures; ///< the cell pictures, staged and unpacked into
                    ///< gCellPictures
  s32 shapes;       ///< the shapes, into Ram_MapGridShapes
  s32 cells;        ///< the cells, into Ram_MapGridCellBuffer
  s32 animation;    ///< the tileset animation script, or 0 for none
  s32 blend;        ///< the blend script, or 0 for none
  s32 markers; ///< the chests and Psynergy stones (Field_PlaceTreasureSpots)
};

/// A map resource: one of a layout's packed things, by its row from
/// ResourceId_Map001.
#define MAPGRID_RESOURCE(map, n)                                               \
  Resource_GetTableEntry((map)->resources[n] + (u32)&ResourceId_Map001)

/// Loads layout index from gMaps: its cell pictures, shapes, cells, tileset
/// animations, blend script and treasure spots; sets up the three background
/// layers' origins, parallax and wrap from the layout header; loads the palette
/// and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says they are loaded; and
/// starts the layer scrolling.
s32 MapGrid_LoadMap(s32 index) {
  struct MapResources *mapResources;
  struct MapGridScrollWork *grid;
  struct MapHeader *mapHeader;
  struct MapGridLayerScroll *layer;
  struct MapLayer *layerSource;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *packed;
  s32 *viewX;
  s32 *viewY;
  u32 control;

  // Hide the map layers and sprites while their memory is replaced.
  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  ScreenFade_FromBlack(0);
  mapResources = (struct MapResources *)&gMaps[index];
  grid =
      HeapSlot_AllocEwram(HEAP_SLOT_MAP_GRID, sizeof(struct MapGridScrollWork));
  Iwram_ClearWords(grid, sizeof(struct MapGridScrollWork));
  mapHeader = MAPGRID_RESOURCE(mapResources, 0);
  // The header's offsets count from the header itself.
  packed = (u8 *)mapHeader + mapHeader->cellPictures;
  TileMap_DecodeStagedBuffer(
      Decompress_TaggedLz(packed, Ram_MapGridCellBuffer + 1));
  packed = (u8 *)mapHeader + mapHeader->shapes;
  Decompress_TaggedLz(packed, Ram_MapGridShapes);
  packed = (u8 *)mapHeader + mapHeader->cells;
  Decompress_TaggedLz(packed, Ram_MapGridCellBuffer);
  TileMap_ConvertBuffer();
  // The tileset animation and blend scripts are optional (offset 0).
  if (mapHeader->animation != 0) {
    packed = (u8 *)mapHeader + mapHeader->animation;
    Decompress_TaggedLz(packed, MAPGRID_ANIMATION_SCRIPT);
    TilesetAnim_StartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
  }
  if (mapHeader->blend != 0) {
    packed = (u8 *)mapHeader + mapHeader->blend;
    Decompress_TaggedLz(packed, MAPGRID_BLEND_SCRIPT);
    BlendScript_Start((u16 *)MAPGRID_BLEND_SCRIPT);
  }
  // The treasure spot list is not decoded: Field_PlaceTreasureSpots reads it in
  // place through this pointer (MapGridMarkers.markers).
  grid->markers = (u8 *)mapHeader + mapHeader->markers;
  // The header's bounds and origins are in 8-pixel tiles.
  grid->minX = px(mapHeader->cameraBounds[0] * 8);
  grid->minY = px(mapHeader->cameraBounds[1] * 8);
  grid->maxX = px(mapHeader->cameraBounds[2] * 8);
  grid->maxY = px(mapHeader->cameraBounds[3] * 8);
  viewX = &grid->view[0];
  *viewX = 0;
  viewY = &grid->view[1];
  *viewY = 0;
  grid->enabled[0] = mapHeader->priority[0];
  grid->enabled[1] = mapHeader->priority[1];
  grid->enabled[2] = mapHeader->priority[2];
  layer = grid->layers;
  layerSource = mapHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scaleX, scaleY;
    s32 offsetX, offsetY;

    layer->offsetX = offsetX = px(x * 8);
    layer->offsetY = offsetY = px(y * 8);
    // Parallax and drift are signed sixteenths; * 4096 makes them 16.16.
    scaleX = layerSource->scrollX * 4096;
    scaleY = layerSource->scrollY * 4096;
    layer->scaleX = scaleX;
    layer->scaleY = scaleY;
    layer->speedX = layerSource->speedX * 4096;
    layer->speedY = layerSource->speedY * 4096;
    layer->maskX = layerSource->periodX;
    layer->maskY = layerSource->periodY;
    layer->phaseX = 0;
    layer->phaseY = 0;
    // The layer's window starts at the cell under its origin tile (a cell is
    // 2x2 tiles, 128 cells to a row).
    layer->cells =
        (struct MapGridCell *)Ram_MapGridCellBuffer + (y / 2) * 128 + x / 2;
    layer->x = Iwram_MulQ16(*viewX, scaleX) + offsetX;
    layer->y = Iwram_MulQ16(*viewY, scaleY) + offsetY;
    layerSource++;
    layer++;
  }
  // The ground, middle and overhead layers show on BG3, BG2 and BG1, a layer
  // only where its priority is not zero.
  grid->flags = DISPCNT_OBJ_ON;
  if (grid->enabled[0] != 0)
    grid->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (grid->enabled[1] != 0)
    grid->flags |= DISPCNT_BG2_ON;
  if (grid->enabled[2] != 0)
    grid->flags |= DISPCNT_BG1_ON;
  control = grid->enabled[0] | BGCNT_CHAR_BASE(mapHeader->charBase[0]) |
            BGCNT_SCREEN_BASE(5);
  REG_BG3CNT = control;
  Io_Write16(grid->enabled[1] | BGCNT_CHAR_BASE(mapHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(grid->enabled[2] | BGCNT_CHAR_BASE(mapHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  // A set FLAG_KEEP_LAYER_GRAPHICS skips loading the graphics once, and is
  // cleared.
  if (GameFlag_GetFar(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
    GameFlag_ClearFar(FLAG_KEEP_LAYER_GRAPHICS);
  } else {
    // One 16 KB buffer takes each resource in turn: the palette (448 bytes, 14
    // of the 16 BG palettes), the three 16 KB tilesets, and last the tileset
    // animations' tiles, which decode straight to gMapGridAnimTiles.
    graphicsBuffer = Heap_Alloc(BG_CHAR_SIZE);
    if (graphicsBuffer != NULL) {
      // Colour 0 of BG palette 0 is the backdrop; it survives the new palette.
      backdrop = *(s16 *)PLTT;
      Decompress_TaggedLz(MAPGRID_RESOURCE(mapResources, 1), graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      // These four copies call the IWRAM routine at its fixed address, not
      // through Iwram_CopyWords.
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, 448);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources, 2),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources, 3),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources, 4),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources, 5),
                               gMapGridAnimTiles);
      Heap_Release(graphicsBuffer);
    }
  }
  // Only BG0, the windows, is shown here; ScreenTransition_Start ORs
  // map->flags into REG_DISPCNT when the map fades in.
  control = 0;
  REG_MOSAIC = control;
  REG_BLDCNT = control;
  control = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  /* FAKEMATCH(dead): a once-only block around the store, as at The Lost Age's
   * twin; as a plain store, 4 of 349 instructions differ in tbs-en. */
  do {
    REG_DISPCNT = control;
  } while (0);
  Task_Add(MapGrid_UpdateLayerScroll, TASK_PASS_FRAME + 5);
  return 2;
}

/// Centres the camera on the point the map follows (origin: x, height, z), or
/// on (0, 0) when there is none, and scrolls the layers there.
void MapGrid_CenterOnFollowedPoint(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 x;
  s32 depth;
  s32 height;
  s32 *followedPosition;

  followedPosition = grid->origin;
  x = 0;
  height = 0;
  depth = 0;
  if (followedPosition != NULL) {
    x = *followedPosition++;
    height = *followedPosition++;
    depth = *followedPosition;
  }
  MapGrid_SetCameraCenter(x, depth - height);
  MapGrid_UpdateLayerScroll();
}

/// Draws a row of 16 cell pictures (32 tiles) of a layer's cells into its BG
/// map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles.
void MapGrid_RenderCellPictureRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 sourceRow = ((tileY / 2) & 127) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 127;
  u32 screenColumnOffset = tileX & 30;
  u32 cellPictureIndex;

  // A cell picture is 2x2 tiles, so the BG map's tile row and column are kept
  // even (& 30) and wrap at 32; the grid wraps at 128 cells.
  for (cellPictureIndex = 0; cellPictureIndex < 16; cellPictureIndex++) {
    u32 *cell = MAPGRID_CELLS;

    cell += sourceRow + sourceColumn;
    MapGrid_CopyCellPictureToScreen(cell, screenMap, screenRowOffset,
                                    screenColumnOffset);
    sourceColumn = (sourceColumn + 1) & 127;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}

/// Draws a column of 11 cell pictures of a layer's cells, one tile wide, into
/// its BG map, starting at tile (tileX, tileY).
void MapGrid_RenderCellPictureColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 sourceRow = ((tileY / 2) & 127) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 127;
  u32 screenColumnOffset = tileX & 30;
  u32 tileHalf = tileX & 1;
  u32 cellPictureIndex;

  // Eleven cell pictures are 22 tiles: the screen's 20 and the rows partly
  // scrolled in. Both the cells and the BG map wrap.
  for (cellPictureIndex = 0; cellPictureIndex < 11; cellPictureIndex++) {
    u32 *cell = MAPGRID_CELLS;

    cell += sourceRow + sourceColumn;
    MapGrid_CopyCellPictureHalfColumn(cell, screenMap, screenRowOffset,
                                      screenColumnOffset, tileHalf);

    sourceRow = (sourceRow + 128) & (127 << 7);
    screenRowOffset = (screenRowOffset + 64) & (30 << 5);
  }
}

/// Each frame on a layered map: centres the view on the point the map follows
/// (origin), clamped to the map's bounds and shaken by any decaying screen
/// shake; then scrolls each of the three layers by its parallax scale and
/// automatic scroll (wrapping at its period), draws the cell picture column or
/// row that scrolls into view, and sets its BG scroll.
void MapGrid_UpdateLayerScroll(void) {
  struct MapGridScrollWork *grid;
  struct MapGridLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  grid = gMapGridWork[0];
  origin = grid->origin;
  layer = grid->layers;
  if (origin == 0)
    return;

  // Height projects the followed point above its ground position.
  baseX = *origin++ - FIELD_VIEW_FOCUS_X;
  {
    s32 height = *origin++;
    baseY = *origin - height - FIELD_VIEW_FOCUS_Y;
  }
  // The bounds shrink by the shake so a shaken view stays on the map; the right
  // and bottom bounds lose the 240x160 screen.
  minX = grid->minX + grid->shakeX;
  maxX = grid->maxX - grid->shakeX - FIELD_VIEW_WIDTH;
  minY = grid->minY + grid->shakeY;
  maxY = grid->maxY - grid->shakeY - FIELD_VIEW_HEIGHT;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  CLAMP(baseX, minX, maxX);
  CLAMP(baseY, minY, maxY);

  // A shake offsets the view by the amplitude times the difference of two
  // random fractions (between -1 and 1), then scales the amplitude by
  // shakeDecay for the next frame.
  if (grid->shakeX != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = grid->shakeX;
    baseX += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    grid->shakeX = Iwram_MulQ16(shakeAmplitude, grid->shakeDecay);
  }
  if (grid->shakeY != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = grid->shakeY;
    baseY += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    grid->shakeY = Iwram_MulQ16(shakeAmplitude, grid->shakeDecay);
  }
  grid->view[0] = baseX;
  grid->view[1] = baseY;
  for (layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(grid->view[0], layer->scaleX);
    baseY = Iwram_MulQ16(grid->view[1], layer->scaleY);
    // A drifting layer wraps at its period: maskX and maskY are in 8-pixel
    // tiles, and the mask keeps the pixel and fraction bits below them.
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX = (baseX + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY = (baseY + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    // x and y are in 8-pixel tiles.
    x = baseX / px(8);
    y = baseY / px(8);
    if ((layer->x ^ baseX) & (8 << 16)) {
      if (layer->x < baseX)
        MapGrid_RenderCellPictureColumn(layerIndex, x + 30, y);
      else
        MapGrid_RenderCellPictureColumn(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & (16 << 16)) {
      if (layer->y < baseY)
        MapGrid_RenderCellPictureRow(layerIndex, x, y + 20);
      else
        MapGrid_RenderCellPictureRow(layerIndex, x, y);
    }
    // MAPGRID_LAYER_GROUND scrolls BG3, MAPGRID_LAYER_OVERHEAD BG1.
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
    layer->x = baseX;
    layer->y = baseY;
  }
}

/// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
/// redraws every enabled layer's BG map around it.
void MapGrid_SetCameraCenter(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct MapGridLayerScroll *layer = grid->layers;

  x -= FIELD_VIEW_FOCUS_X;
  y -= FIELD_VIEW_FOCUS_Y;
  CLAMP(x, grid->minX, grid->maxX - FIELD_VIEW_WIDTH);
  CLAMP(y, grid->minY, grid->maxY - FIELD_VIEW_HEIGHT);
  grid->view[0] = x;
  grid->view[1] = y;

  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    if (grid->enabled[layerIndex]) {
      u32 rows = 22;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellPictureRow, cellPictureColumn;

      x = Iwram_MulQ16(grid->view[0], layer->scaleX);
      y = Iwram_MulQ16(grid->view[1], layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x = (x + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y = (y + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
        // A layer drifting vertically gets its whole 32-row BG map, others
        // the 22 rows (11 cell pictures) a 160-pixel screen can show.
        rows = 32;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      layer++;
      // Divided by 8 pixels in 16.16, x and y become tiles.
      x /= px(8);
      y /= px(8);
      screenMap = BG_SCREEN_BLOCK(5) + layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
      sourceRow = ((y / 2) & 127) << 7;
      screenRowOffset = (y & 30) << 5;
      for (cellPictureRow = 0; cellPictureRow < rows / 2; cellPictureRow++) {
        sourceColumn = (x / 2) & 127;
        screenColumnOffset = x & 30;
        for (cellPictureColumn = 0; cellPictureColumn < 16;
             cellPictureColumn++) {
          u32 *cell = MAPGRID_CELLS;

          cell += sourceRow + sourceColumn;
          MapGrid_CopyCellPictureToScreen(cell, screenMap, screenRowOffset,
                                          screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & 127;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + 128) & (127 << 7);
        screenRowOffset = (screenRowOffset + 64) & (30 << 5);
      }
    }
  }
}

/// The top left of a layer's visible area, in cells.
struct MapGridTilePosition {
  s32 x, y;
};

// The three copies below share their redraw: the three layers' visible areas
// (16 by 12 cells from their scroll positions), and for each copied cell the
// first layer whose area holds it, and that one only, has the cell picture's
// top two tile entries and its bottom two (a tile row, 64 bytes, further on)
// written to its BG map; the layers' BG maps follow each other, 2,048 bytes
// apart, from screen block 5, and in each a row of cells is 128 bytes and a
// cell 4.
/// Copies the cell picture numbers of a rectangle of cells from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination's
/// attributes, and redraws the copied cells that are on screen.
void MapGrid_CopyCellPictureIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
                                        u32 dstY, u32 width, u32 height) {
  u32 y;
  u32 *sourceCells = MAPGRID_CELLS + (sourceY * 128 + sourceX);
  u32 *destinationCells = MAPGRID_CELLS + (dstY * 128 + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)gMapGridWork[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  u32 x;

  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellPictureId = cell & MAPGRID_CELL_PICTURE_ID_MASK;
      u32 column, row, place, offset, bytes;

      *destinationCells =
          (*destinationCells & MAPGRID_ATTRIBUTE_MASK) | cellPictureId;
      column = x & 15;
      destinationCells++;
      i = 0;
      row = y & 15;
      visibleLayer = visibleLayers;
      place = row << 5;
      place += column;
      offset = place * 4;
      for (; i < 3; i++) {
        if (visibleLayer->x <= (s32)x && visibleLayer->x + 16 > (s32)x &&
            visibleLayer->y <= (s32)y && visibleLayer->y + 12 > (s32)y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screen, *cellPicture;

          /* FAKEMATCH(temporary): the addresses held as numbers, a step at a
           * time; as plain pointer expressions, 75 of 150 instructions
           * differ in tbs-en. */
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screen = (u8 *)screenAddress;
          screen += offset;
          bytes = cellPictureId * 8;
          cellPictureAddress = (u32)Ram_CellPictures;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          *(u32 *)screen = *(u32 *)cellPicture;
          cellPictureAddress = (u32)Ram_CellPictures + 4;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screen = (u8 *)screenAddress;
          screen += offset;
          *(u32 *)screen = *(u32 *)cellPicture;
          break;
        }
        bytes = 2048;
        offset += bytes;
        visibleLayer++;
      }
    }
    sourceCells += 128 - width;
    destinationCells += 128 - width;
  }
}

/// One step of a map edit: the rectangle of cells to copy from and the
/// wait after it.
struct MapGridCellAnimationStep {
  u16 source; ///< the source rectangle's x in cells, or
              ///< MAPGRID_ANIMATION_STEPS_END
  /// The source rectangle's y, width and height in cells, and the frames to
  /// wait after the copy.
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
STATIC_ASSERT(sizeof(struct MapGridCellAnimationStep) == 10);

/// Plays a map edit (struct MapGridCellAnimationStep entries, ended by a source
/// of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of cell
/// picture numbers to (destinationX, destinationY) and waits its delay in
/// frames.
void MapGrid_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapGridCellAnimationStep *step =
      (struct MapGridCellAnimationStep *)steps;
  u32 source = step->source;
  /* FAKEMATCH(temporary): the end test reads a copy of source, and the updates
   * that cancel out end CSE's knowledge of it; with the plain loop, 23 of 54
   * instructions differ in tbs-en. */
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

/// Copies a rectangle of cells (16x16 pixels each) as they are, from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells.
void MapGrid_CopyCellPictureCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dstX, s32 dstY) {
  s32 y;
  u32 *sourceCells = MAPGRID_CELLS + (sourceY * 128 + sourceX);
  u32 *destinationCells = MAPGRID_CELLS + (dstY * 128 + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)gMapGridWork[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellPictureId;
      u32 column, row, place, offset, bytes;

      *destinationCells = cell;
      cellPictureId = cell & MAPGRID_CELL_PICTURE_ID_MASK;
      column = x & 15;
      destinationCells++;
      i = 0;
      row = y & 15;
      visibleLayer = visibleLayers;
      place = row << 5;
      place += column;
      offset = place * 4;
      for (; i < 3; i++) {
        if (visibleLayer->x <= x && visibleLayer->x + 16 > x &&
            visibleLayer->y <= y && visibleLayer->y + 12 > y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screen, *cellPicture;

          /* FAKEMATCH(temporary): the addresses held as numbers, a step at a
           * time; as plain pointer expressions, 75 of 150 instructions
           * differ in tbs-en. */
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screen = (u8 *)screenAddress;
          screen += offset;
          bytes = cellPictureId * 8;
          cellPictureAddress = (u32)Ram_CellPictures;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          *(u32 *)screen = *(u32 *)cellPicture;
          cellPictureAddress = (u32)Ram_CellPictures + 4;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screen = (u8 *)screenAddress;
          screen += offset;
          *(u32 *)screen = *(u32 *)cellPicture;
          break;
        }
        bytes = 2048;
        offset += bytes;
        visibleLayer++;
      }
    }
    sourceCells += 128 - width;
    destinationCells += 128 - width;
  }
}

/// Copies only the attributes of a rectangle of cells, from (sourceX, sourceY)
/// to (dstX, dstY), width by height cells, keeping each destination cell's cell
/// picture number.
void MapGrid_CopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY) {
  u32 *destinationCells = MAPGRID_CELLS;
  u32 *sourceCells = destinationCells;
  u16 row;
  u16 column;

  destinationCells += dstY * 128 + dstX;
  sourceCells += sourceY * 128 + sourceX;

  for (row = 0; row < height; row++) {
    u32 *destinationCell = destinationCells + row * 128;
    u32 *sourceCell = sourceCells + row * 128;

    for (column = 0; column < width; column++) {
      *destinationCell = (*destinationCell & MAPGRID_CELL_PICTURE_ID_MASK) |
                         (*sourceCell++ & MAPGRID_ATTRIBUTE_MASK);
      destinationCell++;
    }
  }
}

/// Copies only the cell picture numbers of a rectangle of cells, from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination
/// cell's attributes; cells in a visible area update the BG maps too.
void MapGrid_CopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  s32 y;
  u32 *sourceCells = MAPGRID_CELLS + (sourceY * 128 + sourceX);
  u32 *destinationCells = MAPGRID_CELLS + (dstY * 128 + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)gMapGridWork[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellPictureId = cell & MAPGRID_CELL_PICTURE_ID_MASK;
      u32 column, row, place, offset, bytes;

      *destinationCells =
          (*destinationCells & MAPGRID_ATTRIBUTE_MASK) | cellPictureId;
      column = x & 15;
      destinationCells++;
      i = 0;
      row = y & 15;
      visibleLayer = visibleLayers;
      place = row << 5;
      place += column;
      offset = place * 4;
      for (; i < 3; i++) {
        if (visibleLayer->x <= x && visibleLayer->x + 16 > x &&
            visibleLayer->y <= y && visibleLayer->y + 12 > y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screen, *cellPicture;

          /* FAKEMATCH(temporary): the addresses held as numbers, a step at a
           * time; as plain pointer expressions, 75 of 150 instructions
           * differ in tbs-en. */
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screen = (u8 *)screenAddress;
          screen += offset;
          bytes = cellPictureId * 8;
          cellPictureAddress = (u32)Ram_CellPictures;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          *(u32 *)screen = *(u32 *)cellPicture;
          cellPictureAddress = (u32)Ram_CellPictures + 4;
          cellPicture = (u8 *)cellPictureAddress;
          cellPicture += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screen = (u8 *)screenAddress;
          screen += offset;
          *(u32 *)screen = *(u32 *)cellPicture;
          break;
        }
        bytes = 2048;
        offset += bytes;
        visibleLayer++;
      }
    }
    sourceCells += 128 - width;
    destinationCells += 128 - width;
  }
}

/// Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
/// REG_DISPCNT) from layerFlags, keeping its other flags.
void MapGrid_SetShownLayers(u32 layerFlags) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  u32 shown = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
  u32 flags = grid->flags;

  flags = (flags & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
          shown;
  grid->flags = flags;
}

#undef MAPGRID_CELLS
#undef MAPGRID_CELL_PICTURE_ID_MASK
#undef MAPGRID_ATTRIBUTE_MASK
#undef MAPGRID_VISIBLE_WIDTH
#undef MAPGRID_VISIBLE_HEIGHT
#undef MAPGRID_LAYER_SCREEN_BYTES
#undef MAPGRID_RESOURCE
#undef MAPGRID_ANIMATION_SCRIPT
#undef MAPGRID_BLEND_SCRIPT
