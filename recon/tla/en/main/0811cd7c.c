// Under -mthumb-slow-loads (26 October 2026): differ 4 of 87 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it; none reaches 0.
// Near miss, differ 4 of 87 (27, then 6 before): The Broken Seal's
// Camera_ConfigureScene with this game's heap slots. The 1 stored to the
// transition's active and flag words is a temporary behind a barrier after
// the transform call, which keeps it in r5 across the calls (6 to 4). Left:
// the native builds it before that call, right after the camera's last use
// (movs r5, #1 after ldr r3, [r5, #32]), and copies the camera into r1
// before it loads the distance. Tried: the barrier before the call (50: the
// camera can no longer pass in r5), the plain 1 in a local set there, at
// the declaration or before the call (42: propagated, a register spills).
#include "types.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "fixed_point_position.h"

extern s32 gCameraSceneParameters[];
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);
void Projection_Set(s32 span, u32 depth, s32 range);

void Camera_ConfigureScene(s32 pos) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *secondary =
      Ram_HeapSlots->transitionWork;
  struct FixedPointPosition local;
  u32 result;
  s32 one;

  state->pos[1] = 160 << 11;
  state->pos[0] = 0;
  state->pos[2] = 0;
  secondary->targetYaw = 128 << 7;
  state->yaw = 128 << 7;
  state->pitch = 244 << 8;
  state->followPos = 0;
  state->distance = 0x02ee0000;
  state->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&state->pos[0]);
  SceneTransform_ApplyYaw((s16)state->yaw);
  SceneTransform_ApplyPitch((s16)state->pitch);

  local.x = 0;
  local.y = 0;
  local.z = state->distance;
  Iwram_TransformVector(&local, (s32 *)state);
  one = 1;
  __asm__ volatile("" : "+l"(one));

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  gCameraSceneParameters[4] = pos + 120;
  secondary->active = one;
  BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
  secondary->flag = one;
  secondary->active = 0;
}
