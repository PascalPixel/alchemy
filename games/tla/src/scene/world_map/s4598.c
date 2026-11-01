#include "types.h"
#include "actor.h"

s32 Math_Sine(s32 angle);
void Func_02004b38(struct Actor *object);

void Func_02004598(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->action;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02004b38(object);
    return;
  }
  scale = Math_Sine(step << 10);
  object->scaleX = scale;
  object->scaleY = scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z + (rise - scale) * 5 + 0x80000;
}

void Func_020045e4(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->action;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02004b38(object);
    return;
  }
  scale = Math_Sine(step << 10);
  object->scaleX = scale;
  object->scaleY = -scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z - (rise - scale) * 5 + 0x100000;
}
