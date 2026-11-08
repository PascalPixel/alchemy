#include "airs_rock_inside.h"

void Func_020007b4(void);

void Func_02004884(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 21 instructions
   * differ in tla-en. */
  Func_02005e14();
  Func_02005f2c(0);
  if (Actor_GetById(8)->x >> 19 == 65)
    GameFlag_SetBit(SPLIT_CONSTANT(0x204, 255));
  else
    GameFlag_ClearBit(SPLIT_CONSTANT(0x204, 255));
  Func_02005e1c();
}

void Func_020048bc(void) { Func_020007b4(); }
