#include "mars_lighthouse_room_m.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02001ed4(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003118(object, 2);
  Func_02003120(object, gData_020032d4);
  Actor_SetSpriteColor(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
