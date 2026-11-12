#include "ankohl_ruins.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = FieldActor_Create(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}
