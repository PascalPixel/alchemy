#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "fixed_point_position.h"
#include "battle_display.h"
#include "macros.h"

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Projection_Set(u32 value0, u32 value1, u32 value2);

s32 Runtime_BlankDisplayAndRun(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  Audio_PlayCue(2);
  LuckyDice_Run();
  return 0;
}

// Resets the scene camera and its projection, as The Broken Seal's.
void Camera_ResetSceneDefaults(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct FixedPointPosition local;
  /* FAKEMATCH: the result pointer pinned to r1 is copied from the camera where
   * it is set, ahead of the distance's one-pass block; plain, or with the block
   * alone, the copy waits for the call and the distance load comes first
   * (differ 2). */
  register s32 *result asm("r1");
  u32 ratio;
  u32 param;

  state->pitch = 152 << 8;
  state->distance = 255 << 17;
  SET_VECTOR(state->pos, 0, 0, 0);
  state->yaw = 0;
  state->followPos = 0;
  // The projection's centre.
  gCameraSceneParameters[3] = 0;
  gCameraSceneParameters[4] = 0;
  state->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&state->pos[0]);
  SceneTransform_ApplyYaw((s16)state->yaw);
  SceneTransform_ApplyPitch((s16)state->pitch);

  local.x = 0;
  local.y = 0;
  result = (s32 *)state;
  /* FAKEMATCH: a one-pass block keeps the distance load behind the pinned copy;
   * without it the load is scheduled first (differ 2). */
  do {
    local.z = state->distance;
  } while (0);
  Iwram_TransformVector(&local, result);

  param = 250;
  param <<= 16;
  ratio = Iwram_RatioMulQ14(param, 192 << 8);

  param = 250;
  param <<= 16;
  Projection_Set(param, ratio, 0x7fff0000);
}
