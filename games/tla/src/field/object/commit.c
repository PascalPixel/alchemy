#include "actor.h"

void Actor_WaitForMoveById(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Actor_CommitPosition(object);
    Actor_SetMode(object, 1);
  }
}
