#include "local.h"

void Func_02000cfc(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 10 instructions
   * differ in tla-en. */
  s32 b = 8;
  s32 c = 9;

  /* FAKEMATCH: the native loads the two coordinates before the first argument;
   * a direct call and a forwarding inline load it first. */
  __asm__ volatile("" : "+r"(b), "+r"(c));
  Func_02002178(1, b, c);
  GameFlag_SetBit(SPLIT_CONSTANT(0x104, 255));
}
