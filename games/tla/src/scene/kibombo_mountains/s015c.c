#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_0200015c(s32 id) {
  s32 n = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (n != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    n--;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
