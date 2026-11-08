#include "local.h"

void Func_02000428(s32 a) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Func_020028e0_(gMainRoutineWork.currentOwner, 6, 0x1000);
  Func_020029b0(310);
  GameFlag_SetBit(0x122);
  Func_02002930(a);
}

void Func_02000464(s32 a, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 44 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(id);

  if (id == 9) {
    if (object->x >> 20 == 17) {
      object->z += 0x10000;
      Func_02002764(2, 0x80000);
      GameFlag_SetBit(SPLIT_CONSTANT(0xa6d));
    }
  } else if (id == 10) {
    if (object->x >> 20 == 29) {
      object->z += 0x10000;
      Func_02002764(8, 0x80000);
      GameFlag_SetBit(SPLIT_CONSTANT(0xa6e));
    }
  }
}
