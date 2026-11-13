/// @file
/// The world map's window cells, with a routine not yet understood.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"

extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_Data199;
extern u8 gData_0202e000[];
extern u32 gCameraSceneParameters[];
extern const u8 Data_0802146c[];
extern u8 Data_0802146cSize[];
extern u32 gData_03001144;
extern u32 Data_03001244;

void Fade_FromBlack(s32 vblanks);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
/* VIEW(f02b998): routine Render_ResetTransformState as u32 Render_ResetTransformState(void); graphics.h has void Render_ResetTransformState(void); tried as the definition's void Render_ResetTransformState(void), which the object already declares, Func_0802b998 moves: 1 of 372 instructions in tla-ja. */
extern u32 Render_ResetTransformState__f02b998(void) __asm__("Render_ResetTransformState"); // Returns void; see the tag.
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802dd70(s32 ratio, s32 *position, void *affineWork);
void Func_0802c240(void);
void Func_0802c088(void);

struct WorldView {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *unk010;
  u16 unk014;
  u8 unk016;
  u8 unk017[0xcd];
  s32 viewX;
  s32 viewY;
  u8 unk0ec[0x18];
  u16 windowTop;
  u16 windowBottom;
  u8 unk108[0x14];
  u16 pitch;
  u16 yaw;
  u8 unk120[0x83c];
  s32 farPlane;
  s32 distance;
  u8 unk964[4];
  s32 unk968;
  u16 unk96c;
  u8 unk96e[3];
  s8 flat;
  s8 paused;
  u8 unk973[3];
  s8 fixedEye;
};

struct EyeOffset {
  s32 x, y, z;
};

void Func_0802b998(void) {
  /* FAKEMATCH(signature): Render_ResetTransformState declared returning u32;
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
  Fade_FromBlack(0);
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
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  view->unk014 = 0x1f00;
  view->unk016 = 0x80;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data199),
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
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState__f02b998();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState__f02b998();
  Transform_SetLookAt((s32 *)camera, cameraPosition);
  RamCode_LoadSlot(104, Data_0802146c, Data_0802146cSize);
  Func_0802dd70(Iwram_RatioMulQ14(Trig_Cos(view->pitch), Trig_Sin(view->pitch)),
                cameraPosition, affineWork);
  Data_03001244 = 0;
  gData_03001144 = view->pitch;
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
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
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
  Task_Add(Func_0802c240, 0x485);
  Task_Add(Func_0802c088, 0xc80);
  for (tileIndex = 0; tileIndex <= 0x3ff; tileIndex++)
    ((u16 *)(perspective + 0x148))[tileIndex] = tileIndex;
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
  s32 *position;
  u8 unk004[0x144];
  u16 tiles[32][32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
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
    Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
  }
}

/// Map_SetWindowCellTile without its view test: puts world map tile
/// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it in
/// both layers.
void Func_0802bdd0(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  /* FAKEMATCH(dead): the view's origin read as Map_SetWindowCellTile reads
   * it and never used, which emits nothing but steers register allocation;
   * without it, 57 of 59 instructions differ in tla-en. */
  position = window->position;
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
  Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}
