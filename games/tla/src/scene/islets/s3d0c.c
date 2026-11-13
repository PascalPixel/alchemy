/// @file
/// Scene code of overlay 0x674, in ROM order.

#include "../shared/islets.h"
#include "event_inline.h"

void Func_02003d0c(struct Actor *object) {
  struct Vector3 position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, &position);
  hit = Func_02003fdc(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gData_02004070[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (Func_02003e2c(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
