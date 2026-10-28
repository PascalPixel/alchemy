#include "types.h"
#include "actor.h"
#include "macros.h"

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

s32 Func_02000660(struct Actor *object) {
  s32 radius;

  object->action += object->unk66;
  SET_POSITION(object, object->bounce, object->gravity, 0x980000);
  Vector_AddPolarOffsetFar(object->unk4c, object->action, &object->x);
  radius = object->unk4c - 0x10000;
  object->unk4c = radius;
  if (radius)
    return 1;
  return 0;
}
