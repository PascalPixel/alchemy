#include "local.h"

void Func_02000468(struct ObjectRuntime *object) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = object->angle;
  do {
    sprite = object->animation;
  } while (0);
  sprite->rotation = angle + 0x4000;
}
