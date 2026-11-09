#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02001064(struct Actor *object) {
  struct Actor *anchor = object->watchedObject;
  s32 frame;
  s32 scale;

  object->action++;
  frame = object->action;
  if (frame > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(frame << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = anchor->x;
  object->y += 0x10000;
  object->z = anchor->z;
  return 1;
}
