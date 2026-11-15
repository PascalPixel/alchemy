/// @file
/// The world map: its perspective view, its tiles, and drawing its cells into
/// a BG map.
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"
#include "field/field.h"
#include "field/resource_slot.h"
#include "system/system.h"
#include "resource_ids.h"
#include "system/transform.h"
#include "macros.h"
#include "field/map.h"
#include "field/object.h"
#include "battle/battle_camera.h"
#include "field/world_map.h"

#define WORLD_MAP_BG_PALETTE ((s16 *)BG_PLTT)

/// Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
/// camera work, the projection, the per-scanline ground plane routine in RAM
/// (Transform_UpdateVertices) and its first frame, and the per-frame view and
/// HBlank updates.
UNREAD_RESULT WorldMap_InitializePerspective(void) {
  struct WorldMapView *perspective;
  struct BattleCamera *camera;
  struct WorldMapAffineWork *affineWork;
  struct WorldMapAffinePair(*affinePages)[DISPLAY_HEIGHT];
  s32 *cameraPosition;
  volatile u32 fill;
  s32 farPlane;
  s32 tileIndex;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  ScreenFade_FromBlack(0);
  perspective = HeapSlot_Alloc(HEAP_SLOT_MAP_GRID, sizeof(struct WorldMapView));
  Dma_Fill32Cell(fill, 0, perspective, sizeof(struct WorldMapView));
  perspective->viewX = 0;
  perspective->viewY = 0;
  perspective->scaleX = Q16_ONE * 32;
  perspective->scaleY = Q16_ONE * 64;
  perspective->limitX = px(16352);
  perspective->limitY = px(16352);
  perspective->script = NULL;
  perspective->tiles =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  REG_BLDY = 0;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      gMapGridCellBuffer);
  perspective->fade = DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON |
                      DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
  perspective->fadeStep = 128;
  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapEntrances), gMapScene);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG3CNT);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5) | BGCNT_SIZE(2),
             &REG_BG1CNT);
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
  affinePages = affineWork->pages;
  farPlane = px(510);
  perspective->farPlane = farPlane;
  perspective->distance = farPlane;
  perspective->zoom = Q16_ONE;
  cameraPosition = camera->focus;
  perspective->zoomTransitionFrames = 0;
  camera->followEye = 0;
  camera->followPosition = 0;
  // A first view from the origin at pitch and yaw 0, so the scanline
  // table and the first affine page exist before the first frame; the
  // projection's centre is 120 by 96, as on the layered maps.
  gProjection.centerX = DISPLAY_WIDTH / 2;
  gProjection.centerY = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  SET_VECTOR(cameraPosition, 0, 0, 0);
  Transform_Reset();
  Transform_ApplyPosition(cameraPosition);
  Transform_ApplyYaw(perspective->yaw);
  Transform_ApplyPitch(perspective->pitch);
  // eyeOffset in the block that uses it, as The Broken Seal declares it.
  {
    struct FixedPointPosition eyeOffset;

    SET_POSITION(&eyeOffset, 0, 0, farPlane);
    Iwram_TransformVector(&eyeOffset, camera->eye);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, cameraPosition);
    RamCode_LoadSlot(HEAP_SLOT_RAM_CODE, Transform_UpdateVerticesCode,
                     Transform_UpdateVerticesCodeSize);
    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(Trig_Cos(perspective->pitch),
                                                  Trig_Sin(perspective->pitch)),
                                cameraPosition, affineWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
    ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->ramCode)(
        camera, cameraPosition, affineWork->scanlines,
        affinePages[gFrameCount & 1]);
    // Then the world map's own view: pitch TRIG_TURN * 7 / 8 (minus an eighth
    // of a turn) and yaw 0, with the eye one pixel beyond the distance.
    SET_VECTOR(cameraPosition, 0, 0, 0);
    Transform_Reset();
    perspective->pitch = TRIG_TURN * 7 / 8;
    perspective->yaw = 0;
    Transform_Reset();
    Transform_ApplyPosition(cameraPosition);
    Transform_ApplyYaw(perspective->yaw);
    Transform_ApplyPitch(perspective->pitch);
    if (perspective->fixedEye == 0) {
      SET_POSITION(&eyeOffset, 0, 0, perspective->distance + Q16_ONE);
      Iwram_TransformVector(&eyeOffset, camera->eye);
    }
  }
  REG_MOSAIC = 0;
  Io_Write16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  Task_Add(WorldMap_UpdateView, TASK_PASS_VBLANK + 5);
  Task_Add(WorldMap_ApplyAffinePage, TASK_PASS_FRAME);
  for (tileIndex = 0; tileIndex <= 0x3ff; tileIndex++)
    perspective->tileIds[tileIndex] = tileIndex;
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// Puts world map tile (x / 16, y / 16) at cell (px / 16, py / 16) of the tile
/// id grid, and draws it at once when it is next to the view.
void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct WorldMapView *perspective;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  perspective = Ram_HeapSlots->mapGrid;
  originX = 0;
  originY = 0;
  position = perspective->origin;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  perspective->tileIds[(((py / 2) & 31) << 5) + ((px / 2) & 31)] = tile;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    WorldMap_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    WorldMap_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
  }
}

/// WorldMap_SetWindowCellTile without its view test: puts world map tile
/// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it in
/// both layers.
void WorldMap_SetWindowCellTileAndDraw(s32 x, s32 y, s32 px, s32 py) {
  struct WorldMapView *perspective;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  perspective = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  perspective->tileIds[(((py / 2) & 31) << 5) + ((px / 2) & 31)] = tile;
  WorldMap_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  WorldMap_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}

/// Loads the world map's palette and tiles for the area at (x, z): the set
/// its region picks, its tint, and the plain BG maps the world map draws over.
void WorldMap_LoadGraphics(s32 x, s32 z) {
  u8 *buffer;
  struct WorldMapView *perspective;
  struct WorldMapCell *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 region;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(512);
  perspective = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  block =
      &((struct WorldMapCell *)
            Ram_MapBlocks)[((x / px(32)) & 31) + (((z / px(32)) & 31) << 5)];
  WorldMap_UpdateRegionTint(block->region, 0);
  // Reused: the region, then its group of ten.
  region = block->region;
  region /= 10;
  switch (region) {
  case 3:
    graphicsSet = 3;
    break;
  case 2:
    graphicsSet = 2;
    break;
  case 4:
    graphicsSet = 1;
    break;
  default:
    graphicsSet = 0;
    break;
  }
  resources = gWorldMap_GraphicsResources[graphicsSet];
  perspective->resources = resources;
  backdropColor = WORLD_MAP_BG_PALETTE[0];
  Decompress_ByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_LoadPaletteFar(buffer);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[1]),
                      MAPGRID_ANIMATION_SCRIPT);
  TilesetAnim_StartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer);
  Dma_Set(Ram_BgTileBuffer, BG_CHAR_BLOCK(2),
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                      WORLD_MAP_TILE_SET(1));
  Dma_Set(WORLD_MAP_TILE_SET(1), BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]),
                      WORLD_MAP_TILE_SET(2));
  Dma_Set(WORLD_MAP_TILE_SET(2), BG_CHAR_BLOCK(3),
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[6]),
                      WORLD_MAP_TILE_SET(3));
  Dma_Set(WORLD_MAP_TILE_SET(3), BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gMapGridAnimTiles);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), &gWorldMapWork);
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 1536);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01810180;
  for (row = 0; row < 20; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x00020002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 24; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  tilePair = 0x01010100;
  for (row = 0; row < 4; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
    }
  }
  Heap_Release(buffer);
}

/// HBlank setup each VBlank on the world map: copies the first scanline's BG2
/// and BG3 affine parameters, starts the HBlank DMA for the rest, and picks the
/// BG mode from the window's top and bottom.
void WorldMap_ApplyAffinePage(void) {
  struct WorldMapAffinePair(*affinePages)[160] =
      ((struct WorldMapAffineWork *)Ram_HeapSlots->worldMapAffine)->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)0x04000020;
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  dma0[5] &= 0xc5ff;
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
    Dma_Set(affineSource, (void *)0x04000020, 0xa6600008, (u32 *)dma0);
  }
  perspective->shownTop = perspective->windowTop;
  perspective->shownBottom = perspective->windowBottom;
  mode = 0;
  windowTop = perspective->shownTop;
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
  u32 cellPictureIndex = *(u16 *)cellBytes;
  u16 *cellPictureTiles = (u16 *)Ram_MapCellBuffer;

  cellPictureTiles += cellPictureIndex * 2;
  *(u16 *)screenTile = *cellPictureTiles++;
  *(u16 *)(screenTile + 64) = *cellPictureTiles;
}

/// Draws world map tileRow into the BG map: 32 cells, then 32 more a screen
/// block on.
void WorldMap_RenderPaletteMappedRow(u32 tileRow) {
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

  screenTile += 4032;
  sourceCell += 3968;
  cellsCopied = 0;
  do {
    WorldMap_CopyCellTiles(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);
}

/// Draws world map tileColumn into the BG map: 64 cells down.
void WorldMap_RenderPaletteMappedColumn(u32 tileColumn) {
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

/// Each frame: follows the followed point with the screen shake, draws the rows
/// and columns that came into view, and recomputes the projection and the
/// ground plane for the current pitch and yaw; while the still picture shows,
/// only BG1 shakes.
void WorldMap_UpdateView(void) {
  u8 *camera = Ram_HeapSlots->cameraWork;
  u8 *affineWork = Ram_HeapSlots->worldMapAffine;
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 *cameraPosition = (s32 *)(camera + 12);
  s32 *followedPosition = perspective->origin;
  u8 *affinePages = affineWork + 0xc80;
  s32 distance = perspective->farPlane;
  s32 height = perspective->distance;
  s32 tileRow;
  s32 shakeOffsetX;
  s32 shakeOffsetY;
  struct FixedPointPosition eyeOffset;

  shakeOffsetX = 0;
  shakeOffsetY = 0;
  if (perspective->viewHeld != 0) {
    if (perspective->shakeX == 0 && perspective->shakeY == 0)
      return;
  } else {
    WorldMap_PreloadCurrentTileBlock();
  }
  if (followedPosition != NULL) {
    s32 x = followedPosition[0];
    s32 y = followedPosition[2];
    s32 tileColumn;

    if (perspective->shakeX != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetX = Iwram_MulQ16(perspective->shakeX, shakeDifference);
      x += shakeOffsetX;
      perspective->shakeX =
          Iwram_MulQ16(perspective->shakeX, perspective->shakeDecay);
    }
    if (perspective->shakeY != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetY = Iwram_MulQ16(perspective->shakeY, shakeDifference);
      y += shakeOffsetY;
      perspective->shakeY =
          Iwram_MulQ16(perspective->shakeY, perspective->shakeDecay);
    }
    tileColumn = x / px(16);
    tileRow = y / px(16);
    if ((perspective->viewX ^ x) & 0x100000) {
      if (perspective->viewX < x)
        WorldMap_RenderPaletteMappedColumn(tileColumn + 16);
      else
        WorldMap_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((perspective->viewY ^ y) & 0x100000) {
      if (perspective->viewY < y)
        WorldMap_RenderPaletteMappedRow(tileRow + 12);
      else
        WorldMap_RenderPaletteMappedRow(tileRow - 18);
    }
    perspective->viewX = x;
    perspective->viewY = y;
  }
  cameraPosition[0] = perspective->viewX;
  cameraPosition[1] = 0;
  cameraPosition[2] = perspective->viewY;
  if (perspective->stillPictureShown != 0) {
    gBgScroll[1].x = shakeOffsetX / Q16_ONE;
    gBgScroll[1].y = shakeOffsetY / Q16_ONE;
    return;
  }
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Projection_Set(distance, height / 2, height * 2);
  Transform_Reset();
  Transform_ApplyPosition(cameraPosition);
  Transform_ApplyYaw(perspective->yaw);
  Transform_ApplyPitch(perspective->pitch);
  if (perspective->fixedEye == 0) {
    SET_POSITION(&eyeOffset, 0, 0, height + 0x10000);
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  Transform_Reset();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  if (gScanlinePitch != perspective->pitch) {
    s32 cosine = Trig_Cos(perspective->pitch);
    s32 sine = Trig_Sin(perspective->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                affineWork);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
  }
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->ramCode)(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
}

/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlock(void) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 x0 = 0;
  s32 y0 = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 layerTileBase;
  s32 tile;

  if (perspective->origin != NULL) {
    s32 *p = perspective->origin;

    x0 = *p++;
    y0 = p[1];
  }
  x0 = (x0 - 0x1000000) >> 25;
  y0 = (y0 - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = perspective->tileIds[tile];
        tile += layer << 10;
        WorldMap_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
      }
    }
  }
}
