#include "actor.h"

void Actor_SetModeById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Actor_SetMode(object, action);
}
