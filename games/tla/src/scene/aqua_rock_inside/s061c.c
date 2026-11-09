#include "local.h"
#include "version.h"

#if TLA_PATCH_1
void Func_0200061c(struct Actor *object) {
  object->motionFlags = 3;
  do {
    System_Wait(1);
  } while (object->velocityY != 0);
  Event_PlaySound(188);
  System_Wait(10);
  object->motionFlags = 0;
}
#endif
