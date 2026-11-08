#include "../../../../../../games/tla/src/scene/aqua_rock/local.h"

void Func_0200052c(struct Actor *object) {
  if ((u32)Random16Far() < 0x1000) {
    struct Actor *child;
    s32 spread = Random16Far();
    s32 x = object->x;

    x += spread * 48;
    child = Import_ActorCreate(SPLIT_CONSTANT(0x2a2), x - 0x180000, object->y, object->z);
    if (child != 0) {
      Actor_SetSingleChildFlags(child, 0);
      child->update = (void (*)(void))Func_020004e4;
      child->motionFlags = 0;
    }
  }
}
