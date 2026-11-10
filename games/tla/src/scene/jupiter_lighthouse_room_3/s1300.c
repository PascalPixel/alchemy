#include "local.h"

extern const u16 gData_02002478[];

struct Actor *Func_02002438(s32 *position, s32 mode);
s32 Func_02002338(struct Actor *object, s32 *position);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Func_02002438) whose sprite shape is not in the footprint table, and again
/// when its own cell test (Func_02002338) is positive.
void Func_02001300(struct Actor *object) {
  s32 position[3];
  struct Actor *other;
  u32 i;
  s32 angle;

  angle = object->angle >> 12;
  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  other = Func_02002438(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)other->animation)->value28 ==
        gData_02002478[i])
      return;
    i++;
    if (i < 6)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02002338(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
