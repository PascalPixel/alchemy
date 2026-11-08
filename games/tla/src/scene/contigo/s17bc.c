#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);

void Func_020017bc(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  object->collisionFlags = 8;
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->y = 0x400000;
}
