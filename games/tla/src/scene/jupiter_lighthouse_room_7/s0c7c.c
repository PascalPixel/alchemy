#include "jupiter_lighthouse_room_7.h"

void Func_02000c7c(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 48 instructions
   * differ in tla-en. */
  struct Actor *object;
  s32 i;

  if (Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa1a)))
    Func_020009f8(20, 8, 12, 16, 0);
  GameFlag_ClearBit(SPLIT_CONSTANT(0xa1a));
  Func_020005e8(1);
  wait(2);
  object = Actor_GetById(20);
  for (i = 0; i < 30; i++) {
    if (object->y <= object->terrainHeight && object->velocityY == 0)
      break;
    wait(1);
  }
  object->motionFlags = 0;
}
