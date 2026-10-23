/// @file
/// The world map: its perspective view, its tiles, and the still picture of
/// the view BG1 shows while a menu or event runs.
// Part of field.c.

#include "../field_internal.h"

/// Writes a halfword register (value first).
static __inline__ void Io_Set16(s32 value, volatile u16 *reg) { *reg = value; }

/// Writes a halfword register (register first).
static __inline__ void Io_Put16(volatile u16 *reg, s32 value) { *reg = value; }

typedef s32 (*WorldMapPlaneFn)(void *camera, s32 *position, void *lines,
                               void *out);

/// One 32-pixel cell of the world map: its tile, its kind (WorldMapRegion) and
/// a flag.
struct WorldMapCell {
  u32 tile : 24; ///< its low halfword is the block WorldMap_CopyCellTiles draws
  u32 kind : 7;  ///< the terrain kind; encounters read it too
  /// Selects the second row of gWorldMap_TerrainBehaviorTable
  /// (GetWorldMapTerrainBehavior); a WORLD_MAP_REGION_SNOWFIELD cell takes the
  /// third row whatever this bit says.
  u32 flag : 1;
};

#define WORLD_MAP_BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_MAP_CELLS ((struct WorldMapCell *)Ram_MapBlocks)

/// Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's block
/// buffer, decompressing it from the area's graphics, and with update set
/// also draws it into the layer's BG map. Returns 0 when that quarter already
/// shows the tile (and update is 0), else 1.
///
/// Called by Map_UpdateCurrentTileBlock and Map_SetWindowCellTile (update
/// 1) and by Map_PreloadCurrentTileBlock (update 0). Sets the quarter's entry
/// in perspective->quarterTiles, and borrows HEAP_SLOT_TILE_SCRATCH for the
/// decoded tile.
s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct PerspectiveWork *perspective = gMapWork[0];
  u32 *tileOffsets = perspective->tiles;
  u16 *shownQuarter;
  u16 *decodedTile;
  u16 *sourceTiles;
  u8 *quarterCells;
  u16 *screenTiles;
  u32 rowIndex;
  u32 columnIndex;
  u32 metatileId;

  x &= 1;
  y &= 1;
  shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shownQuarter)
    return 0;
  *shownQuarter = tile;
  decodedTile = Runtime_AllocateHeapBlock(HEAP_SLOT_TILE_SCRATCH, 0x400);
  Resource_DecodeByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);

  // A tile is 16 by 16 cells of 4 bytes. Each layer's block buffer holds
  // 2 by 2 tiles, 32 cells (128 bytes) to a row, so each of the tile's rows
  // goes 64 bytes (16 words) at a time.
  sourceTiles = decodedTile;
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, (DMA_ENABLE | DMA_32BIT) << 16 | 0x10,
            REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  // The affine BG map has one byte a tile, 64 to a row: each cell's first
  // halfword picks a block from Ram_MapCellBuffer, whose two halfwords are
  // its top two tiles and its bottom two, a row (64 bytes) below.
  if (update != 0) {
    screenTiles =
        (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
      for (columnIndex = 0; columnIndex < 16; columnIndex++) {
        u16 *bottomTiles;

        metatileId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + metatileId * 4);
        bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + metatileId * 4);
        screenTiles[32] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      screenTiles += 48;
    }
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_TILE_SCRATCH);
  return 1;
}

// UB: no value is returned, and run_scene.c ignores it; as void, the epilogue
// changes.
/// Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
/// camera work, the projection, the per-scanline ground plane routine in RAM
/// (Transform_UpdateVertices) and its first frame, and the per-frame view and
/// HBlank updates.
///
/// Called through Map_InitializePerspectiveSceneFar when the world map scene
/// starts (run_scene.c). Allocates and clears the perspective work in
/// HEAP_SLOT_MAP, the camera in HEAP_SLOT_CAMERA and the affine pages in
/// HEAP_SLOT_MAP_FRAME; loads the default cells, block attributes and tile
/// animations; writes the blend, BG and affine registers and REG_DISPCNT
/// (DISPCNT_MODE_2); and registers WorldMap_UpdateView and
/// MapAnimation_ApplyAffineFrame with the scheduler.
#ifdef UBFIX
void Map_InitializePerspectiveScene(void) {
#else
s32 Map_InitializePerspectiveScene(void) {
#endif
  struct PerspectiveWork *perspective;
  struct BattleCamera *camera;
  struct MapFrameWork *frameWork;
  struct MapAffinePair(*affinePages)[160];
  s32 *cameraPosition;
  s32 *distance;
  u16 *yaw;
  u16 *pitch;
  u16 *zoomTransitionFrames;
  /* FAKEMATCH(volatile): the fill word is this function's own volatile local,
   * filled by Dma_Fill32Cell, so it takes the native frame slot at sp+28; as
   * Dma_Fill32, 2 of 331 instructions differ in tbs-en. */
  volatile u32 fill;
  struct FixedPointPosition eyeOffset;
  s32 farPlane;
  s32 tileIndex;

  // Hide the backgrounds and sprites while their memory is replaced.
  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  perspective =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MAP, sizeof(struct PerspectiveWork));
  Dma_Fill32Cell(fill, 0, perspective, sizeof(struct PerspectiveWork));
  perspective->viewX = 0;
  perspective->viewY = 0;
  perspective->scaleX = 0x200000;
  perspective->scaleY = 0x400000;
  perspective->limitX = 0x1fe00000;
  perspective->limitY = 0x1fe00000;
  perspective->script = NULL;
  perspective->tiles =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation),
      Ram_MapCollision + 0x1000);
  MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
  Io_Set16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
               BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
           &REG_BLDCNT);
  Io_Set16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  REG_BLDY = 0;
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
      Ram_MapCellBuffer);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes),
      Ram_MapCollision);
  // The ground is BG3 and BG2, two 512-pixel affine layers that wrap (char
  // blocks 2 and 3, screen blocks 8 and 10); BG1 is a plain layer on screen
  // block 5. The affine matrices start as the identity (0x100 is 1.0).
  // fade is the word MapScrollWork calls flags, the REG_DISPCNT bits the
  // scene's fade-in turns on: 0x1f00 is BG0 to BG3 and the sprites.
  perspective->fade = 0x1f00;
  perspective->fadeStep = 0x80;
  Io_Set16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
               BGCNT_WRAP | BGCNT_SIZE(2),
           &REG_BG3CNT);
  Io_Set16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
               BGCNT_WRAP | BGCNT_SIZE(2),
           &REG_BG2CNT);
  Io_Set16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Io_Write16(0x100, &REG_BG2PA);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  Io_Write16(0x100, &REG_BG2PD);
  REG_BG2X = 0;
  REG_BG2Y = 0;
  Io_Write16(0x100, &REG_BG3PA);
  REG_BG3PB = 0;
  REG_BG3PC = 0;
  Io_Write16(0x100, &REG_BG3PD);
  REG_BG3X = 0;
  Io_Write16(0, &REG_BG3Y);

  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  frameWork = Runtime_AllocateHeapBlock(HEAP_SLOT_MAP_FRAME,
                                        sizeof(struct MapFrameWork));
  cameraPosition = camera->focus;
  affinePages = frameWork->pages;
  farPlane = 0x1fe0000;
  perspective->farPlane = farPlane;
  distance = &perspective->distance;
  *distance = farPlane;
  perspective->zoom = 0x10000;
  zoomTransitionFrames = &perspective->zoomTransitionFrames;
  camera->unk18 = 0;
  camera->followPosition = 0;
  *zoomTransitionFrames = 0;
  // A first view from the origin at pitch and yaw 0, so the scanline
  // table and the first affine page exist before the first frame; the
  // projection's centre is 120 by 96, as on the layered maps.
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  SET_VECTOR(cameraPosition, 0, 0, 0);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  yaw = &perspective->yaw;
  SceneTransform_ApplyYaw(*yaw);
  pitch = &perspective->pitch;
  SceneTransform_ApplyPitch(*pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, cameraPosition);
  RamCode_LoadSlot(HEAP_SLOT_BLITTER, Transform_UpdateVertices,
                   &Transform_UpdateVerticesSize);
  WorldMap_BuildScanlineTable(
      Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), cameraPosition,
      frameWork->scanlines);
  gScanlineAngle = 0;
  gScanlinePitch = perspective->pitch;
  ((WorldMapPlaneFn)gWorkSlot[HEAP_SLOT_BLITTER])(camera, cameraPosition,
                                                  frameWork->scanlines,
                                                  affinePages[gFrameCount & 1]);
  // Then the world map's own view: pitch 0xe000 (minus an eighth of a
  // turn) and yaw 0, with the eye one pixel beyond the distance.
  SET_VECTOR(cameraPosition, 0, 0, 0);
  Render_ResetTransformState();
  Io_Put16(pitch, 0xe000);
  *yaw = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(*yaw);
  SceneTransform_ApplyPitch(*pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = *distance + 0x10000;
  Iwram_TransformVector(&eyeOffset, camera->eye);
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
  perspective->windowBottom = 159;
  Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
  Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
  // Each of the 16 by 16 grid cells starts on the tile of its own number.
  for (tileIndex = 255; tileIndex >= 0; tileIndex--)
    perspective->tileIds[tileIndex] = tileIndex;
}

/// Puts world map tile (x / 16, y / 16) at cell (px / 16, py / 16) of the
/// tile id grid, and draws it at once when it is next to the view. px and py
/// count 32-pixel world map cells, so a grid cell is a 512-pixel tile; it is
/// next to the view when px / 8 and py / 8, in 256-pixel steps, are within
/// one step of the followed point's. Layer 1 takes the tile 0x140 (320)
/// further on. Called by the world map scene (FieldMap_SetWindowCellTile,
/// through Map_SetWindowCellTileFar). Sets perspective->tileIds.
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct PerspectiveWork *perspective;
  s32 *followedPosition;
  s32 originX;
  s32 originY;
  s32 tileId;

  perspective = gMapWork[0];
  originX = 0;
  originY = 0;
  followedPosition = perspective->origin;
  if (followedPosition != 0) {
    originX = *followedPosition++;
    originY = followedPosition[1];
  }

  // The followed point and px, py in 256-pixel steps.
  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tileId = (y << 4) + x;
  perspective->tileIds[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tileId;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    Map_WriteLayerCellTile(0, px / 2, py / 2, tileId, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tileId + 0x140, 1);
  }
}

/// Loads the world map's palette and tiles for the area at (x, z): the second
/// set where the ground there is WORLD_MAP_REGION_SNOWFIELD. Fills the BG maps,
/// and for the second set swaps in its own tiles at ids 10-12 and 26-28.
///
/// Called through WorldMap_LoadGraphicsFar when the world map scene starts,
/// with the party's position (run_scene.c). Sets perspective->resources,
/// which Map_LoadAreaGraphics reloads from; writes BG palette colours 1 to
/// 223 (keeping the backdrop), char blocks 2 and 3, gMapLayerData and screen
/// blocks 5 and 6.
void WorldMap_LoadGraphics(s32 x, s32 z) {
  struct PerspectiveWork *perspective;
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
  buffer = MemAlloc(0x200);
  perspective = gMapWork[0];
  // The 32-pixel cell under (x, z) (16.16 / 0x200000), 32 cells to a row.
  if (WORLD_MAP_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)]
          .kind == WORLD_MAP_REGION_SNOWFIELD)
    graphicsSet = 1;
  resources = gWorldMap_GraphicsResources[graphicsSet];
  perspective->resources = resources;
  backdropColor = WORLD_MAP_BG_PALETTE[0];
  Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, WORLD_MAP_BG_PALETTE, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  tiles = gBgTileBuffer;
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]), tiles);
  Dma_Set(tiles, BG_CHAR_BLOCK(2), (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]),
                        Ram_BgTileBuffer + 0x2000);
  Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]),
                        Ram_BgTileBuffer + 0x4000);
  Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_BgTileBuffer + 0x6000);
  Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
  // BG1's map (screen block 5) is filled with tile 0x7f of palette 15, and
  // screen block 6 numbers a 30 by 20 tile picture from tile 0x1a8, two
  // tile numbers a word, for the still view WorldMap_StartStillView shows.
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 0x600);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01a901a8;
  for (screenRow = 0; screenRow < 20; screenRow++) {
    for (screenColumn = 0; screenColumn < 15; screenColumn++) {
      *screenTiles++ = tilePair;
      tilePair += 0x00020002;
    }
    screenTiles++;
  }
  if (graphicsSet == 1) {
    perspective->tileIds[10] = 0x10a;
    perspective->tileIds[11] = 0x10b;
    perspective->tileIds[12] = 0x10c;
    perspective->tileIds[26] = 0x11a;
    perspective->tileIds[27] = 0x11b;
    perspective->tileIds[28] = 0x11c;
    Map_UpdateCurrentTileBlock();
  }
  MemFree(buffer);
}

/// HBlank setup each frame on the world map: copies the first scanline's BG2
/// and BG3 affine parameters, starts the HBlank DMA for the rest, and picks
/// the BG mode from the window's top and bottom.
///
/// Registered with the scheduler by Map_InitializePerspectiveScene (priority
/// 0x480). Reads this frame's affine page (gFrameCount & 1), restarts DMA 0,
/// sets shownTop and shownBottom, writes REG_DISPCNT's mode and clears
/// windowPending.
void MapAnimation_ApplyAffineFrame(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapFrameWork *)WORK_SLOT(HEAP_SLOT_MAP_FRAME))->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)&REG_BG2PA;
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  // Stop last frame's HBlank DMA, then load line 0's eight words (BG2PA to
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
  // DISPCNT_MODE_0 for WorldMap_StartStillView's 200 and 255, which show the
  // still view on BG1.
  windowTop = perspective->shownTop;
  mode = 0;
  if (windowTop < 200) {
    u16 bottom = perspective->shownBottom;

    mode = (bottom != 0) << 1;
    if (windowTop <= bottom) {
      mode = 0;
      if (windowTop == 0)
        mode = 2;
    }
  }
  displayControl |= mode;
  nextDisplayControl = (u16)displayControl;
  REG_DISPCNT = nextDisplayControl;
  perspective->windowPending = 0;
}

/// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void WorldMap_CopyCellTiles(u8 *cellBytes, u8 *screenTile) {
  u32 metatileIndex = *(u16 *)cellBytes;
  u16 *metatileTiles = (u16 *)Ram_MapCellBuffer;

  metatileTiles += metatileIndex * 2;
  *(u16 *)screenTile = *metatileTiles++;
  *(u16 *)(screenTile + 64) = *metatileTiles;
}

/// Draws world map tileRow into the BG map: 32 cells, then 32
/// more a screen block on. Called by WorldMap_UpdateView when the view
/// crosses a row.
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileRow / 2) & 31) << 7);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + ((tileRow & 62) << 6));
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);

  screenTile += 0xfc0;
  sourceCell += 0xf80;
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);
}

/// Draws world map tileColumn into the BG map: 64 cells down. Called by
/// WorldMap_UpdateView when the view crosses a column.
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileColumn / 2) & 31) << 2);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + (tileColumn & 62));
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 128;
    sourceCell += 128;
  } while (cellsCopied <= 63);
}

/// Each frame on the world map: loads tiles as the view moves, shakes and
/// follows the player, draws the BG row or column that scrolls into view,
/// moves the camera, and builds the ground plane's scanline table.
///
/// Registered with the scheduler by Map_InitializePerspectiveScene (priority
/// 0xc85, as Map_UpdateLayerScroll is on layered maps). Sets viewX and viewY,
/// decays the shake, moves the camera and its eye, and runs
/// Transform_UpdateVertices from heap slot 46 into this frame's affine page.
void WorldMap_UpdateView(void) {
  struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  struct MapFrameWork *frameWork = WORK_SLOT(HEAP_SLOT_MAP_FRAME);
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  s32 *cameraPosition = camera->focus;
  s32 *followedPosition = perspective->origin;
  struct MapAffinePair(*affinePages)[160] = frameWork->pages;
  s32 distance = perspective->farPlane;
  s32 height = perspective->distance;
  struct FixedPointPosition eyeOffset;

  Map_PreloadCurrentTileBlock();
  if (followedPosition != NULL) {
    s32 x;
    s32 y;
    s32 tileColumn;
    s32 tileRow;

    y = followedPosition[2];
    x = followedPosition[0];
    if (perspective->shakeX != 0) {
      s32 shakeDifference = Random16();
      s32 shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeX;
      x += Iwram_MulQ16(shakeAmplitude, shakeDifference);
      perspective->shakeX =
          Iwram_MulQ16(shakeAmplitude, perspective->shakeDecay);
    }
    if (perspective->shakeY != 0) {
      s32 shakeDifference = Random16();
      s32 shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeY;
      y += Iwram_MulQ16(shakeAmplitude, shakeDifference);
      perspective->shakeY =
          Iwram_MulQ16(shakeAmplitude, perspective->shakeDecay);
    }
    // On crossing a 16-pixel line, draw the column 16 on that side, or the
    // row 12 on when y grows and 18 back when it falls.
    tileColumn = x / 0x100000;
    tileRow = y / 0x100000;
    if ((perspective->viewX ^ x) & 0x100000) {
      if (perspective->viewX < x)
        Map_RenderPaletteMappedColumn(tileColumn + 16);
      else
        Map_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((perspective->viewY ^ y) & 0x100000) {
      if (perspective->viewY < y)
        Map_RenderPaletteMappedRow(tileRow + 12);
      else
        Map_RenderPaletteMappedRow(tileRow - 18);
    }
    perspective->viewX = x;
    perspective->viewY = y;
  }
  // Rebuild the projection and look at the followed position from the
  // current pitch and yaw, the eye one pixel beyond the zoomed distance.
  // XXX: followedPosition is read here without the NULL test above; whether
  // the world map can run without a followed point would settle if that is
  // safe.
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Projection_Set(distance, height / 2, height * 2);
  SET_VECTOR(cameraPosition, *followedPosition++, 0, followedPosition[1]);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(perspective->yaw);
  SceneTransform_ApplyPitch(perspective->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = height + 0x10000;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, cameraPosition);
  // The scanline table depends only on the pitch, so it is rebuilt only when
  // the pitch has changed.
  if (gScanlinePitch != perspective->pitch) {
    s32 cosine = Trig_Cos(perspective->pitch);
    s32 sine = Trig_Sin(perspective->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                frameWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
  }
  ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[HEAP_SLOT_BLITTER])(
      camera, cameraPosition, frameWork->scanlines,
      affinePages[gFrameCount & 1]);
}

/// Loads and draws the 2x2 world map tiles around the player in both layers.
/// Called through Map_UpdateCurrentTileBlockFar when the world map scene
/// starts, by WorldMap_LoadGraphics for the second graphics set, and by
/// Map_LoadAreaGraphics and Map_LoadDefaultCellsAndUpdateBlock after the
/// still view.
void Map_UpdateCurrentTileBlock(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
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
  // The top left of the 2x2 tiles (512 pixels each, >> 25) around the
  // point, 256 pixels left of it and 320 above.
  firstQuarterX = (firstQuarterX - 0x1000000) >> 25;
  firstQuarterY = (firstQuarterY - 0x1400000) >> 25;
  layer = 0;
  layerTileBase = 0;
  for (; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId =
            (((firstQuarterY + row) & 15) << 4) + ((firstQuarterX + col) & 15);
        tileId = perspective->tileIds[tileId];
        tileId += layerTileBase;
        Map_WriteLayerCellTile(layer, firstQuarterX + col, firstQuarterY + row,
                               tileId, 1);
      }
    }
    layerTileBase += 320;
  }
}

/// Loads the first of the 2x2 world map tiles around the player that is not
/// shown yet (at most one a frame), without drawing it. Called at the start of
/// each WorldMap_UpdateView, so tiles come in one a frame as the player
/// walks.
void Map_PreloadCurrentTileBlock(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
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
  firstQuarterX = (firstQuarterX - 0x1000000) >> 25;
  firstQuarterY = (firstQuarterY - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId =
            (((firstQuarterY + row) & 15) << 4) + ((firstQuarterX + col) & 15);
        tileId = perspective->tileIds[tileId];
        tileId += layer * 320;
        if (Map_WriteLayerCellTile(layer, firstQuarterX + col,
                                   firstQuarterY + row, tileId, 0) != 0)
          return;
      }
    }
  }
}

/// VBlank: shows BG1 and copies the decoded frame's tiles into VRAM. The
/// frame is the still view WorldMap_StartStillView draws: 0x2580 words, 240 by
/// 160 pixels at a byte each, shown as 256-colour tiles on screen block 6. Set
/// as gVBlankHook by WorldMap_StartStillView.
void MapAnimation_PresentFrame(void) {
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
             &REG_BG1CNT);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
}

/// Freezes the world map's perspective view as a still picture on BG1:
/// saves BG1's tiles, draws the current affine frame into pixels and hands
/// the window and VBlank to it.
///
/// Called through WorldMap_StartStillViewFar by WorldMap_StartEvent
/// when a menu or event starts on the world map, and by the world map
/// scene's lighthouse transition. Sets animationActive, suspends the tile
/// animations (MapAnimation_Update), copies 8 KB of char block 1 to
/// gDecodeBuffer, sets the window rows to 200 and 255 (DISPCNT_MODE_0) and sets
/// gVBlankHook to MapAnimation_PresentFrame. Map_LoadAreaGraphics undoes it.
void WorldMap_StartStillView(void) {
  struct MapFrameWork *frameWork = WORK_SLOT(HEAP_SLOT_MAP_FRAME);
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  s32 one = 1;

  perspective->animationActive = one;
  Scheduler_SuspendTasks(MapAnimation_Update);
  Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800, REG_DMA3);
  WaitFrames(1);
  TileMap_DrawRowsFromRam(frameWork->pages[gFrameCount & one],
                          (u8 *)gMapCellBuffer);
  perspective->windowTop = 200;
  perspective->windowBottom = 255;
  gVBlankHook = MapAnimation_PresentFrame;
}

/// VBlank: shows BG1 and copies the area's tiles back into VRAM. Restores
/// BG1CNT as Map_InitializePerspectiveScene set it, and the 32 KB of char
/// blocks 2 and 3 the still view overwrote, from gBgTileBuffer. Set as
/// gVBlankHook by Map_LoadAreaGraphics and Map_LoadDefaultCellsAndUpdateBlock.
void Map_ShowBg1FromBuffer(void) {
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2000, REG_DMA3);
}

/// Reloads the world map area's palette and tiles after the still view, puts
/// the default cells back and returns to the perspective view.
///
/// Called through Map_LoadAreaGraphicsFar by
/// WorldMap_EndEvent when a menu or event ends, by
/// Map_ShowWorldMap, and by the world map scene's lighthouse transition.
/// Decodes into gMapCellBuffer and gBgTileBuffer, sets gVBlankHook to
/// Map_ShowBg1FromBuffer, sets the window rows back to 0 and 159, clears
/// animationActive and resumes the tile animations. Waits two frames.
void Map_LoadAreaGraphics(void) {
  struct PerspectiveWork *perspective;
  u32 *graphicsResources;
  u8 *decodeBuffer;
  s16 backdropColor;

  perspective = gMapWork[0];
  decodeBuffer = (u8 *)gMapCellBuffer;
  graphicsResources = perspective->resources;
  backdropColor = *(s16 *)BG_PLTT;
  Resource_DecodeByteLz(Resource_GetTableEntry(graphicsResources[0]),
                        decodeBuffer);
  *(s16 *)decodeBuffer = backdropColor;
  Dma_Set(decodeBuffer, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[1]),
                        gBgTileBuffer);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[2]),
                        Ram_BgTileBuffer + 0x2000);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[3]),
                        Ram_BgTileBuffer + 0x4000);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[4]),
                        Ram_BgTileBuffer + 0x6000);
  // The VBlank copies the tiles back and the next frame shows the ground;
  // only then is the still picture's buffer refilled with the cells.
  gVBlankHook = Map_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), decodeBuffer);
  Map_UpdateCurrentTileBlock();
  perspective->animationActive = 0;
  Scheduler_ResumeTasks(MapAnimation_Update);
  WaitFrames(1);
}

/// Puts the default world map cells back after the still view and returns
/// to the perspective view, without reloading the area's palette or tiles.
/// Called three times by the world map scene's events. Unlike
/// Map_LoadAreaGraphics it leaves animationActive set.
/// XXX: whether leaving animationActive set is intended; a scene that calls
/// it and then opens a menu would show the effect.
void Map_LoadDefaultCellsAndUpdateBlock(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
  gVBlankHook = Map_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  Map_UpdateCurrentTileBlock();
  Scheduler_ResumeTasks(MapAnimation_Update);
  WaitFrames(1);
}

#undef WORLD_MAP_BG_PALETTE
#undef WORLD_MAP_CELLS
