#include "ride.h"
#include "event_inline.h"
#include "field.h"

void Func_02001234(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}
