#include "local.h"

extern const u16 gData_02002478[];

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
struct ObjectRuntime *Func_02002438(s32 *position, s32 mode);
s32 Func_02002338(struct ObjectRuntime *object, s32 *position);

/// Stops the object where it stands (Object_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Func_02002438) whose sprite shape is not in the footprint table, and again
/// when its own cell test (Func_02002338) is positive.
void Func_02001300(struct ObjectRuntime *object) {
  s32 position[3];
  struct ObjectRuntime *other;
  u32 i;
  s32 angle;

  angle = object->angle >> 12;
  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  other = Func_02002438(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    /* FAKEMATCH(barrier): a memory barrier at the top of the shape search, so
     * the shape, its pointers and the table address are read again on every
     * pass as the native does; without it, 28 of 61 instructions differ in
     * tla-en. */
    asm volatile("" ::: "memory");
    if (*((struct ObjectAnimationState *)other->animation)->value28 ==
        gData_02002478[i])
      return;
    i++;
    if (i < 6)
      goto next;
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02002338(object, position) > 0)
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
