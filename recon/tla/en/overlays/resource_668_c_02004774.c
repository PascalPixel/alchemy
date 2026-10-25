// Near miss, differ 2 of 61: overlay 668's copy of overlay 662's
// Func_02000754 (the same listing), whose draft holds its notes.
#include "types.h"
#include "object_runtime.h"

extern u16 gData_02004ca4[];

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
struct ObjectRuntime *Func_02004c3c(s32 *position, s32 mode);
s32 Func_02004a64(struct ObjectRuntime *object, s32 *position);

void Func_02004774(struct ObjectRuntime *object) {
  s32 position[3];
  struct ObjectRuntime *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  hit = Func_02004c3c(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    __asm__("" : : : "memory");
    if (*((struct BattleAnimationState *)hit->animation)->value28 ==
        gData_02004ca4[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02004a64(object, position) > 0)
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
