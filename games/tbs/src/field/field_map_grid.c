/// @file
/// Layered maps: loading, drawing and scrolling, and copying cells.
// Part of field.c.

#include "field_internal.h"

/// A cell word's attributes, the bits above the number of its cell tiles
/// (MAPGRID_CELL_TILES_ID_MASK).
#define MAPGRID_ATTRIBUTE_MASK 0xfffff000
/// The shift that leaves a cell word's cell tiles number on top, its attributes
/// shifted out. Shifted back down by one less it is the number times 2, the
/// cell tiles' offset in words in gCellTiles (a word for each of its two rows
/// of 2 tiles); by two less, times 4, its offset in halfwords.
#define MAPGRID_CELL_TILES_SHIFT 20
/// The map's 128 by 128 cells, each read whole as its word (a struct
/// MapGridCell's four bytes).
#define MAPGRID_CELL_WORDS ((u32 *)Ram_MapGridCellBuffer)
/// A layer's visible area in cells: the screen's 15 by 10, and the row and
/// column partly scrolled in.
#define MAPGRID_VISIBLE_WIDTH 16
#define MAPGRID_VISIBLE_HEIGHT 12
/// The bytes between one layer's BG map and the next, from screen block 5.
#define MAPGRID_LAYER_SCREEN_BYTES 2048
/// A layer's BG map in cells, 16 by 16 (32 by 32 tiles), and the bytes of one
/// row of its tiles.
#define MAPGRID_SCREEN_CELLS 16
#define MAPGRID_SCREEN_ROW_BYTES 64
/// A cell's top tile row or left tile column in a layer's BG map, even,
/// wrapping at the map's 32; its row as tile entries from the map's start.
#define MAPGRID_SCREEN_TILE_MASK (BG_SCREEN_WIDTH - 2)
#define MAPGRID_SCREEN_ROW_MASK (MAPGRID_SCREEN_TILE_MASK * BG_SCREEN_WIDTH)
/// The cell tiles drawn down a column as the view scrolls sideways: the
/// screen's 10 rows of cells and the one partly scrolled in.
#define MAPGRID_COLUMN_CELLS 11
/// A map's cells as they are decoded: two planes of a byte a cell, the cells'
/// attribute bytes, then a plane of the numbers of their cell tiles, a halfword
/// a cell.
#define MAPGRID_BYTE_PLANES_BYTES (2 * MAPGRID_SIZE * MAPGRID_SIZE)
/// A map's palette: the first 14 of the 16 BG palettes, 224 colours.
#define MAP_PALETTE_BYTES (14 * PALETTE_COLORS * sizeof(u16))

/// Remaps byteCount bytes in place through gSpriteColorGreyTable.
void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount) {
  u8 *remapTable = (u8 *)gSpriteColorGreyTable;

  while (byteCount-- != 0) {
    *bytes = remapTable[*bytes];
    bytes++;
  }
}

/// Unpacks the staged cell tiles into gCellTiles.
void TileMap_DecodeStagedBuffer(s32 size) {
  u16 *destinationWords;
  u16 *sourceWords;
  s32 wordCount;
  s32 i;

  sourceWords = (u16 *)(Ram_MapGridCellBuffer + 2);
  destinationWords = (u16 *)gCellTiles;
  wordCount = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapGridCellBuffer)[1]) {
    case CELL_TILES_PACKING_RAW:
      for (i = 0; i < wordCount; i++)
        *destinationWords++ = *sourceWords++;
      break;
    case CELL_TILES_PACKING_SPLIT_DELTA: {
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
    case CELL_TILES_PACKING_DELTA: {
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

/// Unused: gives each map cell whose cell tiles are number 0xfff (a
/// placeholder) a number of its own, counting up from 0 in map order.
UNUSED void TileMap_NumberPlaceholderCellTiles(void) {
  u32 *cell = (u32 *)gMapGridCellBuffer;
  u32 remaining = MAPGRID_SIZE * MAPGRID_SIZE;
  u32 mask = MAPGRID_CELL_TILES_ID_MASK;
  s32 nextPlaceholder = -1;

  for (; remaining != 0; remaining--) {
    u32 value = *cell++;
    u32 cellTilesId = value & mask;

    if (cellTilesId == mask) {
      if (nextPlaceholder != cellTilesId)
        nextPlaceholder++;
      value = value + nextPlaceholder - cellTilesId;
      cell[-1] = value;
    }
  }
}

/// Joins a decoded map's planes in place into its cell words, with
/// Tile_ConvertMapGrid copied to RAM, which reads the byte planes from a copy.
void TileMap_ConvertBuffer(void) {
  u8 *savedCells;
  TilemapConvertFn convertGrid;

  savedCells = Heap_AllocEwram(MAPGRID_BYTE_PLANES_BYTES);
  Iwram_CopyWords(savedCells, Ram_MapGridCellBuffer, MAPGRID_BYTE_PLANES_BYTES);
  RamCode_Load(convertGrid, Tile_ConvertMapGrid, Tile_ConvertMapGridCodeSize);
  convertGrid((u8 *)gMapGridCellBuffer + MAPGRID_BYTE_PLANES_BYTES,
              Ram_MapGridCellBuffer, savedCells);
  Heap_Release(convertGrid);
  Heap_Release(savedCells);
}

/// Copies one tile column of a cell's tiles into a BG map: its top and
/// bottom tile entries, choosing the left or right half with tileHalf.
static __inline__ void MapGrid_CopyCellTilesHalfColumn(u32 *cell, u8 *screenMap,
                                                       u32 screenRowOffset,
                                                       u32 screenColumnOffset,
                                                       u32 tileHalf) {
  u16 *entry;
  u8 *screenTile;
  u32 index =
      ((*cell << MAPGRID_CELL_TILES_SHIFT) >> (MAPGRID_CELL_TILES_SHIFT - 2)) +
      tileHalf;

  entry = (u16 *)Ram_CellTiles + index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *entry;
  entry = (u16 *)(Ram_CellTiles + 4) + index;
  *(u16 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *entry;
}

/// Copies all four of a cell's tiles into a BG map: its top two tile entries
/// and its bottom two, a map row (64 bytes) below.
static __inline__ void MapGrid_CopyCellTilesToScreen(u32 *cell, u8 *screenMap,
                                                     u32 screenRowOffset,
                                                     u32 screenColumnOffset) {
  u32 *rows;
  u8 *screenTile;
  u32 index =
      (*cell << MAPGRID_CELL_TILES_SHIFT) >> (MAPGRID_CELL_TILES_SHIFT - 1);

  rows = (u32 *)Ram_CellTiles + index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *rows;
  rows = (u32 *)(Ram_CellTiles + 4) + index;
  *(u32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *rows;
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
  // Offsets from the header's start, of data packed for Decompress_TaggedLz
  // but for the treasure spots, which are read in place.
  s32 cellTiles; ///< the cell tiles, staged and unpacked into
                 ///< gCellTiles
  s32 shapes;    ///< the shapes, into Ram_MapGridShapes
  s32 cells;     ///< the cells, into Ram_MapGridCellBuffer
  s32 animation; ///< the tileset animation script, or 0 for none
  s32 blend;     ///< the blend script, or 0 for none
  s32 markers;   ///< the chests and Psynergy stones (Field_PlaceTreasureSpots)
};

/// One of a layout's resources, from its row of gMaps.
#define MAPGRID_RESOURCE(offset)                                               \
  Resource_GetTableEntry((offset) + (u32)RESOURCE_EMPTY_MAP)

/// Loads layout index from gMaps: its cell tiles, shapes, cells, tileset
/// animations, blend script and treasure spots; sets up the three background
/// layers' origins, parallax and wrap from the layout header; loads the palette
/// and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says they are loaded; and
/// starts the layer scrolling.
s32 MapGrid_LoadMap(s32 index) {
  struct MapResources *mapResources;
  struct MapGridScrollState *grid;
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
  grid = HeapSlot_AllocEwram(HEAP_SLOT_MAP_GRID,
                             sizeof(struct MapGridScrollState));
  Iwram_ClearWords(grid, sizeof(struct MapGridScrollState));
  mapHeader = MAPGRID_RESOURCE(mapResources->header);
  // The header's offsets count from the header itself.
  packed = (u8 *)mapHeader + mapHeader->cellTiles;
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
  grid->minX = toFIXED(mapHeader->cameraBounds[0] * 8);
  grid->minY = toFIXED(mapHeader->cameraBounds[1] * 8);
  grid->maxX = toFIXED(mapHeader->cameraBounds[2] * 8);
  grid->maxY = toFIXED(mapHeader->cameraBounds[3] * 8);
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
    FIXED offsetX, offsetY;

    layer->offsetX = offsetX = toFIXED(x * 8);
    layer->offsetY = offsetY = toFIXED(y * 8);
    // Parallax and drift are signed sixteenths.
    scaleX = layerSource->scrollX * (FIXED_ONE / 16);
    scaleY = layerSource->scrollY * (FIXED_ONE / 16);
    layer->scaleX = scaleX;
    layer->scaleY = scaleY;
    layer->speedX = layerSource->speedX * (toFIXED(1) / 16);
    layer->speedY = layerSource->speedY * (toFIXED(1) / 16);
    layer->maskX = layerSource->periodX;
    layer->maskY = layerSource->periodY;
    layer->phaseX = 0;
    layer->phaseY = 0;
    // The layer's window starts at the cell under its origin tile (a cell is
    // 2x2 tiles, 128 cells to a row).
    layer->cells = (struct MapGridCell *)Ram_MapGridCellBuffer +
                   (y / 2) * MAPGRID_SIZE + x / 2;
    layer->x = Iwram_MulFX(*viewX, scaleX) + offsetX;
    layer->y = Iwram_MulFX(*viewY, scaleY) + offsetY;
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
    // One 16 KB buffer takes each resource in turn: the palette, the three 16
    // KB tilesets, and last the tileset animations' tiles, which decode
    // straight to gMapGridAnimTiles.
    graphicsBuffer = Heap_Alloc(BG_CHAR_SIZE);
    if (graphicsBuffer != NULL) {
      // Colour 0 of BG palette 0 is the backdrop; it survives the new palette.
      backdrop = *(s16 *)PLTT;
      Decompress_TaggedLz(MAPGRID_RESOURCE(mapResources->palette),
                          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      // These four copies call the IWRAM routine at its fixed address, not
      // through Iwram_CopyWords.
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, MAP_PALETTE_BYTES);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources->tiles[0]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources->tiles[1]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources->tiles[2]),
                               graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, BG_CHAR_SIZE);
      Decompress_MoveToFrontLz(MAPGRID_RESOURCE(mapResources->animationTiles),
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
  Io_Write16(control, &REG_DISPCNT);
  Task_Add(MapGrid_UpdateLayerScroll, TASK_PASS_FRAME + 5);
  return 2;
}

/// Centres the camera on the point the map follows (origin: x, height, z), or
/// on (0, 0) when there is none, and scrolls the layers there.
void MapGrid_CenterOnFollowedPoint(void) {
  struct MapGridScrollState *grid = gMapGridState[0];
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

/// Draws the cell tiles of a row of 16 of a layer's cells (32 tiles) into its
/// BG map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles.
void MapGrid_RenderCellTilesRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
  u32 screenRowOffset = (tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
  u32 sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
  u32 screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
  u32 cellTilesIndex;

  // A cell's tiles are 2x2, so the BG map's tile row and column are kept
  // even and wrap at 32; the grid wraps at 128 cells.
  for (cellTilesIndex = 0; cellTilesIndex < MAPGRID_SCREEN_CELLS;
       cellTilesIndex++) {
    u32 *cell = MAPGRID_CELL_WORDS;

    cell += sourceRow + sourceColumn;
    MapGrid_CopyCellTilesToScreen(cell, screenMap, screenRowOffset,
                                  screenColumnOffset);
    sourceColumn = (sourceColumn + 1) & (MAPGRID_SIZE - 1);
    screenColumnOffset = (screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
  }
}

/// Draws the cell tiles of a column of 11 of a layer's cells, one tile wide,
/// into its BG map, starting at tile (tileX, tileY).
void MapGrid_RenderCellTilesColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
  u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
  u32 screenRowOffset = (tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
  u32 sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
  u32 screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
  u32 tileHalf = tileX & 1;
  u32 cellTilesIndex;

  // Both the cells and the BG map wrap.
  for (cellTilesIndex = 0; cellTilesIndex < MAPGRID_COLUMN_CELLS;
       cellTilesIndex++) {
    u32 *cell = MAPGRID_CELL_WORDS;

    cell += sourceRow + sourceColumn;
    MapGrid_CopyCellTilesHalfColumn(cell, screenMap, screenRowOffset,
                                    screenColumnOffset, tileHalf);

    sourceRow =
        (sourceRow + MAPGRID_SIZE) & ((MAPGRID_SIZE - 1) * MAPGRID_SIZE);
    screenRowOffset =
        (screenRowOffset + 2 * BG_SCREEN_WIDTH) & MAPGRID_SCREEN_ROW_MASK;
  }
}

/// Each frame on a layered map: centres the view on the point the map follows
/// (origin), clamped to the map's bounds and shaken by any decaying screen
/// shake; then scrolls each of the three layers by its parallax scale and
/// automatic scroll (wrapping at its period), draws the column or row
/// of cell tiles that scrolls into view, and sets its BG scroll.
void MapGrid_UpdateLayerScroll(void) {
  struct MapGridScrollState *grid;
  struct MapGridLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  grid = gMapGridState[0];
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
    baseX += Iwram_MulFX(shakeAmplitude, shakeRollA - shakeRollB);
    grid->shakeX = Iwram_MulFX(shakeAmplitude, grid->shakeDecay);
  }
  if (grid->shakeY != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    FIXED shakeAmplitude = grid->shakeY;
    baseY += Iwram_MulFX(shakeAmplitude, shakeRollA - shakeRollB);
    grid->shakeY = Iwram_MulFX(shakeAmplitude, grid->shakeDecay);
  }
  grid->view[0] = baseX;
  grid->view[1] = baseY;
  for (layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
    baseX = Iwram_MulFX(grid->view[0], layer->scaleX);
    baseY = Iwram_MulFX(grid->view[1], layer->scaleY);
    // A drifting layer wraps at its period: maskX and maskY are in 8-pixel
    // tiles, and the mask keeps the pixel and fraction bits below them.
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX = (baseX + layer->phaseX) &
              (layer->maskX * toFIXED(8) | (toFIXED(8) - 1));
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY = (baseY + layer->phaseY) &
              (layer->maskY * toFIXED(8) | (toFIXED(8) - 1));
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    // x and y are in 8-pixel tiles.
    x = baseX / toFIXED(8);
    y = baseY / toFIXED(8);
    if ((layer->x ^ baseX) & toFIXED(8)) {
      if (layer->x < baseX)
        MapGrid_RenderCellTilesColumn(layerIndex, x + DISPLAY_WIDTH / 8, y);
      else
        MapGrid_RenderCellTilesColumn(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & toFIXED(16)) {
      if (layer->y < baseY)
        MapGrid_RenderCellTilesRow(layerIndex, x, y + DISPLAY_HEIGHT / 8);
      else
        MapGrid_RenderCellTilesRow(layerIndex, x, y);
    }
    // MAPGRID_LAYER_GROUND scrolls BG3, MAPGRID_LAYER_OVERHEAD BG1.
    gBgScroll[3 - layerIndex].x = FIXEDtoINT(baseX);
    gBgScroll[3 - layerIndex].y = FIXEDtoINT(baseY);
    layer->x = baseX;
    layer->y = baseY;
  }
}

/// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
/// redraws every enabled layer's BG map around it.
void MapGrid_SetCameraCenter(FIXED x, FIXED y) {
  u32 layerIndex;
  u8 *screenMap;
  struct MapGridScrollState *grid = gMapGridState[0];
  struct MapGridLayerScroll *layer = grid->layers;

  x -= FIELD_VIEW_FOCUS_X;
  y -= FIELD_VIEW_FOCUS_Y;
  CLAMP(x, grid->minX, grid->maxX - FIELD_VIEW_WIDTH);
  CLAMP(y, grid->minY, grid->maxY - FIELD_VIEW_HEIGHT);
  grid->view[0] = x;
  grid->view[1] = y;

  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    if (grid->enabled[layerIndex]) {
      u32 rows = 2 * MAPGRID_COLUMN_CELLS;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          cellTilesRow, cellTilesColumn;

      x = Iwram_MulFX(grid->view[0], layer->scaleX);
      y = Iwram_MulFX(grid->view[1], layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x = (x + layer->phaseX) &
            (layer->maskX * toFIXED(8) | (toFIXED(8) - 1));
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y = (y + layer->phaseY) &
            (layer->maskY * toFIXED(8) | (toFIXED(8) - 1));
        // A layer drifting vertically gets its whole BG map, others the rows
        // a scrolling column draws.
        rows = 2 * MAPGRID_SCREEN_CELLS;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      layer++;
      // Divided by 8 pixels in 16.16, x and y become tiles.
      x /= toFIXED(8);
      y /= toFIXED(8);
      screenMap = BG_SCREEN_BLOCK(5) + layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
      sourceRow = ((y / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
      screenRowOffset = (y & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
      for (cellTilesRow = 0; cellTilesRow < rows / 2; cellTilesRow++) {
        sourceColumn = (x / 2) & (MAPGRID_SIZE - 1);
        screenColumnOffset = x & MAPGRID_SCREEN_TILE_MASK;
        for (cellTilesColumn = 0; cellTilesColumn < MAPGRID_SCREEN_CELLS;
             cellTilesColumn++) {
          u32 *cell = MAPGRID_CELL_WORDS;

          cell += sourceRow + sourceColumn;
          MapGrid_CopyCellTilesToScreen(cell, screenMap, screenRowOffset,
                                        screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & (MAPGRID_SIZE - 1);
          screenColumnOffset =
              (screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
        }
        sourceRow =
            (sourceRow + MAPGRID_SIZE) & ((MAPGRID_SIZE - 1) * MAPGRID_SIZE);
        screenRowOffset =
            (screenRowOffset + 2 * BG_SCREEN_WIDTH) & MAPGRID_SCREEN_ROW_MASK;
      }
    }
  }
}

/// The top left of a layer's visible area, in cells.
struct MapGridTilePosition {
  s32 x, y;
};

/// The redraw the three cell copies below share, for the cell at (cellX, cellY)
/// just
/// copied: the first layer whose visible area (16 by 12 cells from its scroll
/// position) holds it, and that one only, has the cell's top two tile
/// entries and its bottom two (a tile row, 64 bytes, further on) written to its
/// BG map. The layers' BG maps follow each other, 2,048 bytes apart, from
/// screen block 5, and in each a row of cells is 128 bytes and a cell 4. It
/// uses the copy's own locals: row and column (the cell's place in the BG map),
/// cellTilesId, visibleLayers and the scratch place, offset, bytes, i and
/// visibleLayer.
#define MAPGRID_REDRAW_COPIED_CELL(cellX, cellY)                               \
  visibleLayer = visibleLayers;                                                \
  place = row << 5;                                                            \
  place += column;                                                             \
  offset = place * 4;                                                          \
  do {                                                                         \
    if (visibleLayer->x <= (cellX) &&                                          \
        visibleLayer->x + MAPGRID_VISIBLE_WIDTH > (cellX) &&                   \
        visibleLayer->y <= (cellY) &&                                          \
        visibleLayer->y + MAPGRID_VISIBLE_HEIGHT > (cellY)) {                  \
      u8 *screen, *cellTiles;                                                  \
                                                                               \
      screen = BG_SCREEN_BLOCK(5);                                             \
      screen += offset;                                                        \
      bytes = cellTilesId * 8;                                                 \
      cellTiles = Ram_CellTiles;                                               \
      cellTiles += bytes;                                                      \
      *(u32 *)screen = *(u32 *)cellTiles;                                      \
      cellTiles = Ram_CellTiles + 4;                                           \
      cellTiles += bytes;                                                      \
      screen = BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;                  \
      screen += offset;                                                        \
      *(u32 *)screen = *(u32 *)cellTiles;                                      \
      break;                                                                   \
    }                                                                          \
    bytes = MAPGRID_LAYER_SCREEN_BYTES;                                        \
    offset += bytes;                                                           \
    visibleLayer++;                                                            \
    i++;                                                                       \
  } while (i < 3)

/// Copies the cell tiles numbers of a rectangle of cells from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination's
/// attributes, and redraws the copied cells that are on screen.
void MapGrid_CopyCellTilesIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
                                      u32 dstY, u32 width, u32 height) {
  u32 y;
  u32 *sourceCells = MAPGRID_CELL_WORDS + (sourceY * MAPGRID_SIZE + sourceX);
  u32 *destinationCells = MAPGRID_CELL_WORDS + (dstY * MAPGRID_SIZE + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollState *)gMapGridState[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  u32 x;

  for (i = 0; i < 3; i++) {
    visibleLayer->x = FIELD_CELL(layer->x);
    visibleLayer->y = FIELD_CELL(layer->y);
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellTilesId = cell & MAPGRID_CELL_TILES_ID_MASK;
      u32 column, row, place, offset, bytes;

      *destinationCells =
          (*destinationCells & MAPGRID_ATTRIBUTE_MASK) | cellTilesId;
      column = x & (MAPGRID_SCREEN_CELLS - 1);
      destinationCells++;
      i = 0;
      row = y & (MAPGRID_SCREEN_CELLS - 1);
      MAPGRID_REDRAW_COPIED_CELL((s32)x, (s32)y);
    }
    sourceCells += MAPGRID_SIZE - width;
    destinationCells += MAPGRID_SIZE - width;
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
/// tiles numbers to (destinationX, destinationY) and waits its delay in
/// frames.
void MapGrid_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
  struct MapGridCellAnimationStep *step =
      (struct MapGridCellAnimationStep *)steps;
  u32 source = step->source;
  /* FAKEMATCH(temporary): the end test reads a copy of source. */
  u32 test = source;

  /* FAKEMATCH(dead): updates of source that cancel out. */
  if (step) {
    source ^= test;
    source ^= test;
  }
  if (test != MAPGRID_ANIMATION_STEPS_END) {
    u32 end = MAPGRID_ANIMATION_STEPS_END;
    // Each step's rectangle and delay, read a step at a time beside it.
    /* FAKEMATCH(temporary): rectangle, a pointer to the step's y stepped a
     * step at a time beside step. */
    s16 *rectangle = (s16 *)(steps + 1);

    while (1) {
      s32 sourceY = rectangle[0];
      s32 width = rectangle[1];
      s32 height = rectangle[2];
      s32 delay = rectangle[3];

      rectangle += 5;
      MapGrid_CopyCellTilesIndicesRect(source, (u16)sourceY, destinationX,
                                       destinationY, (u16)width, (u16)height);
      step++;
      wait((u16)delay);
      source = step->source;
      /* FAKEMATCH(dead): updates of source that cancel out. */
      if (step) {
        source++;
        source--;
      }
      {
        /* FAKEMATCH(temporary): the loop's end test reads a copy of source. */
        u32 next = source;

        /* FAKEMATCH(dead): updates of source that cancel out. */
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
void MapGrid_CopyCellTilesCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                    s32 height, s32 dstX, s32 dstY) {
  s32 y;
  u32 *sourceCells = MAPGRID_CELL_WORDS + (sourceY * MAPGRID_SIZE + sourceX);
  u32 *destinationCells = MAPGRID_CELL_WORDS + (dstY * MAPGRID_SIZE + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollState *)gMapGridState[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  for (i = 0; i < 3; i++) {
    visibleLayer->x = FIELD_CELL(layer->x);
    visibleLayer->y = FIELD_CELL(layer->y);
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellTilesId;
      u32 column, row, place, offset, bytes;

      *destinationCells = cell;
      cellTilesId = cell & MAPGRID_CELL_TILES_ID_MASK;
      column = x & (MAPGRID_SCREEN_CELLS - 1);
      destinationCells++;
      i = 0;
      row = y & (MAPGRID_SCREEN_CELLS - 1);
      MAPGRID_REDRAW_COPIED_CELL(x, y);
    }
    sourceCells += MAPGRID_SIZE - width;
    destinationCells += MAPGRID_SIZE - width;
  }
}

/// Copies only the attributes of a rectangle of cells, from (sourceX, sourceY)
/// to (dstX, dstY), width by height cells, keeping each destination cell's cell
/// tiles number.
void MapGrid_CopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY) {
  u32 *destinationCells = MAPGRID_CELL_WORDS;
  u32 *sourceCells = destinationCells;
  u16 row;
  u16 column;

  destinationCells += dstY * MAPGRID_SIZE + dstX;
  sourceCells += sourceY * MAPGRID_SIZE + sourceX;

  for (row = 0; row < height; row++) {
    u32 *destinationCell = destinationCells + row * MAPGRID_SIZE;
    u32 *sourceCell = sourceCells + row * MAPGRID_SIZE;

    for (column = 0; column < width; column++) {
      *destinationCell = (*destinationCell & MAPGRID_CELL_TILES_ID_MASK) |
                         (*sourceCell++ & MAPGRID_ATTRIBUTE_MASK);
      destinationCell++;
    }
  }
}

/// Copies only the cell tiles numbers of a rectangle of cells, from (sourceX,
/// sourceY) to (dstX, dstY), width by height cells, keeping each destination
/// cell's attributes; cells in a visible area update the BG maps too.
///
/// The same copy as MapGrid_CopyCellTilesIndicesRect with the size before the
/// destination and signed cells: the game has both, and events call each.
void MapGrid_CopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  s32 y;
  u32 *sourceCells = MAPGRID_CELL_WORDS + (sourceY * MAPGRID_SIZE + sourceX);
  u32 *destinationCells = MAPGRID_CELL_WORDS + (dstY * MAPGRID_SIZE + dstX);
  struct MapGridLayerScroll *layer =
      ((struct MapGridScrollState *)gMapGridState[0])->layers;
  struct MapGridTilePosition visibleLayers[3];
  s32 i;
  struct MapGridTilePosition *visibleLayer = visibleLayers;
  s32 x;

  for (i = 0; i < 3; i++) {
    visibleLayer->x = FIELD_CELL(layer->x);
    visibleLayer->y = FIELD_CELL(layer->y);
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *sourceCells++;
      u32 cellTilesId = cell & MAPGRID_CELL_TILES_ID_MASK;
      u32 column, row, place, offset, bytes;

      *destinationCells =
          (*destinationCells & MAPGRID_ATTRIBUTE_MASK) | cellTilesId;
      column = x & (MAPGRID_SCREEN_CELLS - 1);
      destinationCells++;
      i = 0;
      row = y & (MAPGRID_SCREEN_CELLS - 1);
      MAPGRID_REDRAW_COPIED_CELL(x, y);
    }
    sourceCells += MAPGRID_SIZE - width;
    destinationCells += MAPGRID_SIZE - width;
  }
}

/// Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
/// REG_DISPCNT) from layerFlags, keeping its other flags.
void MapGrid_SetShownLayers(u32 layerFlags) {
  struct MapGridScrollState *grid = gMapGridState[0];
  u32 shown = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
  u32 flags = grid->flags;

  flags = (flags & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
          shown;
  grid->flags = flags;
}

#undef MAPGRID_CELL_WORDS
#undef MAPGRID_ATTRIBUTE_MASK
#undef MAPGRID_CELL_TILES_SHIFT
#undef MAPGRID_VISIBLE_WIDTH
#undef MAPGRID_VISIBLE_HEIGHT
#undef MAPGRID_LAYER_SCREEN_BYTES
#undef MAPGRID_SCREEN_CELLS
#undef MAPGRID_SCREEN_ROW_BYTES
#undef MAPGRID_RESOURCE
#undef MAPGRID_REDRAW_COPIED_CELL
