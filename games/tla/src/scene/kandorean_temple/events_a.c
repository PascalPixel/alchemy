/// @file
/// Scene code of overlay 0x653, in ROM order, from events_a's offset up to the next object the overlay links.

#include "local.h"

void Func_02000280(void) { Func_080d2b4c(11, 27); }

u32 Func_0200028c(struct Actor *object) {
  s32 left;

  left = --*(s16 *)&object->unk66;
  return (u32)(-left | left) >> 31;
}

s32 Func_020002a0(struct Actor *object) {
  if (object->action != 0) {
    object->action--;
  } else {
    Func_080d47b4((s16)object->unk66, 0x105, 0);
    object->action = ((Random16Far() * 240) >> 16) + 240;
  }
  return 1;
}

u32 __umodsi3(u32 a, u32 b);

s32 Func_020002dc(struct Actor *object) {
  switch (object->action) {
  case 6:
    object->scaleX -= 0x4000;
    {
      s32 scale = object->scaleY;

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
  object->collisionFlags |= 8;
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}
