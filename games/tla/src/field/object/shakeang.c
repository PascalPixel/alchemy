#include "types.h"
#include "actor.h"
#include "system.h"

void Actor_ShakeAngle(u32 objectId) {
  struct Actor *object;
  s32 angle;
  s32 i;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  angle = object->angle;
  for (i = 15; i >= 0; i--) {
    u32 a = Random16();
    u32 b = Random16();
    u32 cur;

    cur = object->angle;
    b >>= 4;
    a >>= 4;
    object->angle = cur + (a - b);
    wait(1);
  }
  object->angle = angle;
}
