#include "../../../../../../games/tla/src/scene/shaman_village_cave/local.h"

void Func_02000148(void) {
  struct Actor *object = Actor_GetById(16);

  GameFlag_SetBit(SPLIT_CONSTANT(0x300, 2));
  object->motionFlags = 0;
  object->priorityFlags = 0;
  Func_02001060_(8, 42, 1, 1, 6, 42);
}
