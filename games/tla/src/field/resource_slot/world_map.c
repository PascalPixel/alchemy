/// @file
/// The world map's window cells, and drawing its tiles into a BG map, with
/// routines not yet understood.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"
#include "fixed_math.h"
#include "module/field/field.h"
#include "module/field/resource_slot.h"
#include "module/system/system.h"
#include "resource_ids.h"
#include "transform.h"

/* VIEW(resource_slot): routine Projection_Set as void Projection_Set(s32
 * distance, s32 nearPlane, s32 farPlane); module/system/system.h has void
 * Projection_Set(u32 value0, u32 value1, u32 value2). */
extern void
Projection_Set__resource_slot(s32 distance, s32 nearPlane,
                              s32 farPlane) __asm__("Projection_Set");
/* VIEW(f02b998): routine Transform_Reset as u32
 * Transform_Reset(void); graphics.h has void
 * Transform_Reset(void); tried as the definition's void
 * Transform_Reset(void), which the object already declares,
 * WorldMap_InitializePerspective moves: 1 of 372 instructions in tla-ja. */
extern u32 Render_ResetTransformState__f02b998(void) __asm__(
    "Transform_Reset"); // Returns void; see the tag.
/* VIEW(resource_slot): routine SceneTransform_ApplyPosition as void
 * SceneTransform_ApplyPosition(void *position); module/system/system.h has void
 * SceneTransform_ApplyPosition(const s32 *pos). */
extern void SceneTransform_ApplyPosition__resource_slot(void *position) __asm__(
    "SceneTransform_ApplyPosition");
void WorldMap_BuildScanlineTable(s32 ratio, s32 *position, void *affineWork);

void WorldMap_InitializePerspective(void) {
  /* FAKEMATCH(signature): Transform_Reset declared returning u32;
   * as void, 2 of 376 instructions differ in tla-en. */
  u8 *perspective;
  struct WorldView *view;
  u8 *camera;
  u8 *affineWork;
  u8 *affinePages;
  s32 *cameraPosition;
  volatile u32 fill;
  struct EyeOffset eyeOffset;
  s32 farPlane;
  s32 tileIndex;

  REG_DISPCNT &= 0xc1ff;
  ScreenFade_FromBlack(0);
  perspective = HeapSlot_Alloc(32, 0x978);
  view = (struct WorldView *)perspective;
  Dma_Fill32Cell(fill, 0, perspective, 0x978);
  view->viewX = 0;
  view->viewY = 0;
  *(s32 *)(perspective + 0xec) = 0x200000;
  *(s32 *)(perspective + 0xf0) = 0x400000;
  *(s32 *)(perspective + 0xf4) = 0x3fe00000;
  *(s32 *)(perspective + 0xf8) = 0x3fe00000;
  view->unk010 = NULL;
  *(void **)(perspective + 0x114) =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Io_Write16(0x3f9e, &REG_BLDCNT);
  Io_Write16(0x1010, &REG_BLDALPHA);
  REG_BLDY = 0;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      gMapCellBuffer);
  view->unk014 = 0x1f00;
  view->unk016 = 0x80;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Data199),
                      gData_0202e000);
  Io_Write16(0xa80a, &REG_BG3CNT);
  Io_Write16(0xaa0e, &REG_BG2CNT);
  Io_Write16(0x8501, &REG_BG1CNT);
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

  camera = HeapSlot_AllocEwram(48, 76);
  affineWork = HeapSlot_Alloc(28, 0x3484);
  affinePages = affineWork + 0xc80;
  farPlane = 0x1fe0000;
  view->farPlane = farPlane;
  view->distance = farPlane;
  view->unk968 = 0x10000;
  cameraPosition = (s32 *)(camera + 12);
  view->unk96c = 0;
  *(s32 *)(camera + 24) = 0;
  *(s32 *)(camera + 28) = 0;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 96;
  Projection_Set__resource_slot(farPlane, farPlane >> 1, farPlane << 1);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState__f02b998();
  SceneTransform_ApplyPosition__resource_slot(cameraPosition);
  Transform_ApplyYaw(view->yaw);
  Transform_ApplyPitch(view->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState__f02b998();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  RamCode_LoadSlot(104, Transform_UpdateVerticesCode,
                   Transform_UpdateVerticesCodeSize);
  WorldMap_BuildScanlineTable(
      Iwram_RatioMulQ14(Trig_Cos(view->pitch), Trig_Sin(view->pitch)),
      cameraPosition, affineWork);
  gScanlineAngle = 0;
  gScanlinePitch = view->pitch;
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState__f02b998();
  view->pitch = 0xe000;
  view->yaw = 0;
  Render_ResetTransformState__f02b998();
  SceneTransform_ApplyPosition__resource_slot(cameraPosition);
  Transform_ApplyYaw(view->yaw);
  Transform_ApplyPitch(view->pitch);
  if (view->fixedEye == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = view->distance + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  REG_MOSAIC = 0;
  Io_Write16(0x42, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  view->windowTop = 0;
  view->windowBottom = 159;
  Task_Add(WorldMap_UpdateView, 0x485);
  Task_Add(WorldMap_ApplyAffinePage, 0xc80);
  for (tileIndex = 0; tileIndex <= 0x3ff; tileIndex++)
    ((u16 *)(perspective + 0x148))[tileIndex] = tileIndex;
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  originX = 0;
  originY = 0;
  position = window->position;
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
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    WorldMap_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    WorldMap_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
  }
}

/// WorldMap_SetWindowCellTile without its view test: puts world map tile
/// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it in
/// both layers.
void WorldMap_SetWindowCellTileAndDraw(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;
  WorldMap_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  WorldMap_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}

#include "fixed_math.h"
#include "module/field/field.h"
#include "module/field/map.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"
#include "module/system/system.h"
#include "transform.h"

/* VIEW(world_map): variable gWorldMapWork as extern u8 gWorldMapWork[];
 * battle.h has extern struct WorldMapWork gWorldMapWork; tried as the rest of
 * the object declares it, it does not compile in tla-ja:
 * games/tla/src/field/resource_slot.c:8903: `gWorldMapWork' has an incomplete
 * type. */
extern u8 gWorldMapWork__world_map[] __asm__("gWorldMapWork");

void WorldMap_LoadGraphics(s32 x, s32 z) {
  u8 *buffer;
  struct MapViewWork *view;
  struct Data02020000 *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 region;
  s16 backdropColor;
  u32 *screenTiles;
  u32 tilePair;
  s32 row;
  s32 column;

  buffer = Heap_Alloc(512);
  view = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  block =
      &((struct Data02020000 *)Ram_MapBlocks)[((x / (32 << 16)) & 31) +
                                              (((z / (32 << 16)) & 31) << 5)];
  Func_0802d088(block->region, 0);
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
  resources = gData_0802ed34[graphicsSet];
  view->resources = (u32 *)resources;
  backdropColor = *(s16 *)BG_PLTT;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_LoadBufferPaletteFar(buffer);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[1]),
                      Ram_MapCollision + 0x1000);
  TilesetAnim_StartAll((const u16 *)(Ram_MapCollision + 0x1000));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer);
  Dma_Set(Ram_BgTileBuffer, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(2048), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                      Ram_BgTileBuffer + 0x2000);
  Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]),
                      Ram_BgTileBuffer + 0x4000);
  Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_COPY_WORDS(2048),
          REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[6]),
                      Ram_BgTileBuffer + 0x6000);
  Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gData_02028000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[8]),
                      gWorldMapWork__world_map);
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 1536);
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01810180;
  for (row = 0; row < 20; row++) {
    for (column = 15; column >= 0; column--) {
      *screenTiles++ = tilePair;
      tilePair += 0x20002;
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

void WorldMap_ApplyAffinePage(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapAffineWork *)Ram_HeapSlots->unk1c[0])->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)0x04000020;
  struct PerspectiveWindow *perspective =
      (struct PerspectiveWindow *)Ram_HeapSlots->mapWork;
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  dma0[5] &= 0xc5ff;
  dma0[5] &= 0x7fff;
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

/* VIEW(world_map): variable gMapBlocks as extern u8 gMapBlocks[]; f0298c0 has
 * extern u32 gMapBlocks[]; tried as the rest of the object declares it, it does
 * not compile in tla-ja: games/tla/src/field/resource_slot.c:9021:
 * `gMapBlocks__world_map' undeclared (first use in this function). */
extern u8 gMapBlocks__world_map[] __asm__("gMapBlocks");

/// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyWorldMapTileColumn(u8 *cellBytes, u8 *screenTile) {
  u32 cellPictureIndex = *(u16 *)cellBytes;
  u16 *cellPictureTiles = (u16 *)Ram_MapCellBuffer;

  cellPictureTiles += cellPictureIndex * 2;
  *(u16 *)screenTile = *cellPictureTiles++;
  *(u16 *)(screenTile + 64) = *cellPictureTiles;
}

/// Draws world map tile row tileRow (two screen rows) into the BG map at
/// 0x06004000, from the 32 by 32 cell grid in gMapBlocks.
void WorldMap_RenderPaletteMappedRow(u32 tileRow) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 column;

  cellBytes = gMapBlocks__world_map + ((((s32)tileRow / 2) & 31) << 7);
  {
    /* FAKEMATCH(pin): the BG map address held in r5, loaded right after the
     * cell address is formed; as a plain local, 4 of 57 instructions differ in
     * tla-en. */
    register u32 screenBase asm("r5") = 0x06004000;

    /* FAKEMATCH(barrier): the empty asm holding the pin above, which keeps the
     * address's load where it is; without it, 10 of 57 instructions differ in
     * tla-en. */
    __asm__ volatile("" : "+l"(screenBase));
    screenTile = (u8 *)(((tileRow & 62) << 6) + screenBase);
  }
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);

  {
    /* FAKEMATCH(pin): the step to the next half's screen row held in r2; as a
     * plain local, built in r3, 6 of 57 instructions differ in tla-en. */
    register u32 screenStep asm("r2") = 0xfc0;

    /* FAKEMATCH(barrier): the empty asm holding the pin above; without it, 10
     * of 57 instructions differ in tla-en. */
    __asm__("" : "+l"(screenStep));
    screenTile += screenStep;
  }
  cellBytes += 0xf80;
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);
}

/// Draws world map tile column tileColumn (two screen columns) into the BG map
/// at 0x06004000.
void WorldMap_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 row;
  /* FAKEMATCH(pin): the BG map address held in r5, loaded between the cell
   * offset's shift and add; as a plain local, 4 of 32 instructions differ in
   * tla-en. */
  register u32 screenBase asm("r5") = 0x06004000;

  cellBytes = gMapBlocks__world_map + ((((s32)tileColumn / 2) & 31) << 2);
  /* FAKEMATCH(barrier): the empty asm holding the pin above after the cell
   * address; without it, 4 of 32 instructions differ in tla-en. */
  __asm__ volatile("" : "+l"(screenBase));
  screenTile = (u8 *)((tileColumn & 62) + screenBase);
  row = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    row++;
    screenTile += 128;
    cellBytes += 128;
  } while (row <= 63);
}

u32 Random16(void);
void WorldMap_BuildScanlineTable(s32 ratio, s32 *position, void *affineWork);

void WorldMap_UpdateView(void) {
  u8 *camera = Ram_HeapSlots->cameraWork;
  u8 *affineWork = Ram_HeapSlots->unk1c[0];
  struct WorldView *view = (struct WorldView *)Ram_HeapSlots->mapWork;
  s32 *cameraPosition = (s32 *)(camera + 12);
  s32 *followedPosition = view->origin;
  u8 *affinePages = affineWork + 0xc80;
  s32 distance = view->farPlane;
  s32 height = view->distance;
  s32 tileRow;
  s32 shakeOffsetX;
  s32 shakeOffsetY;
  struct EyeOffset eyeOffset;

  shakeOffsetX = 0;
  shakeOffsetY = 0;
  if (view->paused != 0) {
    if (view->shakeX == 0 && view->shakeY == 0)
      return;
  } else {
    Func_0802c58c();
  }
  if (followedPosition != NULL) {
    s32 x = followedPosition[0];
    s32 y = followedPosition[2];
    s32 tileColumn;

    if (view->shakeX != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetX = Iwram_MulQ16(view->shakeX, shakeDifference);
      x += shakeOffsetX;
      view->shakeX = Iwram_MulQ16(view->shakeX, view->shakeDecay);
    }
    if (view->shakeY != 0) {
      s32 shakeDifference = Random16();

      shakeDifference -= Random16();
      shakeOffsetY = Iwram_MulQ16(view->shakeY, shakeDifference);
      y += shakeOffsetY;
      view->shakeY = Iwram_MulQ16(view->shakeY, view->shakeDecay);
    }
    tileColumn = x / 0x100000;
    tileRow = y / 0x100000;
    if ((view->viewX ^ x) & 0x100000) {
      if (view->viewX < x)
        WorldMap_RenderPaletteMappedColumn(tileColumn + 16);
      else
        WorldMap_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((view->viewY ^ y) & 0x100000) {
      if (view->viewY < y)
        WorldMap_RenderPaletteMappedRow(tileRow + 12);
      else
        WorldMap_RenderPaletteMappedRow(tileRow - 18);
    }
    view->viewX = x;
    view->viewY = y;
  }
  cameraPosition[0] = view->viewX;
  cameraPosition[1] = 0;
  cameraPosition[2] = view->viewY;
  if (view->flat != 0) {
    gBgScroll[1].x = shakeOffsetX / 0x10000;
    gBgScroll[1].y = shakeOffsetY / 0x10000;
    return;
  }
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 96;
  Projection_Set__resource_slot(distance, height / 2, height * 2);
  Transform_Reset();
  SceneTransform_ApplyPosition__resource_slot(cameraPosition);
  Transform_ApplyYaw(view->yaw);
  Transform_ApplyPitch(view->pitch);
  if (view->fixedEye == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = height + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  Transform_Reset();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  if (gScanlinePitch != view->pitch) {
    s32 cosine = Trig_Cos(view->pitch);
    s32 sine = Trig_Sin(view->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                affineWork);
    gScanlineAngle = 0;
    gScanlinePitch = view->pitch;
  }
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
}

/* VIEW(resource_slot): tag struct MapTileWindow as struct
 * MapTileWindow__resource_slot; module/field/field.h has another definition. */
struct MapTileWindow__resource_slot {
  s32 *origin;
  u8 unk004[0x144];
  u16 tileIds[32 * 32];
};

void WorldMap_UpdateCurrentTileBlock(void) {
  struct MapTileWindow__resource_slot *window =
      (struct MapTileWindow__resource_slot *)Ram_HeapSlots->mapWork;
  s32 x0 = 0;
  s32 y0 = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 bias;
  s32 tile;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    x0 = *p++;
    y0 = p[1];
  }
  x0 = (x0 - 0x1000000) >> 25;
  y0 = (y0 - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = window->tileIds[tile];
        tile += layer << 10;
        WorldMap_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
      }
    }
  }
}
