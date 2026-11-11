#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

/// Shakes the battle view while the effect work's shakeFrames lasts, moving
/// BG1 and gCameraSceneParameters[3] and [4] by a random offset; once the
/// shake runs out, BG1 returns to its saved position and both parameters to
/// 120. It takes two random numbers each shaking frame, x first.
///
/// Called by nearly every effect. The x offset is centred on
/// half of rangeY too, so with rangeX smaller the x offset is never positive.
///
/// rangeX: the horizontal range, a power of two.
/// rangeY: the vertical range, a power of two; both offsets are centred on
///         half of it.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
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
