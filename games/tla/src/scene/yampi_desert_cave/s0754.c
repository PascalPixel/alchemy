#include "types.h"
#include "actor.h"

extern u16 gData_02001968[];

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
struct Actor *Func_02001948(s32 *position, s32 mode);
s32 Func_02001850(struct Actor *object, s32 *position);

void Func_02000754(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  hit = Func_02001948(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    /* FAKEMATCH(barrier): a memory barrier at the loop's head, so
     * hit->animation, its value and the table are loaded again on every pass;
     * without it, 26 of 61 instructions differ in tla-en. */
    __asm__("" : : : "memory");
    if (*((struct ObjectAnimationState *)hit->animation)->value28 ==
        gData_02001968[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02001850(object, position) > 0)
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
