#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_020039a8(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->action;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
  return 1;
}
