// Matches: differ 0 of 112 (tla-en, 2026-10-30), with no device.
// The Broken Seal's BattleCamera_SetViewAtDepth (battle/battle/background.c)
// with the camera from its heap slot, as in its adopted sibling
// BattleCamera_SetView (setup_transition_scene.c): the same plain C,
// Iwram_RatioMulQ14 called directly, the division written as / 100, the eye
// passed as scene->eye and the projection centre stored to
// gCameraSceneParameters, the listing's own pool symbol. The previous draft
// (direct __divsi3, a local entry pointer, gProjection) already measured
// differ 0 on 2026-10-30; its header's 2 of 112 was stale. The zoom is
// read uninitialized (lsls r0, r4, #16) as in The Broken Seal: original
// behaviour kept with BUG and UBFIX, not a device.
#include "types.h"
#include "battle_display.h"
#include "graphics.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "transform.h"

extern u32 gCameraSceneParameters[];
void Projection_Set(s32 span, u32 depth, s32 range);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
s32 Render_ProjectPoint(const void *point, s32 *screen);

#ifdef UBFIX
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  s32 span = 0x01fe0000;
  // BUG: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = scene->distance;
  Iwram_TransformVector(renderBounds, scene->eye);
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}
