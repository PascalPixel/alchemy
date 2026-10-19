// Near miss, differ 16 of 295 (2026-10-18): The Broken Seal's
// WorldMap_UpdateView with this game's flat view (byte 0x971 scrolls BG1 by
// the shake instead of projecting), its paused view (0x972) and the eye
// offset skipped while 0x976 is set. Left: instruction order only, in the
// prologue's work-slot loads and constants, the view x reload and the flat
// test's offset. Six minutes of permute: no better.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "bg_scroll.h"
#include "frame.h"

extern u32 gCameraSceneParameters[];
extern u32 gData_03001144;
extern u32 Data_03001244;

u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Projection_Set(s32 distance, s32 nearPlane, s32 farPlane);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Func_0802c58c(void);
void Func_0802dd70(s32 ratio, s32 *position, void *frameWork);
void Map_RenderPaletteMappedRow(u32 tileRow);
void Map_RenderPaletteMappedColumn(u32 tileColumn);

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
  u8 *frameWork = Ram_HeapSlots->unk1c[0];
  struct WorldView *view = (struct WorldView *)Ram_HeapSlots->mapWork;
  s32 *cameraPosition = (s32 *)(camera + 12);
  s32 *followedPosition = view->origin;
  u8 *affinePages = frameWork + 0xc80;
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

    Func_0802dd70(Iwram_RatioMulQ14(cosine, sine), cameraPosition, frameWork);
    Data_03001244 = 0;
    gData_03001144 = view->pitch;
  }
  ((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->unk64[1])(
      camera, cameraPosition, frameWork,
      affinePages + (gFrameCount & 1) * 0x1400);
}
