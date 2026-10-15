#include "jupiter_room_7.h"

void Func_02000c7c(void) {
  struct ObjectRuntime *object;
  s32 i;

  if (Value1((s32 (*)())GameFlag_Test, 0xa1a))
    Func_020009f8(20, 8, 12, 16, 0);
  Call1((void (*)())GameFlag_ClearBit, 0xa1a);
  Func_020005e8(1);
  WaitFrames(2);
  object = Object_GetById(20);
  for (i = 0; i < 30; i++) {
    if (object->y <= object->terrainHeight && object->velocityY == 0)
      break;
    WaitFrames(1);
  }
  object->motionFlags = 0;
}
