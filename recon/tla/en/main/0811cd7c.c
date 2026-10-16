// Near miss, differ 6 of 87 (27 before): The Broken Seal's
// Camera_ConfigureScene with this game's heap slots. Left: the native builds
// the 1 it stores to the transition's active and flag words (movs r5, #1)
// right after the camera's last use, before the two IWRAM calls, and keeps it
// in r5 across them; this draft builds it just before the stores, which
// moves the distance load and one pool row. Measured: the 1 in a local set
// there, at the declaration or before the transform call (differ 42: the
// local is propagated and a register spills).
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

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  gCameraSceneParameters[4] = pos + 120;
  secondary->active = 1;
  BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
  secondary->flag = 1;
  secondary->active = 0;
}
