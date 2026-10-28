#include "local.h"

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = Import_ActorCreate(kind, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}
