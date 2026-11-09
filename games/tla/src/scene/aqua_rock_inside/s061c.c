#include "local.h"
#include "version.h"

#if TLA_PATCH_1
void Func_0200061c(struct Actor *object) {
  object->motionFlags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(188);
  wait(10);
  object->motionFlags = 0;
}
#endif
