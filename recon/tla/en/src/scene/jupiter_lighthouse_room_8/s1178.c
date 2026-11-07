#include "types.h"
#include "actor.h"

struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};

extern const u16 gData_020022d0[];

s32 Func_020021b8(struct Actor *object, struct IsletsVector *position);
struct Actor *Func_020022a8(struct IsletsVector *position, s32 mode);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle,
                              struct IsletsVector *position);

void Func_02001178(struct Actor *object) {
  struct IsletsVector position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, &position);
  hit = Func_020022a8(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    /* FAKEMATCH(barrier): a memory barrier at the loop's head, so
     * hit->animation, its value and the table are loaded again on every pass;
     * without it, 26 of 61 instructions differ in tla-en. */
    __asm__("" : : : "memory");
    if (*((struct ObjectAnimationState *)hit->animation)->value28 ==
        gData_020022d0[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (Func_020021b8(object, &position) > 0)
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
