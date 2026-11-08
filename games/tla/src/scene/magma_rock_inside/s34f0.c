#include "local.h"

extern char SceneId_Ov692Room3;

s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020034f0(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 34 instructions
   * differ in tla-en. */
  GameFlag_SetBit(SPLIT_CONSTANT(0x301));
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    Func_02000694(0, 24, 15, 0);
    Func_02000694(0, 9, 16, 0);
    Actor_SetModeById(11, 4);
  } else {
    Func_02000694(0, 26, 14, 0);
  }
}
