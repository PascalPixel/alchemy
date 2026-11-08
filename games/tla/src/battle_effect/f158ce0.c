#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

/// The Broken Seal's Camera_ApplyShake.
void Func_08158ce0(s32 randomMask, u32 shakeRange) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (randomMask - 1) & Random16();
    s32 halfRange = (s32)shakeRange / 2;
    s32 offsetY = ((shakeRange - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + 32;
    gCameraSceneParameters[3] = 120 - offsetX;
    gCameraSceneParameters[4] = 120 - offsetY;
    (*remainingFrames)--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
}
