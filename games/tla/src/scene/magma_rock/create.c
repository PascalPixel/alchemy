#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = ((struct Actor *)Import_ActorCreate(kind, x, y, z));

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
