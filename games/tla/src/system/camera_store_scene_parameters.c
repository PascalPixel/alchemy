#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
s32 Trig_Sin(s32);

void Projection_SetFieldOfView(s32 value, s32 param1, s32 param2) {
  s32 half;
  s32 first;
  s32 result;

  half = value / 2;
  first = Trig_Sin(half);
  result = Iwram_RatioMulQ14(first, Trig_Sin(half + 0x4000) * 0x50);
  gCameraSceneParameters[1] = param1;
  gCameraSceneParameters[0] = result;
  gCameraSceneParameters[2] = param2;
}

void Projection_Set(u32 value0, u32 value1, u32 value2) {
  u32 *work = gCameraSceneParameters;
  work[0] = value0;
  work[1] = value1;
  work[2] = value2;
}
