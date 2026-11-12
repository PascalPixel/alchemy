#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "heap_release.h"
#include "resource.h"
#include "graphics.h"

/// One word of the table at Ram_MapBlocks, 32 words to a row.
struct Data02020000 {
  u32 unk0 : 24;
  /// Passed to Func_0802d088; region / 10 picks the set. The Broken Seal's
  /// WorldMapCell keeps the encounter region at these bits.
  u32 region : 6;
  u32 unk1e : 2;
};

extern const u32 gData_0802ed34[4][9];
extern u8 gData_02028000[];
extern u8 gWorldMapWork[];

void Func_0802d088(s32 region, s32 mode);
void Func_080c8970(void *palette);
void Func_0802cc9c(const u16 *script);

void Func_0802be4c(s32 x, s32 z) {
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
  Func_080c8970(buffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]),
                        Ram_MapCollision + 0x1000);
  Func_0802cc9c((const u16 *)(Ram_MapCollision + 0x1000));
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_MapCollision);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer);
  Dma_Set(Ram_BgTileBuffer, BG_CHAR_BLOCK(2), DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_BgTileBuffer + 0x2000);
  Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]),
                        Ram_BgTileBuffer + 0x4000);
  Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_COPY_WORDS(2048),
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[6]),
                        Ram_BgTileBuffer + 0x6000);
  Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          DMA_COPY_WORDS(2048), REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[7]), gData_02028000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
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

#include "frame.h"

struct MapScanline {
  u8 unk00[20];
};

struct MapAffinePair {
  u32 words[8];
};

struct MapAffineWork {
  struct MapScanline scanlines[160];
  struct MapAffinePair pages[2][160];
};

struct PerspectiveWindow {
  u8 unk000[0x104];
  u16 windowTop;
  u16 windowBottom;
  u16 shownTop;
  u16 shownBottom;
  u16 windowPending;
};

void Func_0802c088(void) {
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

extern u8 gMapBlocks[];

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
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 column;

  cellBytes = gMapBlocks + ((((s32)tileRow / 2) & 31) << 7);
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
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 row;
  /* FAKEMATCH(pin): the BG map address held in r5, loaded between the cell
   * offset's shift and add; as a plain local, 4 of 32 instructions differ in
   * tla-en. */
  register u32 screenBase asm("r5") = 0x06004000;

  cellBytes = gMapBlocks + ((((s32)tileColumn / 2) & 31) << 2);
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

#include "iwram_call.h"
#include "bg_scroll.h"

extern u32 gData_03001144;
extern u32 Data_03001244;

u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802c58c(void);
void Func_0802dd70(s32 ratio, s32 *position, void *affineWork);

struct WorldView {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 unk010[0xd4];
  s32 viewX;
  s32 viewY;
  u8 unk0ec[0x30];
  u16 pitch;
  u16 yaw;
  u8 unk120[0x83c];
  s32 farPlane;
  s32 distance;
  u8 unk964[0xd];
  s8 flat;
  s8 paused;
  u8 unk973[3];
  s8 fixedEye;
};

struct EyeOffset {
  s32 x, y, z;
};

void Func_0802c240(void) {
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
        Map_RenderPaletteMappedColumn(tileColumn + 16);
      else
        Map_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((view->viewY ^ y) & 0x100000) {
      if (view->viewY < y)
        Map_RenderPaletteMappedRow(tileRow + 12);
      else
        Map_RenderPaletteMappedRow(tileRow - 18);
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
  Projection_Set(distance, height / 2, height * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  if (view->fixedEye == 0) {
    eyeOffset.x = 0;
    eyeOffset.y = 0;
    eyeOffset.z = height + 0x10000;
    Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  }
  Render_ResetTransformState();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  if (gData_03001144 != view->pitch) {
    s32 cosine = Trig_Cos(view->pitch);
    s32 sine = Trig_Sin(view->pitch);

    Func_0802dd70(Iwram_RatioMulQ14(cosine, sine), cameraPosition, affineWork);
    Data_03001244 = 0;
    gData_03001144 = view->pitch;
  }
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, affineWork,
      affinePages + (gFrameCount & 1) * 0x1400);
}

struct MapTileWindow {
  s32 *origin;
  u8 unk004[0x144];
  u16 tileIds[32 * 32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_UpdateCurrentTileBlock(void) {
  struct MapTileWindow *window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
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
        Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
      }
    }
  }
}
