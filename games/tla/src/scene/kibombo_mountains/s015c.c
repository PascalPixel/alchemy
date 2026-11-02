#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);

void Func_0200015c(s32 id) {
  s32 n = 60;
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 3;
  while (n != 0) {
    wait(1);
    if (object->velocityY == 0)
      break;
    n--;
  }
  wait(10);
  object->motionFlags = 0;
}
