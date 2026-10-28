#include "anemos_inner_sanctum_entrance.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

static __inline__ struct Actor *Actor_Create(s32 type, s32 x, s32 y,
                                                      s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Import_ActorCreate(type, x, y, z);
}

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = Actor_Create(type, x, y, z);

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
