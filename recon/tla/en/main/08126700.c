// Under -mthumb-slow-loads (26 October 2026): differ 2 of 112 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it, five minutes of permute; none reaches 0. A barrier
// before render_bounds[2] puts the camera's copy after the zero stores, as
// the native, but then moves the blend entry's copy (mov fp, r2) in the
// opening (4).
// Near miss, differ 6 of 112 (15 before, with the camera from gCameraWork;
// 2026-10-19): the camera now comes from its heap slot, as in its sibling
// 08126804. Left, as there: the native stores pos[1] and pos[2] before it
// loads Iwram_RatioMulQ14's entry and pos[0] after it, and copies the camera
// to r2 (mov r2, r8) before the two zero render bounds, this draft after
// them. Tried: the render bounds written last to first (12).
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "battle_display.h"
#include "ram_buffer.h"

extern u8 gProjection[];

void Projection_Set(s32, u32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Transform_SetLookAt();
s32 Render_ProjectPoint(const void *point, s32 *screen);

/* FAKEMATCH: mode intentionally remains uninitialized to preserve the match. */

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y) {
  s32 span = 0x01fe0000;
  s32 mode;
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 *hud = (s32 *)gProjection;
  s32 scale = __divsi3(mode << 16, 100);
  s32 render_bounds[3];
  s32 measured_bounds[3];
  s32 source_bounds[3];
  s32 (*blend)(s32, s32);
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  source_bounds[0] = 0;
  alpha = 0xc000;
  source_bounds[1] = 0;
  source_bounds[2] = 0;

  blend = Iwram_RatioMulQ14;
  result = blend(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  render_bounds[0] = 0;
  render_bounds[1] = 0;
  render_bounds[2] = scene->distance;
  Iwram_TransformVector(render_bounds, (s32 *)scene);
  hud[3] = 120;
  hud[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene, pos);
  Render_ProjectPoint(source_bounds, measured_bounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measured_bounds[0]) << 8,
                        (120 - measured_bounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = blend(width, alpha);
  Projection_Set(width, result, half * 4);
}
