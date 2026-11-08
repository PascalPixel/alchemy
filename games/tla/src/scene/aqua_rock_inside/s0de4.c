#include "local.h"

void Func_02000de4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 21 instructions
   * differ in tla-en. */
  s32 i;

  Func_02003628();
  Func_02003688(44, 0, 34, 2);
  for (i = 4; i >= 0; i--) {
    Func_020036b8(33, 55);
    wait(15);
  }
  GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 103));
}
