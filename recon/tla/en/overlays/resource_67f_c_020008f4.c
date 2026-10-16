// Near miss, differ 4 of 59: after the twin Effect_AdvanceMotion body the
// listing loads the party state address before it builds the owner offset, and
// the object's layer byte after the mask; this draft has both pairs the other
// way round.
#include "../../../../games/tla/src/scene/izumo_ruins/izumo_ruins.h"

void Func_020008f4(struct ObjectRuntime *object) {
  s32 vx;
  s32 vz;
  s32 rate;
  struct IzumoRuinsAnimation *animation;
  struct ObjectRuntime *owner;

  vx = object->bounce;
  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the
   * Z velocity. */
  __asm__("");
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->animation;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: and would address the spin before storing the Y scale. */
  __asm__("");
  animation->angle += object->action;
  owner = Object_GetById(gPartyState.currentOwner);
  ((struct IzumoRuinsAnimation *)object->animation)->layer =
      ((struct IzumoRuinsAnimation *)owner->animation)->layer;
}
