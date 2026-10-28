#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02003b84(s32 a, s32 b);

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = Import_ActorCreate(kind, x, y, z);

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
