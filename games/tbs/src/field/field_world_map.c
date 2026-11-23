/// @file
/// The world map: its perspective view, its tiles, and the still image of
/// the view BG1 shows while a menu or event runs.
// Part of field.c.

#include "field_internal.h"

/// Io_Write16 as an inline function.
static __inline__ void Io_Write16Inline(s32 value, volatile u16 *reg) {
  *reg = value;
}

/// Io_Write16Inline with the address first.
static __inline__ void Io_Write16Swapped(volatile u16 *reg, s32 value) {
  *reg = value;
}

/// Matrix_UpdateVertices, run from RAM: works out each screen line's BG2 and
/// BG3 affine parameters for the camera into one of the two affine pages.
typedef s32 (*WorldMapPlaneFn)(void *camera, s32 *position, void *lines,
                               void *out);

/// One 32-pixel cell of the world map: its tile, its kind
/// (WorldMapEncounterRegion) and a flag.
struct WorldMapCell {
  u32 tile
      : 24;     ///< low halfword: the cell picture WorldMap_CopyCellTiles draws
  u32 kind : 7; ///< the encounter region; encounters read it too
  /// Selects the second row of gWorldMap_BattleEnvironments
  /// (WorldMap_GetBattleEnvironment); a WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
  /// cell takes the third row whatever this bit says.
  u32 flag : 1;
};

#define WORLD_MAP_BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_MAP_CELLS ((struct WorldMapCell *)Ram_CellPictures)

/// A world map tile is 16 by 16 cells.
#define WORLD_MAP_TILE_CELLS 16
/// The world map is 16 by 16 tiles, 512 pixels each, wrapping; each layer's
/// tile numbers follow the 320 before it.
#define WORLD_MAP_TILES_ACROSS 16
#define WORLD_MAP_LAYER_TILES 320
/// The tiles after the world's 256 in a layer: other versions of a few grid
/// cells.
#define WORLD_MAP_ALTERNATE_TILES                                              \
  (WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS)
/// 16.16 pixels to whole 512-pixel tiles.
#define WORLD_MAP_FIXED_TO_TILE(v) ((v) >> 25)
/// Each layer's cell buffer holds the 2 by 2 tiles around the view: 32 cells
/// a side, 4 bytes a cell.
#define WORLD_MAP_BUFFER_CELLS 32
#define WORLD_MAP_CELL_ROW_BYTES (WORLD_MAP_BUFFER_CELLS * 4)
#define WORLD_MAP_LAYER_CELL_BYTES                                             \
  (WORLD_MAP_BUFFER_CELLS * WORLD_MAP_CELL_ROW_BYTES)
/// An affine layer's BG map: 64 tiles a row, one byte each, two screen blocks
/// in all.
#define WORLD_MAP_SCREEN_WIDTH 64
/// A map row as the halfwords that write it two tiles at a time.
#define WORLD_MAP_SCREEN_ROW_HALFWORDS (WORLD_MAP_SCREEN_WIDTH / 2)
#define WORLD_MAP_LAYER_SCREEN_BYTES                                           \
  (WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_WIDTH)
/// MapGridScrollWork's wideTiles, which the world map shares: any nonzero
/// value has the tileset animations copy 64-byte tiles.
#define WORLD_MAP_WIDE_TILES 128

/// The window rows WorldMap_StartStillImage sets, below the screen: the
/// frame update then picks mode 0 and BG1 shows the still image.
#define WORLD_MAP_STILL_WINDOW_TOP 200
#define WORLD_MAP_STILL_WINDOW_BOTTOM 255
#define WORLD_MAP_STILL_TILES (DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT)

/// Grid cell (column, row) shows its alternate tile.
#define WORLD_MAP_USE_ALTERNATE_TILE(perspective, column, row)                 \
  ((perspective)->tileIds[(row) * WORLD_MAP_TILES_ACROSS + (column)] =         \
       WORLD_MAP_ALTERNATE_TILES + (row) * WORLD_MAP_TILES_ACROSS + (column))

/// Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's cell
/// picture buffer, decompressing it from the area's graphics, and with update
/// set also draws it into the layer's BG map.
s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct WorldMapView *perspective = gMapGridWork[0];
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
  decodedTile = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH,
                               WORLD_MAP_TILE_CELLS * WORLD_MAP_TILE_CELLS * 4);
  Decompress_ByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);

  // A tile is 16 by 16 cells of 4 bytes.
  sourceTiles = decodedTile;
  quarterCells = Ram_CellPictures + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, DMA_COPY_WORDS(WORLD_MAP_TILE_CELLS),
            REG_DMA3);
    sourceTiles += WORLD_MAP_TILE_CELLS * 2;
    quarterCells += WORLD_MAP_CELL_ROW_BYTES;
  }

  // The affine BG map has one byte a tile, 64 to a row: each cell's first
  // halfword picks a cell picture from Ram_MapGridCellBuffer, whose two
  // halfwords are its top two tiles and its bottom two, a row (64 bytes) below.
  if (update != 0) {
    screenTiles =
        (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
      for (columnIndex = 0; columnIndex < WORLD_MAP_TILE_CELLS; columnIndex++) {
        u16 *bottomTiles;

        cellPictureId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapGridCellBuffer + cellPictureId * 4);
        bottomTiles = (u16 *)(Ram_MapGridCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + cellPictureId * 4);
        screenTiles[WORLD_MAP_SCREEN_ROW_HALFWORDS] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      // A row of cells is two map rows.
      screenTiles += 2 * WORLD_MAP_SCREEN_ROW_HALFWORDS - WORLD_MAP_TILE_CELLS;
    }
  }
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  return 1;
}

/// Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
/// camera work, the projection, the per-scanline ground plane routine in RAM
/// (Matrix_UpdateVertices) and its first frame, and the per-frame view and
/// HBlank updates.
UNREAD_RESULT WorldMap_InitializePerspective(void) {
  struct WorldMapView *perspective;
  struct BattleCamera *camera;
  struct WorldMapAffineWork *affineWork;
  struct WorldMapAffinePair(*affinePages)[DISPLAY_HEIGHT];
  s32 *cameraPosition;
  s32 *distance;
  s16 *zoomTransitionFrames;
  FIXED farPlane;
  s32 tileIndex;

  // Hide the backgrounds and sprites while their memory is replaced.
  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  ScreenFade_FromBlack(0);
  perspective = HeapSlot_Alloc(HEAP_SLOT_MAP_GRID, sizeof(struct WorldMapView));
  Dma_Fill32(0, perspective, sizeof(struct WorldMapView));
  perspective->view[0] = 0;
  perspective->view[1] = 0;
  perspective->scaleX = FIXED_ONE * 32;
  perspective->scaleY = FIXED_ONE * 64;
  perspective->limitX = toFIXED(8160);
  perspective->limitY = toFIXED(8160);
  perspective->script = NULL;
  perspective->tiles = Resource_GetTableEntry(RESOURCE_PERSPECTIVE_DATA_A);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DEFAULT_TILE_ANIMATION),
                      MAPGRID_ANIMATION_SCRIPT);
  TilesetAnim_StartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
  Io_Write16Inline(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                       BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN |
                       BLDCNT_TGT2_ALL,
                   &REG_BLDCNT);
  Io_Write16Inline(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  REG_BLDY = 0;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DEFAULT_GRID_CELLS),
                      Ram_MapGridCellBuffer);
  Decompress_TaggedLz(
      Resource_GetTableEntry(RESOURCE_DEFAULT_CELL_PICTURE_ATTRIBUTES),
      Ram_MapGridShapes);
  // The ground is BG3 and BG2, two 512-pixel affine layers that wrap (char
  // blocks 2 and 3, screen blocks 8 and 10); BG1 is a plain layer on screen
  // block 5.
  perspective->fade = DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON |
                      DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
  // fadeStep is where a map keeps wideTiles, read by the tileset animations.
  perspective->fadeStep = WORLD_MAP_WIDE_TILES;
  Io_Write16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
                       BGCNT_SCREEN_BASE(8) | BGCNT_WRAP | BGCNT_SIZE(2),
                   &REG_BG3CNT);
  Io_Write16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) |
                       BGCNT_SCREEN_BASE(10) | BGCNT_WRAP | BGCNT_SIZE(2),
                   &REG_BG2CNT);
  Io_Write16Inline(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Io_Write16(Q_8_8(1), &REG_BG2PA);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  Io_Write16(Q_8_8(1), &REG_BG2PD);
  REG_BG2X = 0;
  REG_BG2Y = 0;
  Io_Write16(Q_8_8(1), &REG_BG3PA);
  REG_BG3PB = 0;
  REG_BG3PC = 0;
  Io_Write16(Q_8_8(1), &REG_BG3PD);
  REG_BG3X = 0;
  Io_Write16(0, &REG_BG3Y);

  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  affineWork = HeapSlot_Alloc(HEAP_SLOT_WORLD_MAP_AFFINE,
                              sizeof(struct WorldMapAffineWork));
  cameraPosition = camera->focus;
  affinePages = affineWork->pages;
  farPlane = toFIXED(510);
  perspective->farPlane = farPlane;
  distance = &perspective->distance;
  *distance = farPlane;
  perspective->zoom = FIXED_ONE;
  zoomTransitionFrames = &perspective->zoomTransitionFrames;
  camera->followEye = 0;
  camera->followPosition = 0;
  *zoomTransitionFrames = 0;
  // A first view from the origin at pitch and yaw 0, so the scanline
  // table and the first affine page exist before the first frame; the
  // projection's centre is 120 by 96, as on the layered maps.
  gProjection.centerX = DISPLAY_WIDTH / 2;
  gProjection.centerY = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  SET_VECTOR(cameraPosition, 0, 0, 0);
  Matrix_Initialize();
  Matrix_TranslateVector(cameraPosition);
  Matrix_RotateY(perspective->yaw);
  Matrix_RotateX(perspective->pitch);
  {
    struct Point eyeOffset;

    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = farPlane;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, cameraPosition);
    RamCode_LoadSlot(HEAP_SLOT_RAM_CODE, Matrix_UpdateVertices,
                     &Matrix_UpdateVerticesSize);
    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(Trig_Cos(perspective->pitch),
                                                  Trig_Sin(perspective->pitch)),
                                cameraPosition, affineWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
    ((WorldMapPlaneFn)gHeapSlots[HEAP_SLOT_RAM_CODE])(
        camera, cameraPosition, affineWork->scanlines,
        affinePages[gFrameCount & 1]);
    // Then the world map's own view: pitch TRIG_TURN * 7 / 8 (minus an eighth
    // of a turn) and yaw 0, with the eye one pixel beyond the distance.
    SET_VECTOR(cameraPosition, 0, 0, 0);
    Matrix_Initialize();
    Io_Write16Swapped(&perspective->pitch, TRIG_TURN * 7 / 8);
    perspective->yaw = 0;
    Matrix_Initialize();
    Matrix_TranslateVector(cameraPosition);
    Matrix_RotateY(perspective->yaw);
    Matrix_RotateX(perspective->pitch);
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = *distance + FIXED_ONE;
    Iwram_TransformVector(&eyeOffset, camera->eye);
  }
  REG_MOSAIC = 0;
  Io_Write16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  // Rows 0 to 159: the whole screen shows the perspective view.
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  Task_Add(WorldMap_UpdateView, TASK_PASS_FRAME + 5);
  Task_Add(WorldMap_ApplyAffinePage, TASK_PASS_VBLANK);
  // Each of the 16 by 16 grid cells starts on the tile of its own number.
  for (tileIndex = 0;
       tileIndex < WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS; tileIndex++)
    perspective->tileIds[tileIndex] = tileIndex;
}

/// Puts world map tile (x / 16, y / 16) at cell (cellX / 16, cellY / 16) of the
/// tile id grid, and draws it at once when it is next to the view. cellX and
/// cellY count 32-pixel world map cells, so a grid cell is a 512-pixel tile; it
/// is next to the view when cellX / 8 and cellY / 8, in 256-pixel steps, are
/// within one step of the followed point's.
void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 cellX, s32 cellY) {
  struct WorldMapView *perspective;
  s32 *followedPosition;
  s32 originX;
  s32 originY;
  s32 tileId;

  perspective = gMapGridWork[0];
  originX = 0;
  originY = 0;
  followedPosition = perspective->origin;
  if (followedPosition != 0) {
    originX = *followedPosition++;
    originY = followedPosition[1];
  }

  // The followed point and cellX, cellY in 256-pixel steps.
  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  cellX >>= 3;
  cellY >>= 3;
  tileId = (y << 4) + x;
  perspective->tileIds[(((cellY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 4) +
                       ((cellX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tileId;

  if (ABS(originX - cellX) <= 1 && ABS(originY - cellY) <= 1) {
    WorldMap_WriteLayerCellTile(0, cellX / 2, cellY / 2, tileId, 1);
    WorldMap_WriteLayerCellTile(1, cellX / 2, cellY / 2,
                                tileId + WORLD_MAP_LAYER_TILES, 1);
  }
}

/// Loads the world map's palette and tiles for the area at (x, z): the second
/// set where the ground there is WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.
void WorldMap_LoadGraphics(s32 x, s32 z) {
  struct WorldMapView *perspective;
  u32 *resources;
  u8 *buffer;
  u8 *tiles;
  u32 *screenTiles;
  u32 tilePair;
  s32 graphicsSet;
  s32 screenRow;
  s32 screenColumn;
  s16 backdropColor;

  graphicsSet = 0;
  buffer = Heap_Alloc(512);
  perspective = gMapGridWork[0];
  // The 32-pixel cell under (x, z), 32 cells to a row.
  if (WORLD_MAP_CELLS[((x / toFIXED(32)) & 31) +
                      (((z / toFIXED(32)) & 31) << 5)]
          .kind == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
    graphicsSet = 1;
  resources = gWorldMap_GraphicsResources[graphicsSet];
  perspective->resources = resources;
  backdropColor = WORLD_MAP_BG_PALETTE[0];
  // Palettes 0 to 13, keeping the backdrop colour.
  Decompress_ByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, WORLD_MAP_BG_PALETTE, DMA_COPY_WORDS(14 * PALETTE_COLORS / 2),
          REG_DMA3);
  tiles = gBgTileBuffer;
  Decompress_TaggedLz(Resource_GetTableEntry(resources[1]), tiles);
  Dma_Set(tiles, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[2]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
  Dma_Set(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES,
          BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
  Dma_Set(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2, BG_CHAR_BLOCK(3),
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
  Dma_Set(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3,
          BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]), gMapGridAnimTiles);
  // BG1's map (screen block 5) is filled with tile 127 of WINDOW_PALETTE, and
  // screen block 6 numbers a 30 by 20 tile image from
  // WORLD_MAP_STILL_FIRST_TILE, two tile numbers a word, for the still
  // image WorldMap_StartStillImage shows.
  Dma_Fill32(WINDOW_TILE_PAIR(WINDOW_TILE(127)), BG_SCREEN_BLOCK(5), 1536);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = MAP_ENTRY_PAIR(WORLD_MAP_STILL_FIRST_TILE,
                            WORLD_MAP_STILL_FIRST_TILE + 1);
  for (screenRow = 0; screenRow < DISPLAY_TILE_HEIGHT; screenRow++) {
    for (screenColumn = 0; screenColumn < DISPLAY_TILE_WIDTH / 2;
         screenColumn++) {
      *screenTiles++ = tilePair;
      tilePair += MAP_ENTRY_PAIR(2, 2);
    }
    screenTiles++;
  }
  // On the snowfield set, six grid cells of the snowy north show their own
  // tiles.
  if (graphicsSet == 1) {
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 0);
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 0);
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 0);
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 1);
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 1);
    WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 1);
    WorldMap_UpdateCurrentTileBlock();
  }
  Heap_Release(buffer);
}

/// HBlank setup each VBlank on the world map: copies the first scanline's BG2
/// and BG3 affine parameters, starts the HBlank DMA for the rest, and picks the
/// BG mode from the window's top and bottom.
void WorldMap_ApplyAffinePage(void) {
  struct WorldMapAffinePair(*affinePages)[DISPLAY_HEIGHT] =
      ((struct WorldMapAffineWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE))
          ->pages;
  s32 displayControl = (s16)(REG_DISPCNT & ~DISPCNT_MODE_MASK);
  u32 *affineRegisters = (u32 *)&REG_BG2PA;
  struct WorldMapView *perspective = gMapGridWork[0];
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  // Stop the last VBlank's HBlank DMA, then load line 0's eight words (BG2PA to
  // BG3Y) by hand; the DMA repeats eight words into the same registers at
  // each HBlank for the lines after it.
  dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma0[5] &= ~DMA_ENABLE;
  (void)dma0[5];
  if (affinePages != NULL) {
    affineSource = (u32 *)affinePages[gFrameCount & 1];
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters = *affineSource++;
    Dma_Set(affineSource, (void *)&REG_BG2PA,
            (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT |
             DMA_DEST_RELOAD)
                    << 16 |
                8,
            (volatile u32 *)dma0);
  }
  perspective->shownTop = perspective->windowTop;
  perspective->shownBottom = perspective->windowBottom;
  // DISPCNT_MODE_2 (the affine ground) for the load's rows 0 to 159;
  // DISPCNT_MODE_0 for WorldMap_StartStillImage's 200 and 255, which show the
  // still image on BG1.
  windowTop = perspective->shownTop;
  mode = DISPCNT_MODE_0;
  if (windowTop < WORLD_MAP_STILL_WINDOW_TOP) {
    u16 bottom = perspective->shownBottom;

    mode = (bottom != 0) << 1;
    if (windowTop <= bottom) {
      mode = DISPCNT_MODE_0;
      if (windowTop == 0)
        mode = DISPCNT_MODE_2;
    }
  }
  displayControl |= mode;
  nextDisplayControl = (u16)displayControl;
  REG_DISPCNT = nextDisplayControl;
  perspective->windowPending = 0;
}

/// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void WorldMap_CopyCellTiles(u8 *cellBytes, u8 *screenTile) {
  u32 cellPictureIndex = *(u16 *)cellBytes;
  u16 *cellPictureTiles = (u16 *)Ram_MapGridCellBuffer;

  cellPictureTiles += cellPictureIndex * 2;
  *(u16 *)screenTile = *cellPictureTiles++;
  *(u16 *)(screenTile + WORLD_MAP_SCREEN_WIDTH) = *cellPictureTiles;
}

/// Draws row tileRow of the cell buffer into both layers' BG maps, 32 cells in
/// each: the row of the ground that scrolls into view.
void WorldMap_RenderPaletteMappedRow(u32 tileRow) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_CellPictures + ((((s32)tileRow / 2) & 31) << 7);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + ((tileRow & 62) << 6));
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied < WORLD_MAP_BUFFER_CELLS);

  // The same row of the second layer.
  screenTile += WORLD_MAP_LAYER_SCREEN_BYTES - WORLD_MAP_BUFFER_CELLS * 2;
  sourceCell += WORLD_MAP_LAYER_CELL_BYTES - WORLD_MAP_CELL_ROW_BYTES;
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied < WORLD_MAP_BUFFER_CELLS);
}

/// Draws column tileColumn of the cell buffer into both layers' BG maps, 32
/// cells down in each (the second layer's map and cells follow the first's):
/// the column of the ground that scrolls into view.
void WorldMap_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_CellPictures + ((((s32)tileColumn / 2) & 31) << 2);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + (tileColumn & 62));
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += WORLD_MAP_SCREEN_WIDTH * 2;
    sourceCell += WORLD_MAP_CELL_ROW_BYTES;
  } while (cellsCopied < 2 * WORLD_MAP_BUFFER_CELLS);
}

/// Each frame on the world map: loads tiles as the view moves, shakes and
/// follows the player, draws the BG row or column that scrolls into view, moves
/// the camera, and builds the ground plane's scanline table.
void WorldMap_UpdateView(void) {
  struct BattleCamera *camera = gBattleCamera;
  struct WorldMapAffineWork *affineWork =
      HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
  struct WorldMapView *perspective = gMapGridWork[0];
  s32 *cameraPosition = camera->focus;
  s32 *followedPosition = perspective->origin;
  struct WorldMapAffinePair(*affinePages)[DISPLAY_HEIGHT] = affineWork->pages;
  s32 distance = perspective->farPlane;
  s32 height = perspective->distance;
  struct Point eyeOffset;

  WorldMap_PreloadCurrentTileBlock();
  if (followedPosition != NULL) {
    FIXED x;
    FIXED y;
    s32 tileColumn;
    s32 tileRow;

    y = followedPosition[2];
    x = followedPosition[0];
    if (perspective->shakeX != 0) {
      s32 shakeDifference = Random16();
      s32 shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeX;
      x += Iwram_MulFX(shakeAmplitude, shakeDifference);
      perspective->shakeX =
          Iwram_MulFX(shakeAmplitude, perspective->shakeDecay);
    }
    if (perspective->shakeY != 0) {
      s32 shakeDifference = Random16();
      FIXED shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeY;
      y += Iwram_MulFX(shakeAmplitude, shakeDifference);
      perspective->shakeY =
          Iwram_MulFX(shakeAmplitude, perspective->shakeDecay);
    }
    // On crossing a 16-pixel line, draw the column 16 on that side, or the
    // row 12 on when y grows and 18 back when it falls.
    tileColumn = x / toFIXED(16);
    tileRow = y / toFIXED(16);
    if ((perspective->view[0] ^ x) & toFIXED(16)) {
      if (perspective->view[0] < x)
        WorldMap_RenderPaletteMappedColumn(tileColumn + 16);
      else
        WorldMap_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((perspective->view[1] ^ y) & toFIXED(16)) {
      if (perspective->view[1] < y)
        WorldMap_RenderPaletteMappedRow(tileRow + 12);
      else
        WorldMap_RenderPaletteMappedRow(tileRow - 18);
    }
    perspective->view[0] = x;
    perspective->view[1] = y;
  }
  // Rebuild the projection and look at the followed position from the
  // current pitch and yaw, the eye one pixel beyond the zoomed distance.
  // followedPosition is read here without the NULL test above, which never
  // fails on the world map: FieldSequence_Run's Actor_ResetForLeader
  // points it at the camera actor before any frame passes, and nothing clears
  // it.
  gProjection.centerX = DISPLAY_WIDTH / 2;
  gProjection.centerY = 96;
  Projection_Set(distance, height / 2, height * 2);
  SET_VECTOR(cameraPosition, *followedPosition++, 0, followedPosition[1]);
  Matrix_Initialize();
  Matrix_TranslateVector(cameraPosition);
  Matrix_RotateY(perspective->yaw);
  Matrix_RotateX(perspective->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = height + FIXED_ONE;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Matrix_Initialize();
  Matrix_SetLookAt(camera->eye, cameraPosition);
  // The scanline table depends only on the pitch, so it is rebuilt only when
  // the pitch has changed.
  if (gScanlinePitch != perspective->pitch) {
    FIXED cosine = Trig_Cos(perspective->pitch);
    FIXED sine = Trig_Sin(perspective->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                affineWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
  }
  ((WorldMapPlaneFn)gHeapSlots[HEAP_SLOT_RAM_CODE])(
      camera, cameraPosition, affineWork->scanlines,
      affinePages[gFrameCount & 1]);
}

/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlock(void) {
  struct WorldMapView *perspective = gMapGridWork[0];
  s32 firstQuarterX = 0;
  s32 firstQuarterY = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 layerTileBase;
  s32 tileId;

  if (perspective->origin != NULL) {
    s32 *followedPosition = perspective->origin;

    firstQuarterX = *followedPosition++;
    firstQuarterY = followedPosition[1];
  }
  // The top left of the 2x2 tiles around the point, 256 pixels left of it
  // and 320 above.
  firstQuarterX = WORLD_MAP_FIXED_TO_TILE(firstQuarterX - toFIXED(256));
  firstQuarterY = WORLD_MAP_FIXED_TO_TILE(firstQuarterY - toFIXED(320));
  layer = 0;
  layerTileBase = 0;
  for (; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1)) << 4) +
                 ((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
        tileId = perspective->tileIds[tileId];
        tileId += layerTileBase;
        WorldMap_WriteLayerCellTile(layer, firstQuarterX + col,
                                    firstQuarterY + row, tileId, 1);
      }
    }
    layerTileBase += WORLD_MAP_LAYER_TILES;
  }
}

/// Loads the first of the 2x2 world map tiles around the player that is not
/// shown yet (at most one a frame), without drawing it.
void WorldMap_PreloadCurrentTileBlock(void) {
  struct WorldMapView *perspective = gMapGridWork[0];
  s32 firstQuarterX = 0;
  s32 firstQuarterY = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 tileId;

  if (perspective->origin != NULL) {
    s32 *followedPosition = perspective->origin;

    firstQuarterX = *followedPosition++;
    firstQuarterY = followedPosition[1];
  }
  firstQuarterX = WORLD_MAP_FIXED_TO_TILE(firstQuarterX - toFIXED(256));
  firstQuarterY = WORLD_MAP_FIXED_TO_TILE(firstQuarterY - toFIXED(320));
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1)) << 4) +
                 ((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
        tileId = perspective->tileIds[tileId];
        tileId += layer * WORLD_MAP_LAYER_TILES;
        if (WorldMap_WriteLayerCellTile(layer, firstQuarterX + col,
                                        firstQuarterY + row, tileId, 0) != 0)
          return;
      }
    }
  }
}

/// VBlank: shows BG1 and copies the decoded picture's tiles into VRAM.
void WorldMap_PresentStillImage(void) {
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
             &REG_BG1CNT);
  Dma_Set(gMapGridCellBuffer,
          BG_CHAR_BLOCK(0) + WORLD_MAP_STILL_FIRST_TILE * TILE_SIZE_8BPP,
          DMA_COPY_WORDS(WORLD_MAP_STILL_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
}

/// Freezes the world map's perspective view as a still image on BG1: saves
/// BG1's tiles, draws the current affine frame into pixels and hands the window
/// and VBlank to it.
void WorldMap_StartStillImage(void) {
  struct WorldMapAffineWork *affineWork =
      HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
  struct WorldMapView *perspective = gMapGridWork[0];

  perspective->animationActive = 1;
  Task_SuspendAll(TilesetAnim_Update);
  Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  wait(1);
  TileMap_DrawRowsFromRam(affineWork->pages[gFrameCount & 1],
                          (u8 *)gMapGridCellBuffer);
  perspective->windowTop = WORLD_MAP_STILL_WINDOW_TOP;
  perspective->windowBottom = WORLD_MAP_STILL_WINDOW_BOTTOM;
  gVBlankCallback = WorldMap_PresentStillImage;
}

/// VBlank at the end of the still image: puts BG1 back as the plain layer
/// and copies the area's four tile sets back into char blocks 2 and 3.
void WorldMap_ShowBg1FromBuffer(void) {
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2),
          DMA_COPY_WORDS(4 * WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
}

/// Reloads the world map area's palette and tiles after the still image, puts
/// the default cells back and returns to the perspective view.
void WorldMap_LoadAreaGraphics(void) {
  struct WorldMapView *perspective;
  u32 *graphicsResources;
  u8 *decodeBuffer;
  s16 backdropColor;

  perspective = gMapGridWork[0];
  decodeBuffer = (u8 *)gMapGridCellBuffer;
  graphicsResources = perspective->resources;
  backdropColor = *(s16 *)BG_PLTT;
  Decompress_ByteLz(Resource_GetTableEntry(graphicsResources[0]), decodeBuffer);
  *(s16 *)decodeBuffer = backdropColor;
  Dma_Set(decodeBuffer, (void *)BG_PLTT,
          DMA_COPY_WORDS(14 * PALETTE_COLORS / 2), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(graphicsResources[1]),
                      gBgTileBuffer);
  Decompress_TaggedLz(Resource_GetTableEntry(graphicsResources[2]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
  Decompress_TaggedLz(Resource_GetTableEntry(graphicsResources[3]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
  Decompress_TaggedLz(Resource_GetTableEntry(graphicsResources[4]),
                      Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
  // The VBlank copies the tiles back and the next frame shows the ground;
  // only then is the still image's buffer refilled with the cells.
  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((u32)RESOURCE_DEFAULT_GRID_CELLS),
                      decodeBuffer);
  WorldMap_UpdateCurrentTileBlock();
  perspective->animationActive = 0;
  Task_ResumeAll(TilesetAnim_Update);
  wait(1);
}

/// Puts the default world map cells back after the still image and returns to
/// the perspective view, without reloading the area's palette or tiles.
void WorldMap_LoadDefaultCellsAndUpdateBlock(void) {
  struct WorldMapView *perspective = gMapGridWork[0];
  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DEFAULT_GRID_CELLS),
                      gMapGridCellBuffer);
  WorldMap_UpdateCurrentTileBlock();
  Task_ResumeAll(TilesetAnim_Update);
  wait(1);
}

#undef WORLD_MAP_BG_PALETTE
#undef WORLD_MAP_CELLS
#undef WORLD_MAP_TILE_CELLS
#undef WORLD_MAP_TILES_ACROSS
#undef WORLD_MAP_LAYER_TILES
#undef WORLD_MAP_ALTERNATE_TILES
#undef WORLD_MAP_FIXED_TO_TILE
#undef WORLD_MAP_BUFFER_CELLS
#undef WORLD_MAP_CELL_ROW_BYTES
#undef WORLD_MAP_LAYER_CELL_BYTES
#undef WORLD_MAP_SCREEN_WIDTH
#undef WORLD_MAP_SCREEN_ROW_HALFWORDS
#undef WORLD_MAP_LAYER_SCREEN_BYTES
#undef WORLD_MAP_WIDE_TILES
#undef WORLD_MAP_STILL_WINDOW_TOP
#undef WORLD_MAP_STILL_WINDOW_BOTTOM
#undef WORLD_MAP_STILL_TILES
#undef WORLD_MAP_USE_ALTERNATE_TILE
