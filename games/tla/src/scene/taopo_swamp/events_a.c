#include "local.h"

void Func_02000104(s32 a, s32 b) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 8 instructions differ
   * in tla-en. */
  GameFlag_SetBit(b + SPLIT_CONSTANT(0x8d8));
}

void Func_02000118(void) {
  Func_02002840_(50, 16, 1, 2, 47, 16);
  GameFlag_SetBit(0x220);
}
