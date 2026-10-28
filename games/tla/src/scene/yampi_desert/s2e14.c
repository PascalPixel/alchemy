#include "scene.h"

void Func_02002e14(void) {
  struct FieldActor *object;
  s32 x;
  s32 z;

  if (GameFlag_Test(0x88d)) {
    object = Actor_GetById(gSequenceParentWork.currentOwner);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 8) {
      if (z == 31 && !(gInput & 0x80))
        return;
      if (z == 33 && !(gInput & 0x40))
        return;
    } else {
      if (z != 32)
        return;
      if (x == 7 && !(gInput & 0x10))
        return;
      if (x == 9 && !(gInput & 0x20))
        return;
    }
    Func_02002d6c(object, 136, 516, 5);
  }
}
