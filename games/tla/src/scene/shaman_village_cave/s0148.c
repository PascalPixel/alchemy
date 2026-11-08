#include "local.h"

void Func_02000148(void) {
  struct Actor *object = Actor_GetById(16);

  GameFlag_SetBit(0x302);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  Func_02001060_(8, 42, 1, 1, 6, 42);
}
