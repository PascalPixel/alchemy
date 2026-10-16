// Near miss, differ 6 of 111 (two reordered rows, 15 before): The Broken
// Seal's BattlePres_SetupTransitionScene with the camera from its heap slot.
// The native stores pos[1] and pos[2] before it loads Iwram_RatioMulQ14's entry
// and stores pos[0] after it; this draft stores pos[0] between them. Later the
// native copies the camera to r1 (mov r1, r8) before the three render bound
// stores, this draft after them. Measured: the position stores reordered
// (differ 12 and 16), the bounds unchanged.
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

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = 0x01fe0000;
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
  renderBounds[2] = span;
  Iwram_TransformVector(renderBounds, (s32 *)scene);
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
