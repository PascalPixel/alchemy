#include "local.h"
#include "edition.h"

#if (LANGUAGE != LANGUAGE_JAPANESE)
void Func_0200061c(struct Actor *object) {
  object->motionFlags = 3;
  do {
    WaitFrames(1);
  } while (object->velocityY != 0);
  Func_02003ef4(188);
  WaitFrames(10);
  object->motionFlags = 0;
}
#endif
