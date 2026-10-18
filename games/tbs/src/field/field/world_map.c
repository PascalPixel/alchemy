// Part of field.c, the field object, compiled only through it.
// The world map: its perspective view, its tiles and its BG1 animation.

#include "../field_internal.h"

// Writes a halfword register (value first).
static __inline__ void Io_Set16(s32 value, volatile u16 *reg) { *reg = value; }

// Writes a halfword register (register first).
static __inline__ void Io_Put16(volatile u16 *reg, s32 value) { *reg = value; }

typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

struct WorldCell {
  u32 tile : 24;
  u32 kind : 7;
  u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

// Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's block
// buffer, decompressing it from the area's graphics, and with update set
// also draws it into the layer's BG map. Returns 0 when that quarter already
// shows the tile (and update is 0), else 1.
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
  decodedTile = Runtime_AllocateHeapBlock(14, 0x400);
  Resource_DecodeByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);

  sourceTiles = decodedTile;
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, (DMA_ENABLE | DMA_32BIT) << 16 | 0x10,
            REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

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
  Runtime_ReleaseHeapBlock(14);
  return 1;
}

// Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
// camera work, the projection, the per-scanline ground plane routine in RAM
// (Transform_UpdateVertices) and its first frame, and the per-frame view and
// HBlank updates.
s32 Map_InitializePerspectiveScene(void) {
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
  /* FAKEMATCH(one-pass): its loop notes keep the 0x100 for BG2PD built just
   * before its store and the 12 for Runtime_AllocateBlock loaded before the 76;
   * as plain statements, 3 of 331 instructions differ in tbs-en. */
  do {
    Io_Put16(&REG_BG2PD, 0x100);
    REG_BG2X = 0;
    REG_BG2Y = 0;
    Io_Put16(&REG_BG3PA, 0x100);
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    Io_Put16(&REG_BG3PD, 0x100);
    REG_BG3X = 0;
    REG_BG3Y = 0;
  } while (0);

  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  frameWork = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
  cameraPosition = camera->pos;
  affinePages = frameWork->pages;
  farPlane = 0x1fe0000;
  perspective->farPlane = farPlane;
  distance = &perspective->distance;
  *distance = farPlane;
  perspective->zoom = 0x10000;
  zoomTransitionFrames = &perspective->zoomTransitionFrames;
  camera->unk18 = 0;
  camera->followPos = 0;
  *zoomTransitionFrames = 0;
  // Rebuild the projection around the followed position before the affine pass.
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
  RamCode_LoadSlot(46, Transform_UpdateVertices, &Transform_UpdateVerticesSize);
  WorldMap_BuildScanlineTable(
      Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), cameraPosition,
      frameWork->scanlines);
  gScanlineAngle = 0;
  gScanlinePitch = perspective->pitch;
  ((PlaneFn)gWorkSlot[46])(camera, cameraPosition, frameWork->scanlines,
                           affinePages[gFrameCount & 1]);
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
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
  Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
  for (tileIndex = 255; tileIndex >= 0; tileIndex--)
    perspective->tileIds[tileIndex] = tileIndex;
}

// Puts world map tile (x / 16, y / 16) at cell (px / 16, py / 16) of the
// tile id grid, and draws it at once when it is next to the view.
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

// Loads the world map's palette and tiles for the area at (x, z): the second
// set where the ground there is terrain kind 21. Fills the BG maps, and for
// the second set swaps in its own tiles at ids 10-12 and 26-28.
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
  if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind ==
      21)
    graphicsSet = 1;
  resources = gWorldMap_GraphicsResources[graphicsSet];
  perspective->resources = resources;
  backdropColor = BG_PALETTE[0];
  Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, BG_PALETTE, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
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

// HBlank setup each frame on the world map: copies the first scanline's BG2
// and BG3 affine parameters, starts the HBlank DMA for the rest, and turns
// the window on or off for the window's top and bottom.
void MapAnimation_ApplyAffineFrame(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapFrameWork *)WORK_SLOT(7))->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)&REG_BG2PA;
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

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

// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyWorldMapTileColumn(u8 *cellBytes, u8 *screenTile) {
  u32 metatileIndex = *(u16 *)cellBytes;
  u16 *metatileTiles = (u16 *)Ram_MapCellBuffer;

  metatileTiles += metatileIndex * 2;
  *(u16 *)screenTile = *metatileTiles++;
  *(u16 *)(screenTile + 64) = *metatileTiles;
}

// Draws world map tileRow into the BG map: 32 cells, then 32
// more a screen block on.
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileRow / 2) & 31) << 7);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + ((tileRow & 62) << 6));
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);

  screenTile += 0xfc0;
  sourceCell += 0xf80;
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);
}

// Draws world map tileColumn into the BG map: 64 cells down.
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileColumn / 2) & 31) << 2);
  screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + (tileColumn & 62));
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 128;
    sourceCell += 128;
  } while (cellsCopied <= 63);
}

// Each frame on the world map: loads tiles as the view moves, shakes and
// follows the player, draws the BG row or column that scrolls into view,
// moves the camera, and builds the ground plane's scanline table.
void WorldMap_UpdateView(void) {
  struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  struct MapFrameWork *frameWork = WORK_SLOT(7);
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  s32 *cameraPosition = camera->pos;
  s32 *followedPosition = perspective->origin;
  struct MapAffinePair(*affinePages)[160] = frameWork->pages;
  s32 distance = perspective->farPlane;
  s32 height = perspective->distance;
  struct FixedPointPosition eyeOffset;

  Map_UpdateCurrentTileBlockUntilBlocked();
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
  // Rebuild the projection around the followed position before the affine pass.
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
  // Pitch changes require a new ground-plane intersection for every scanline.
  if (gScanlinePitch != perspective->pitch) {
    s32 cosine = Trig_Cos(perspective->pitch);
    s32 sine = Trig_Sin(perspective->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                frameWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
  }
  ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(
      camera, cameraPosition, frameWork->scanlines,
      affinePages[gFrameCount & 1]);
}

// Loads and draws the 2x2 world map tiles around the player in both layers.
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

// Loads the first of the 2x2 world map tiles around the player that is not
// shown yet (at most one a frame), without drawing it.
void Map_UpdateCurrentTileBlockUntilBlocked(void) {
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

// VBlank: shows BG1 and copies the decoded frame's tiles into VRAM.
void MapAnimation_PresentFrame(void) {
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
             &REG_BG1CNT);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
}

// Starts a full-screen BG1 animation on the world map: saves BG1's tiles,
// decodes the first frame and hands the window and VBlank to it.
void MapAnimation_Start(void) {
  struct MapFrameWork *frameWork = WORK_SLOT(7);
  struct PerspectiveWork *perspective = WORK_SLOT(HEAP_SLOT_MAP);
  s32 one = 1;

  perspective->animationActive = one;
  Scheduler_SuspendTasks(MapAnimation_Update);
  Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800, REG_DMA3);
  WaitFrames(1);
  Resource_RunCopiedDecoder((s32)frameWork->pages[gFrameCount & one],
                            (s32)gMapCellBuffer);
  perspective->windowTop = 200;
  perspective->windowBottom = 255;
  gVBlankHook = MapAnimation_PresentFrame;
}

// VBlank: shows BG1 and copies the area's tiles back into VRAM.
void Map_ShowBg1FromBuffer(void) {
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2000, REG_DMA3);
}

// Reloads the world map area's palette and tiles after a BG1 animation, puts
// the default cells back and stops the animation.
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

// Puts the default world map cells back after a BG1 animation and stops it.
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
