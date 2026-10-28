#include "kibombo.h"

void Func_020000a0(struct KibomboObject *object) {
  if (gFrameCount & 2)
    Actor_SetPartAttribute(object, 10);
  else
    Actor_SetPartAttribute(object, 0);
}

s32 Func_020000c4(struct KibomboObject *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    object->timer = ((Random16Far() * 10) >> 16) + 10;
    object->facing = Random16Far();
  }
  return 1;
}
