#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_020030b8(struct KibomboObject *object) {
  if (object->field64 != 0) {
    object->field64--;
  } else {
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    if (object->field66 != 0) {
      object->scaleX = 0x18000;
      object->field66 = 0;
    } else {
      object->scaleX = 0xfffe8000;
      object->field66 = 1;
    }
  }
}

void Func_02003104(s32 x, s32 y, s32 z) {
  struct KibomboObject *object = Func_02005a34(0x106, x, y, z);
  struct FieldSprite *sprite;

  if (object != 0) {
    object->motionFlags = 0;
    object->priorityFlags &= ~1;
    object->scaleY = 0x20000;
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    object->field66 = 0;
    object->update = Func_020030b8;
    sprite = object->sprite;
    sprite->priority = 1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02005a24(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}
