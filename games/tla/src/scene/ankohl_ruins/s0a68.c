#include "ankohl_ruins.h"

void Func_02000a68(s32 unused, s32 objectId) {
  register s32 id asm("r5") = objectId;
  register struct ObjectRuntime *object asm("r6") = Object_GetById(id);
  s16 frames = 60;

  if (object->z >> 20 == 31) {
    object->motionFlags = 3;
    Func_02002e34(object, 3);
    Func_02002eec(204);
    while (frames != 0) {
      WaitTicks(1);
      if (object->velocityY == 0)
        break;
      frames--;
    }
    Func_02002e6c(id, 0, 0);
    if (id == 10)
      GameFlag_SetBit(0x9d2);
    if (id == 11)
      GameFlag_SetBit(0x9d3);
    if (id == 12)
      GameFlag_SetBit(0x9d4);
    if (id == 13)
      GameFlag_SetBit(0x9d5);
    if (id == 14)
      GameFlag_SetBit(0x9d6);
    if (id == 15)
      GameFlag_SetBit(0x9d7);
    if (id == 16)
      GameFlag_SetBit(0x9d8);
  }
}
