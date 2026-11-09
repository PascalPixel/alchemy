#include "local.h"

void Func_020007a8(void) {
  struct Actor *object;
  struct FieldSpriteFlags *sprite;

  Actor_ShowItemIconById(10, 0x1c8);
  object = Actor_Get(20);
  sprite = object->animation;
  sprite->flag0 = 1;
  object->motionFlags = 0;
  object->y = 0x180000;
}
