#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02000418(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_Get(id);
  if (mode == 2) {
    Actor_SetPosition(id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    System_Wait(50);
  }
}
