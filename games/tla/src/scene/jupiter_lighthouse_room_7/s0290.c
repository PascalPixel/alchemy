#include "jupiter_lighthouse_room_7.h"

s32 Func_02000290(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 20 of 41 instructions
   * differ in tla-en. */
  s32 count = 0;

  if (GameFlag_Test(0xa10) || GameFlag_Test(SPLIT_CONSTANT(0xa00, 18)) ||
      GameFlag_Test(SPLIT_CONSTANT(0xa00, 20)))
    count = 1;
  if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 17)) ||
      GameFlag_Test(SPLIT_CONSTANT(0xa00, 19)) ||
      GameFlag_Test(SPLIT_CONSTANT(0xa00, 21)))
    count++;
  return count;
}
