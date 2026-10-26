#include "islets.h"

void Func_02003d0c(struct ObjectRuntime *object) {
  struct IsletsVector position;
  struct ObjectRuntime *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, &position);
  hit = Func_02003fdc(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    /* FAKEMATCH(barrier): a memory barrier at the loop's head, so
     * hit->animation, its value and the table are loaded again on every pass;
     * without it, 26 of 61 instructions differ in tla-en. */
    __asm__("" : : : "memory");
    if (*((struct BattleAnimationState *)hit->animation)->value28 ==
        gData_02004070[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (Func_02003e2c(object, &position) > 0)
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
