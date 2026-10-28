#include "local.h"

u32 __umodsi3(u32 a, u32 b);

s32 Func_020002dc(struct Actor *object) {
  switch (object->action) {
  case 6:
    object->scaleX -= 0x4000;
    {
      s32 scale = object->scaleY;

      /* FAKEMATCH: a one-pass block loads the scale before the step is built
       * (one reordered row). */
      do {
      } while (0);
      object->scaleY = scale + 0x2000;
    }
    break;
  case 4:
    object->scaleX += 0x2000;
    object->scaleY -= 0x1000;
    break;
  case 2:
    object->scaleX += 0x1000;
    object->scaleY -= 0x800;
    break;
  case 0:
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->action = Random16Far() % 90 + 60;
    break;
  }
  object->action--;
  return 1;
}

s32 Func_0200035c(struct Actor *object) {
  object->motionFlags = 0;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  /* FAKEMATCH: the loop note ends the scheduling block after the scale stores;
   * without it the collision flags are addressed before them (one reordered
   * row). */
  do {
  } while (0);
  object->collisionFlags |= 8;
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}
