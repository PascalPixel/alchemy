/// @file
/// Layered maps: loading, drawing and scrolling, and copying cells.
// Part of field.c.

#include "../field_internal.h"

// A cell word: its cell picture number in the low 12 bits, the rest its
// attributes.
#define MAPGRID_CELL_PICTURE_ID_MASK 0x00000fff ///< the cell picture number
#define MAPGRID_ATTRIBUTE_MASK 0xfffff000       ///< the cell's attributes

/// Remaps byteCount bytes in place through gSpriteColorRemapTables. Called
/// through SpriteColor_RemapPixelsFar by CommandIcon_Load, which
/// remaps the colours of a command icon's first 768 bytes.
void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount) {
  s32 remaining;
  u8 *cursor;
  u8 *remapTable;

  cursor = bytes;
  remapTable = (u8 *)gSpriteColorRemapTables;
  remaining = byteCount - 1;
  if (remaining != -1) {
    do {
      remaining -= 1;
      *cursor = remapTable[*cursor];
      cursor += 1;
    } while (remaining != -1);
  }
}

/// Unpacks the staged cell picture data into gCellPictures. Byte 1 of the
/// staged buffer (Ram_MapGridCellBuffer) is the method, and the 16-bit data
/// follows from byte 2. size counts the method byte too (an even size means no
/// data). The methods are enum CellPicturePacking.
///
/// Called by MapGrid_LoadMap with the size Decompress_TaggedLz
/// returns; the packed stream is decoded to Ram_MapGridCellBuffer + 1 so that
/// its method byte lands at byte 1 and the halfwords at byte 2 are aligned.
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
  u32 remaining = 128 << 7;
  u32 mask = MAPGRID_CELL_PICTURE_ID_MASK;
  s32 nextPlaceholderCellPicture = -1;

  do {
    u32 value = *cell++;
    u32 cellPictureId = value & mask;

    if (cellPictureId == mask) {
      if (nextPlaceholderCellPicture != cellPictureId) {
        nextPlaceholderCellPicture++;
      }
      value = value + nextPlaceholderCellPicture - cellPictureId;
      cell[-1] = value;
    }
    remaining--;
  } while (remaining != 0);
}

/// Converts the map's cells in place with Tile_ConvertMapGrid, run from RAM.
/// The grid is decoded as three planes: cell behaviors and shapes
/// (16 KB each), then 32 KB of cell picture halfwords (the cell picture number
/// with the layer and priority bits an actor standing there takes). The routine
/// makes each cell picture halfword the low half of a cell word, numbering
/// placeholder cell pictures (0xfff) from 0 up, and interleaves the other two
/// planes into the high halves. The 128 by 128 cell words overwrite the first
/// two planes, so those 32 KB are first copied to a buffer from the EWRAM pool;
/// the buffer and the routine's RAM copy are freed when done. Called by
/// MapGrid_LoadMap once the cell planes are decoded.
void TileMap_ConvertBuffer(void) {
  u8 *savedCells;
  TilemapConvertFn convertGrid;
  u32 savedBytes;

  savedBytes = 32768;
  savedCells = Heap_AllocEwram(savedBytes);
  Iwram_CopyWords(savedCells, Ram_MapGridCellBuffer, savedBytes);
  RamCode_Load(convertGrid, Tile_ConvertMapGrid, Tile_ConvertMapGridCodeSize);
  convertGrid(((u8 *)gMapGridCellBuffer + 32768), Ram_MapGridCellBuffer,
              savedCells);
  Heap_Release(convertGrid);
  Heap_Release(savedCells);
}

/// Copies one tile column of a cell picture into a BG map: its top and
/// bottom tile entries, choosing the left or right half with tileHalf.
static __inline__ void
MapGrid_CopyCellPictureHalfColumn(u32 *cell, u8 *screenMap, u32 screenRowOffset,
                                  u32 screenColumnOffset, u32 tileHalf) {
  u16 *cellPictureEntry;
  u8 *screenTile;
  // A cell picture is 8 bytes of gCellPictures: its top two tile entries, then
  // its bottom two. The cell's low 12 bits times 4 index its first halfword.
  u32 index = ((*cell << 20) >> 18) + tileHalf;

  cellPictureEntry = (u16 *)Ram_CellPictures;
  cellPictureEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *cellPictureEntry;
  cellPictureEntry = (u16 *)(Ram_CellPictures + 4);
  cellPictureEntry += index;
  *(u16 *)(screenTile + 64) = *cellPictureEntry;
}

/// Copies a whole cell picture into a BG map: its top two tile entries
/// and its bottom two, a map row (64 bytes) below.
static __inline__ void MapGrid_CopyCellPictureToScreen(u32 *cell, u8 *screenMap,
                                                       u32 screenRowOffset,
                                                       u32 screenColumnOffset) {
  u32 *cellPictureRows;
  u8 *screenTile;
  u32 index = (*cell << 20) >> 19;

  cellPictureRows = (u32 *)Ram_CellPictures;
  cellPictureRows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *cellPictureRows;
  cellPictureRows = (u32 *)(Ram_CellPictures + 4);
  cellPictureRows += index;
  *(u32 *)(screenTile + 64) = *cellPictureRows;
}

extern const struct MapResources gMaps[];

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
  // XXX (no evidence): why the cells are packed as planes and the treasure
  // spots left unpacked; only Camelot's tools would show what the packing was
  // chosen for.
  s32 cellPictures; ///< the cell pictures, staged and unpacked into
                    ///< gCellPictures
  s32 shapes;       ///< the shapes, into Ram_MapGridShapes
  s32 cells;        ///< the cells, into Ram_MapGridCellBuffer
  s32 animation;    ///< the tileset animation script, or 0 for none
  s32 blend;        ///< the blend script, or 0 for none
  s32 markers; ///< the chests and Psynergy stones (Field_PlaceTreasureSpots)
};

/// Loads layout index from gMaps: its cell pictures, shapes,
/// cells, tileset animations, blend script and treasure spots; sets up the
/// three background layers' origins, parallax and wrap from the layout header;
/// loads the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says
/// they are loaded; and starts the layer scrolling. Returns 2.
///
/// Called through MapGrid_LoadMapFar when a map with a layout
/// starts (index is gMainRoutineWork.map). Allocates and clears the map work
/// in HEAP_SLOT_MAP_GRID; writes the BG palette (keeping the backdrop colour),
/// char blocks 1 to 3, gMapGridAnimTiles, BG1CNT to BG3CNT, REG_MOSAIC,
/// REG_BLDCNT and REG_DISPCNT; and adds MapGrid_UpdateLayerScroll as a
/// task.
s32 MapGrid_LoadMap(s32 index) {
  struct MapResources *mapResources;
  struct MapGridScrollWork *grid;
  struct MapHeader *mapHeader;
  struct MapGridLayerScroll *layer;
  struct MapLayer *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  // Hide the map layers and sprites while their memory is replaced.
  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  ScreenFade_FromBlack(0);
  mapResources = (struct MapResources *)&gMaps[index];
  grid =
      HeapSlot_AllocEwram(HEAP_SLOT_MAP_GRID, sizeof(struct MapGridScrollWork));
  Iwram_ClearWords(grid, sizeof(struct MapGridScrollWork));
  mapHeader = (struct MapHeader *)Resource_GetTableEntry(
      mapResources->resources[0] + (u32)&ResourceId_Map001);
  // The header's offsets count from the header itself. The cell pictures are
  // staged in the cell buffer, which the cell planes then overwrite.
  encodedData = (u8 *)mapHeader + mapHeader->cellPictures;
  TileMap_DecodeStagedBuffer(
      Decompress_TaggedLz(encodedData, Ram_MapGridCellBuffer + 1));
  encodedData = (u8 *)mapHeader + mapHeader->shapes;
  Decompress_TaggedLz(encodedData, Ram_MapGridShapes);
  encodedData = (u8 *)mapHeader + mapHeader->cells;
  Decompress_TaggedLz(encodedData, Ram_MapGridCellBuffer);
  TileMap_ConvertBuffer();
  // The tileset animation and blend scripts are optional (offset 0), and decode
  // into the space after the shapes in Ram_MapGridShapes.
  if (mapHeader->animation != 0) {
    encodedData = (u8 *)mapHeader + mapHeader->animation;
    Decompress_TaggedLz(encodedData, Ram_MapGridShapes + 0x1000);
    TilesetAnim_StartAll((const u16 *)(Ram_MapGridShapes + 0x1000));
  }
  if (mapHeader->blend != 0) {
    encodedData = (u8 *)mapHeader + mapHeader->blend;
    Decompress_TaggedLz(encodedData, Ram_MapGridShapes + 0x1e00);
    BlendScript_Start((u16 *)(Ram_MapGridShapes + 0x1e00));
  }
  // The treasure spot list is not decoded: Field_PlaceTreasureSpots reads it in
  // place through this pointer (MapGridMarkers.markers).
  grid->markers = (u8 *)mapHeader + mapHeader->markers;
  // The header's bounds and origins are in 8-pixel tiles; << 19 makes them
  // 16.16 pixels. The view starts at (0, 0) until the camera is centred.
  grid->minX = mapHeader->cameraBounds[0] << 19;
  grid->minY = mapHeader->cameraBounds[1] << 19;
  grid->maxX = mapHeader->cameraBounds[2] << 19;
  grid->maxY = mapHeader->cameraBounds[3] << 19;
  viewX = &grid->viewX;
  *viewX = 0;
  viewY = &grid->viewY;
  *viewY = 0;
  grid->enabled[0] = mapHeader->priority[0];
  grid->enabled[1] = mapHeader->priority[1];
  grid->enabled[2] = mapHeader->priority[2];
  layer = grid->layers;
  layerSource = mapHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scrollX;
    s32 scrollY;
    s32 baseX;
    s32 baseY;

    layer->offsetX = baseX = x << 19;
    layer->offsetY = baseY = y << 19;
    // Parallax and drift are signed sixteenths; * 4096 makes them 16.16.
    scrollX = layerSource->scrollX * 4096;
    scrollY = layerSource->scrollY * 4096;
    layer->scaleX = scrollX;
    layer->scaleY = scrollY;
    layer->speedX = layerSource->speedX * 4096;
    layer->speedY = layerSource->speedY * 4096;
    layer->maskX = layerSource->periodX;
    layer->maskY = layerSource->periodY;
    layer->phaseX = 0;
    layer->phaseY = 0;
    // The layer's window starts at the cell under its origin tile (a cell is
    // 2x2 tiles, 128 cells to a row).
    layer->cells =
        (struct MapGridCell *)Ram_MapGridCellBuffer + (y >> 1) * 128 + (x >> 1);
    layer->x = Iwram_MulQ16(*viewX, scrollX) + baseX;
    layer->y = Iwram_MulQ16(*viewY, scrollY) + baseY;
    layerSource++;
    layer++;
  }
  // The ground, middle and overhead layers show on BG3, BG2 and BG1, a
  // layer only where its priority
  // is not zero. Each BGCNT takes the priority as it is, its char block from
  // the header and screen blocks 5, 6 and 7.
  grid->flags = DISPCNT_OBJ_ON;
  if (grid->enabled[0] != 0)
    grid->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (grid->enabled[1] != 0)
    grid->flags |= DISPCNT_BG2_ON;
  if (grid->enabled[2] != 0)
    grid->flags |= DISPCNT_BG1_ON;
  displayFlags = grid->enabled[0] | BGCNT_CHAR_BASE(mapHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5);
  REG_BG3CNT = displayFlags;
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
    // One 16 KB buffer takes each resource in turn: the palette (448
    // bytes, 14 of the 16 BG palettes), the three 16 KB tilesets, and
    // last the tileset animations' tiles, which decode straight to
    // gMapGridAnimTiles. Without the memory, nothing is loaded.
    graphicsBuffer = Heap_Alloc(16384);
    if (graphicsBuffer != NULL) {
      // Colour 0 of BG palette 0 is the backdrop; it survives the new palette.
      backdrop = *(s16 *)BG_PLTT;
      Decompress_TaggedLz(Resource_GetTableEntry(mapResources->resources[1] +
                                                 (u32)&ResourceId_Map001),
                          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      /* FAKEMATCH(wrapper): these four copies call the fixed entry itself, past
       * the IWRAM call layer; as Iwram_CopyWords, 103 of 349 instructions
       * differ in tbs-en. */
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, 448);
      Decompress_MoveToFrontLz(
          Resource_GetTableEntry(mapResources->resources[2] +
                                 (u32)&ResourceId_Map001),
          graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, 16384);
      Decompress_MoveToFrontLz(
          Resource_GetTableEntry(mapResources->resources[3] +
                                 (u32)&ResourceId_Map001),
          graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, 16384);
      Decompress_MoveToFrontLz(
          Resource_GetTableEntry(mapResources->resources[4] +
                                 (u32)&ResourceId_Map001),
          graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, 16384);
      Decompress_MoveToFrontLz(
          Resource_GetTableEntry(mapResources->resources[5] +
                                 (u32)&ResourceId_Map001),
          gMapGridAnimTiles);
      Heap_Release(graphicsBuffer);
    }
  }
  // Only BG0, the windows, is shown here; ScreenTransition_Start ORs
  // map->flags into REG_DISPCNT when the map fades in.
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  REG_DISPCNT = displayFlags;
  // UB: Task_Add returns s32; call it through its declared type.
#ifdef UBFIX
  Task_Add(MapGrid_UpdateLayerScroll, TASK_PASS_FRAME + 5);
#else
  /* FAKEMATCH(wrapper): called through a cast to the void call the bytes
   * show, which loads the task function before its priority; as a call by its
   * prototype, 2 of 349 instructions differ in tbs-en. */
  ((void (*)(void (*)(void), s32))Task_Add)(MapGrid_UpdateLayerScroll,
                                            TASK_PASS_FRAME + 5);
#endif
  return 2;
}

/// Centres the camera on the point the map follows (origin: x, height, z),
/// or on (0, 0) when there is none, and scrolls the layers there. Called
/// through MapGrid_CenterOnFollowedPointFar when a layered map starts, by the
/// camera event commands after they move the camera, and by about 35
/// events.
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
/// wraps every 32 tiles. Called by MapGrid_UpdateLayerScroll when a layer
/// scrolls onto a new row of cell pictures.
void MapGrid_RenderCellPictureRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 127) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 127;
  u32 screenColumnOffset = tileX & 30;
  u32 cellPictureIndex;

  // A cell picture is 2x2 tiles, so the BG map's tile row and column are kept
  // even (& 30) and wrap at 32; the grid wraps at 128 cells. Sixteen cell
  // pictures are the BG map's whole width.
  for (cellPictureIndex = 0; cellPictureIndex <= 15; cellPictureIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapGridCellBuffer;
    u32 *cellPictureRows;
    u8 *screenTile;
    u32 cellPictureWordOffset;

    sourceCell += sourceRow + sourceColumn;
    cellPictureWordOffset = (*sourceCell << 20) >> 19;
    cellPictureRows = (u32 *)Ram_CellPictures;
    cellPictureRows += cellPictureWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *cellPictureRows;
    cellPictureRows = (u32 *)(Ram_CellPictures + 4);
    cellPictureRows += cellPictureWordOffset;
    *(u32 *)(screenTile + 64) = *cellPictureRows;
    sourceColumn = (sourceColumn + 1) & 127;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}

/// Draws a column of 11 cell pictures of a layer's cells, one tile wide, into
/// its BG map, starting at tile (tileX, tileY). Called by
/// MapGrid_UpdateLayerScroll when a layer scrolls onto a new column of tiles;
/// tileX's low bit picks the cell picture's left or right half.
void MapGrid_RenderCellPictureColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 127) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 127;
  u32 screenColumnOffset = tileX & 30;
  u32 tileHalf = tileX & 1;
  u32 cellPictureIndex;

  // Eleven cell pictures are 22 tiles: the screen's 20 and the rows partly
  // scrolled in. Both the cells and the BG map wrap.
  for (cellPictureIndex = 0; cellPictureIndex <= 10; cellPictureIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapGridCellBuffer;

    sourceCell += sourceRow + sourceColumn;
    MapGrid_CopyCellPictureHalfColumn(sourceCell, screenMap, screenRowOffset,
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
///
/// Added as a task by MapGrid_LoadMap (priority TASK_PASS_FRAME +
/// 5), and called by MapGrid_CenterOnFollowedPoint. Sets map->viewX and viewY,
/// decays map->shakeX and shakeY, advances each drifting layer's phase, and
/// writes each layer's position and gBgScroll entry.
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
  // The bounds shrink by the shake so a shaken view stays on the map; the
  // right and bottom bounds lose the 240x160 screen. A map smaller than
  // that keeps its top left in view.
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
  grid->viewX = baseX;
  grid->viewY = baseY;
  for (layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(grid->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(grid->viewY, layer->scaleY);
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
    // x and y are in 8-pixel tiles. A new tile column (the 8-pixel bit) or
    // cell picture row (the 16-pixel bit) has come into view: draw it at the
    // leading edge, 30 tiles to the right or 20 below when scrolling that way.
    x = baseX / (8 << 16);
    y = baseY / (8 << 16);
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

/// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds,
/// and redraws every enabled layer's BG map around it. Called by
/// MapGrid_CenterOnFollowedPoint. Sets map->viewX and viewY, and advances each
/// drifting layer's phase by one frame.
void MapGrid_SetCameraCenter(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct MapGridLayerScroll *layer = grid->layers;

  x -= FIELD_VIEW_FOCUS_X;
  y -= FIELD_VIEW_FOCUS_Y;
  CLAMP(x, grid->minX, grid->maxX - FIELD_VIEW_WIDTH);
  CLAMP(y, grid->minY, grid->maxY - FIELD_VIEW_HEIGHT);
  grid->viewX = x;
  grid->viewY = y;

  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    if (grid->enabled[layerIndex]) {
      u32 rows = 22;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellPictureRow, cellPictureColumn;

      x = Iwram_MulQ16(grid->viewX, layer->scaleX);
      y = Iwram_MulQ16(grid->viewY, layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x = (x + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y = (y + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
        // A layer drifting vertically gets its whole 32-row BG map, others
        // the 22 rows (11 cell pictures) a 160-pixel screen can show.
        // XXX (no evidence): why only vertical drift fills the whole map; the
        // only layers that drift vertically are the ground of the Tolbi
        // ship's deck and the haze over Suhalla Desert, and
        // MapGrid_UpdateLayerScroll draws each row as it scrolls in with
        // either fill, so the bytes show no row the shorter fill would miss.
        rows = 32;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      layer++;
      // Divided by 8 pixels in 16.16, x and y become tiles. A cell picture
      // is 2 by 2 tiles: the grid, 128 cell pictures a row, is read at half of
      // them, and the screen map's offsets step 2 tiles and wrap at 32.
      x /= 8 << 16;
      y /= 8 << 16;
      screenMap = BG_SCREEN_BLOCK(5) + (layerIndex << 11);
      sourceRow = ((y / 2) & 127) << 7;
      screenRowOffset = (y & 30) << 5;
      for (cellPictureRow = 0; cellPictureRow < rows / 2; cellPictureRow++) {
        sourceColumn = (x / 2) & 127;
        screenColumnOffset = x & 30;
        for (cellPictureColumn = 0; cellPictureColumn <= 15;
             cellPictureColumn++) {
          u32 *cells = (u32 *)Ram_MapGridCellBuffer;
          cells += sourceRow + sourceColumn;
          MapGrid_CopyCellPictureToScreen(cells, screenMap, screenRowOffset,
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

/// Copies the cell picture numbers of a rectangle of cells from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination's
/// attributes, and redraws the copied cells that are on screen. It does what
/// MapGrid_CopyCellIndexRect does, with the destination before the size.
///
/// Called by MapGrid_PlayMapEdit, and by about 40 events
/// through MapGrid_CopyCellPictureIndicesRectFar.
void MapGrid_CopyCellPictureIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
                                        u32 dstY, u32 width, u32 height) {
  /* FAKEMATCH(temporary): the chains of temporaries set the registers and the
   * schedule of the cell loop; as a plain body (the offset computed once, two
   * word stores), 96 of 141 instructions differ in tbs-en. */
  u32 *sourceCells = (u32 *)Ram_MapGridCellBuffer + (sourceY * 128 + sourceX);
  u32 *destinationCells = (u32 *)Ram_MapGridCellBuffer + (dstY * 128 + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)gMapGridWork[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  u32 y;
  u32 x;

  // The three layers' visible areas: their scroll positions in cells.
  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 sourceCell = *sourceCells++;
      u32 cellPictureId = sourceCell & MAPGRID_CELL_PICTURE_ID_MASK;
      u32 offset;
      u32 tmp3;
      u32 n;
      u32 tmp6;
      u32 tmp9;
      u32 tmp12;
      u32 tmp11;
      u32 tmp13;
      u32 tmp15;
      tmp9 = *destinationCells;
      tmp3 = tmp9 & MAPGRID_ATTRIBUTE_MASK;
      tmp12 = x & 15;
      destinationCells[0] = cellPictureId | tmp3;
      tmp6 = tmp12;
      destinationCells++;
      i = 0;
      tmp15 = y & 15;
      tmp3 = tmp15;
      visibleLayer = visibleLayers;
      tmp13 = tmp3 << 5;
      tmp12 = tmp13;
      n = tmp12;
      n += tmp6;
      tmp13 = 4 * n;
      tmp11 = tmp13;
      tmp6 = tmp11;
      offset = tmp6;
      // The first layer whose visible area (16x12 cells) holds the cell has
      // the cell picture written to its BG map too, and only that one (a map
      // lays its layers out in different parts of the grid); offset is the
      // cell's place in a 32x32 BG map (128 bytes a cell row, 4 a cell), 2,048
      // bytes per layer from screen block 5.
      for (; i < 3; i++) {
        if (visibleLayer->x <= (s32)x && 16 + visibleLayer->x > (s32)x &&
            visibleLayer->y <= (s32)y && visibleLayer->y + 12 > (s32)y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screenTile, *cellPictureRows;

          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          tmp11 = cellPictureId * 8;
          cellPictureAddress = (u32)Ram_CellPictures;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += tmp11;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;

          cellPictureAddress = (u32)Ram_CellPictures + 4;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += tmp11;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;
          break;
        }
        offset += 2048;
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
typedef char MapGridCopyCommandSize
    [(sizeof(struct MapGridCellAnimationStep) == 10) ? 1 : -1];

/// Plays a map edit (struct MapGridCellAnimationStep entries, ended by
/// a source of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of
/// cell picture numbers to (destinationX, destinationY) and waits its delay in
/// frames. Doors and other changing scenery are drawn this way.
///
/// Called by about 30 events through MapGrid_PlayMapEditFar;
/// it returns only when the edit ends, so it blocks the event that calls
/// it.
void MapGrid_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapGridCellAnimationStep *copyCommand =
      (struct MapGridCellAnimationStep *)steps;
  u32 sourceX = copyCommand->source;
  u32 endMarker = MAPGRID_ANIMATION_STEPS_END;
  u32 firstSourceCopy = sourceX;

  /* FAKEMATCH(dead): the updates that cancel out emit nothing but end CSE
   * before the comparison, keeping its ip copy while source stays in r0;
   * without them, 2 of 53 instructions differ in tbs-en. */
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

      MapGrid_CopyCellPictureIndicesRect(sourceX, (u16)sourceY, destinationX,
                                         destinationY, (u16)width, (u16)height);
      copyCommand++;
      wait((u16)delay);
      sourceX = copyCommand->source;
      /* FAKEMATCH(dead): updates that cancel out between the load and its
       * copy, so CSE does not swap the load into the copy and source stays in
       * r0; without them, 8 of 53 instructions differ in tbs-en. */
      if (copyCommand) {
        sourceX++;
        sourceX--;
      }
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH(dead): updates that cancel out after the copy end CSE's
         * knowledge of source, so the comparison keeps its ip copy; without
         * them, 9 of 53 instructions differ in tbs-en. */
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

// Map: copying rectangles of cells.

#define MAPGRID_CELLS ((u32 *)gMapGridCellBuffer)

/// Copies a rectangle of cells (16x16 pixels each) as they are, from
/// (sourceX, sourceY) to (dstX, dstY), width by height cells. Where a copied
/// cell lies in a layer's visible area (16x12 cells), its cell picture's 2x2
/// tiles go to that layer's BG map in VRAM too. Called by about 15 events
/// through MapGrid_CopyCellPictureCellsRectFar.
void MapGrid_CopyCellPictureCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dstX, s32 dstY) {
  /* FAKEMATCH(temporary): the chains of temporaries, tmp2 to tmp17, set the
   * registers, the schedule and the slots of the 36-byte stack frame; as a
   * plain body (the offset computed once, two word stores), 85 of 137
   * instructions differ in tbs-en. */
  s32 y = dstY;
  u32 *sourceCells = (u32 *)Ram_MapGridCellBuffer + (sourceY * 128 + sourceX);
  u32 *destinationCells = &((u32 *)Ram_MapGridCellBuffer)[y * 128 + dstX];
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)*gMapGridWork)->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 cellPictureId;
        u32 offset;
        u32 tmp3;
        u32 *screenTile;
        u32 n;
        u32 *cellPictureRows;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        destinationCells[0] = sourceCell;
        cellPictureId = sourceCell & MAPGRID_CELL_PICTURE_ID_MASK;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        i = 0;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (2,048
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
            // The cell picture's (8 bytes) top two tiles, and its bottom two
            // 64 bytes further on.
            u32 *tmp;
            u32 tmp2;
            u32 tmp4;
            u32 *tmp7;
            u32 tmp10;
            u32 tmp14;
            u32 tmp16;
            u32 bytes = cellPictureId * 8;
            u32 tmp8;
            u32 tmp17;

            cellPictureId = bytes / 8;
            tmp = (u32 *)Ram_CellPictures;
            tmp8 = (u32)BG_SCREEN_BLOCK(5) + offset;
            cellPictureRows = tmp;
            tmp2 = tmp8;
            tmp = (u32 *)tmp2;
            screenTile = tmp;
            cellPictureRows = (u32 *)((u8 *)cellPictureRows + bytes);
            tmp16 = *cellPictureRows;
            tmp14 = tmp16;
            tmp17 = (u32)BG_SCREEN_BLOCK(5) + 64 + offset;
            tmp10 = tmp14;
            *screenTile = tmp10;
            tmp7 = (u32 *)(Ram_CellPictures + 4);
            cellPictureRows = tmp7;
            cellPictureRows = (u32 *)((u8 *)cellPictureRows + bytes);
            tmp8 = tmp17;
            tmp14 = tmp8;
            tmp17 = cellPictureRows[0];
            tmp16 = tmp17;
            tmp10 = tmp16;
            tmp4 = tmp10;
            tmp2 = tmp4;
            tmp4 = tmp14;
            tmp7 = (u32 *)tmp4;
            screenTile = tmp7;
            *screenTile = tmp2;
            break;
          }
          n = 2048; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells += (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

/// Copies only the attributes of a rectangle of cells, from (sourceX, sourceY)
/// to (dstX, dstY), width by height cells, keeping each destination cell's
/// cell picture number. Nothing is redrawn: the attributes (flags, cell
/// behavior and shape) do not show. Called by about 65 events through
/// MapGrid_CopyCellAttributeRectFar.
void MapGrid_CopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY) {
  u32 *destinationCells = MAPGRID_CELLS;
  u32 *sourceCells = destinationCells;
  u16 rowIndex;
  u16 columnIndex;

  destinationCells += (dstY << 7) + dstX;
  sourceCells += (sourceY << 7) + sourceX;

  for (rowIndex = 0; rowIndex < height; rowIndex++) {
    u32 *destinationCell = destinationCells + (rowIndex << 7);
    u32 *sourceCell = sourceCells + (rowIndex << 7);

    for (columnIndex = 0; columnIndex < width; columnIndex++) {
      *destinationCell = (*destinationCell & MAPGRID_CELL_PICTURE_ID_MASK) |
                         (*sourceCell++ & MAPGRID_ATTRIBUTE_MASK);
      destinationCell++;
    }
  }
}

/// Copies only the cell picture numbers of a rectangle of cells, from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination
/// cell's attributes; cells in a visible area update the BG maps too. Called by
/// seven events through MapGrid_CopyCellIndexRectFar.
void MapGrid_CopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  /* FAKEMATCH(temporary): the word and index temporaries set the registers and
   * the 36-byte stack frame; as a plain body (the offset computed once, two
   * word stores), 94 of 142 instructions differ in tbs-en. */
  s32 y;
  u32 *sourceCells = (u32 *)Ram_MapGridCellBuffer + (sourceY * 128 + sourceX);
  u32 *destinationCells = &((u32 *)Ram_MapGridCellBuffer)[dstY * 128 + dstX];
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollWork *)*gMapGridWork)->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  y = dstY;
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 cellPictureId;
        u32 offset;
        u32 tmp3;
        u32 n;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        cellPictureId = sourceCell & MAPGRID_CELL_PICTURE_ID_MASK;
        destinationCells[0] =
            (destinationCells[0] & MAPGRID_ATTRIBUTE_MASK) | cellPictureId;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        i = 0;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (2,048
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
            // The cell picture's (8 bytes) top two tiles, and its bottom two
            // 64 bytes further on.
            u32 screenAddress, cellPictureAddress;
            u8 *screenTile, *cellPictureRows;

            screenAddress = (u32)BG_SCREEN_BLOCK(5);
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            tmp13 = cellPictureId * 8;
            cellPictureAddress = (u32)Ram_CellPictures;
            cellPictureRows = (u8 *)cellPictureAddress;
            cellPictureRows += tmp13;
            *(u32 *)screenTile = *(u32 *)cellPictureRows;

            cellPictureAddress = (u32)Ram_CellPictures + 4;
            cellPictureRows = (u8 *)cellPictureAddress;
            cellPictureRows += tmp13;
            screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)cellPictureRows;
            break;
          }
          n = 2048; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells = sourceCells + (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

/// Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
/// REG_DISPCNT) from layerFlags, keeping its other flags. Called by Lunpa
/// Fortress's event through MapGrid_SetShownLayersFar.
void MapGrid_SetShownLayers(u32 layerFlags) {
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  u32 mask = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
  u32 flags = gridState->flags;

  // A halfword mask: as ~(...) without the cast, the mask in the literal pool
  // is 0xfffff1ff, where the ROM has 0xf1ff.
  flags =
      (flags & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) | mask;
  gridState->flags = flags;
}

#undef MAPGRID_CELLS
#undef MAPGRID_CELL_PICTURE_ID_MASK
#undef MAPGRID_ATTRIBUTE_MASK
