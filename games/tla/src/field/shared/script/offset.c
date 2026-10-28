#include "script.h"
#include "actor.h"

void Actor_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLocalOffsetPosition(struct Actor *object) {
  s32 offset[3];

  Actor_SetMoveTarget(object, object->x + offset[0], object->y + offset[1],
                       object->z + offset[2]);
  object->step += 3;
  return 1;
}
