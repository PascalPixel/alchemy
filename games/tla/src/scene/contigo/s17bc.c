#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);

void Func_020017bc(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  object->collisionFlags = 8;
  /* FAKEMATCH: the loop note ends the scheduling block: the native stores the
   * collision flags before it copies the object for the priority flags (2
   * differ without it; a pointer to the flags 6). */
  do {
  } while (0);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->y = 0x400000;
}
