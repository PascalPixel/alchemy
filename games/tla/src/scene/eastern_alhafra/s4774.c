#include "types.h"
#include "actor.h"
#include "event_inline.h"

extern u16 gData_02004ca4[];

struct Actor *Func_02004c3c(s32 *position, s32 mode);
s32 Func_02004a64(struct Actor *object, s32 *position);

void Func_02004774(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  hit = Func_02004c3c(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->animation)->value28 ==
        gData_02004ca4[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02004a64(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
