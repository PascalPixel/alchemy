#include "types.h"
#include "actor.h"

void Func_0811bf18(struct Actor **from, struct Actor **to,
                   s32 percent) {
  struct Actor *object = *from;
  struct Actor *target = *to;
  s32 x;
  s32 z;

  x = object->x + (target->x - object->x) * percent / 100;
  z = object->z + (target->z - object->z) * percent / 100;
  *(volatile u16 *)0x04000050 = 0;
  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x40000;
  object->gravity = 0xab85;
  object->bounce = 0;
  object->actionFlags = 1;
  Actor_SetTargetPosition(object, x, 0, z);
  Actor_SetMode(object, 2);
}
