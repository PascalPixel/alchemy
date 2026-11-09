#include "types.h"
#include "scene.h"
#include "motobj.h"
#include "actor.h"

void BattleEffect_AdvanceAngleWithRamp(struct Actor *object) {
  u32 step;

  step = object->action * 0x50;
  object->angle += step + 0x1000;
  if (step < 0x1000)
    object->action++;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
// As ☀️'s FieldEffect_UpdateDescendingObject.
void BattleEffect_UpdateDescendingObject(struct Actor *obj) {
  u32 scale;
  u32 scaleX;
  s32 step = -0x280;

  obj->scaleY += step;
  scaleX = obj->scaleX;
  scale = scaleX + step;
  obj->angle += 0x2000;
  obj->y += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    obj->animationKind = 0;
  }
}
