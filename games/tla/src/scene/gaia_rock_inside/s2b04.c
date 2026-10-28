#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);

void Func_02002b04(u32 id, s8 mode) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x1000;
  object->speedLimit = 0x10000;
  object->y = 0x100000;
  object->action = object->x >> 16;
  {
    s32 z = object->z >> 16;

    /* FAKEMATCH(barrier): an empty asm on z's whole part, so the mode's
     * narrowing starts before it loads; as object->unk66 = object->z >> 16, 4
     * of 40 instructions differ in tla-en. */
    asm("" : "+l"(z));
    object->unk66 = z;
  }
  Actor_SetModeById(id, mode);
}
