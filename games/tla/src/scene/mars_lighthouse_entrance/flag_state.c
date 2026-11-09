#include "ride.h"
#include "event_inline.h"

void Func_02002c2c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}
