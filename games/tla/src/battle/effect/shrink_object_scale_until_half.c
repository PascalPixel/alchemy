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
#include "object_runtime.h"

#define Object_SetPositionAndResetMotionFar                                    \
  ((s32 (*)(void *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

s32 FieldEffect_RunWhirlwind();

void BattleEffect_ShrinkObjectScaleUntilHalf(struct ObjectRuntime *object) {
  s32 scale;

  scale = object->scaleX - 0x80;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < 0x8000) {
    Object_SetPositionAndResetMotionFar(object, 0, 0, 0);
    object->update = 0;
  }
}

void FieldEffect_RunWhirlwindOnTarget(void) { FieldEffect_RunWhirlwind(); }
