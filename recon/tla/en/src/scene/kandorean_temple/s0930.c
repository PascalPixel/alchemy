#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02000930(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  sprite = object->animation;
  sprite->priority = priority;
  Actor_SetSingleChildFlags(object, 0);
  Func_02005130(object, 2);
  Func_02005138(object, gData_02005494);
  Actor_SetPartAttribute(object, 9);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
