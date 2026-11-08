#include "local.h"

void Func_02002a14(struct Actor *source) {
  struct Actor *object = Create(0x2a0, source->x, source->y, source->z);

  object->scaleY = 0x6666;
  object->scaleX = 0x6666;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02004ca0(object, gData_02005534);
}
