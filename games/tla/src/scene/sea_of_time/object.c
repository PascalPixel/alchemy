#include "sea_of_time.h"

struct SeaOfTimeObject *Func_020000d0(s32 x, s32 y, s32 z, s32 type) {
  struct SeaOfTimeObject *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 15);
    object->priorityFlags |= 34;
    return object;
  }
  return NULL;
}
