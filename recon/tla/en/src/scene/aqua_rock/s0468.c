#include "../../../../../../games/tla/src/scene/aqua_rock/local.h"

void Func_02000468(struct Actor *object) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = object->angle;
  sprite = object->animation;
  sprite->rotation = angle + 0x4000;
}
