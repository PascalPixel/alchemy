#include "ride.h"

void Func_02001234(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}
