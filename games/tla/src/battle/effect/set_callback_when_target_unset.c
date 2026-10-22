#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "frame.h"

void BattleEffect_SetCallbackWhenTargetUnset(struct ObjectRuntime *target) {
  s32 tx;
  s32 ty;

  tx = target->targetX;
  /* FAKEMATCH: ⚓️ loads x before it builds the unset marker. */
  __asm__("");
  if (tx == 0x80000000) {
    ty = target->targetY;
    if ((ty == tx) && (target->targetZ == ty)) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)target,
                                   (u32)gBattleEffect_UntargetedObjectScript);
    }
  }
}

extern u8 gData_080f0e9c[];
void Animation_ApplyChildValuesFar(void *object, s32 value);

void BattleEffect_ApplyFlickerValue(void *object, s32 value) {
  s32 child = value - 1;

  if (value == 0)
    child = gData_080f0e9c[(gFrameCount >> 1) & 7];
  Animation_ApplyChildValuesFar(object, child);
}
