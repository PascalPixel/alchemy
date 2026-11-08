#include "sea_of_time.h"

void Func_02001068(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 23 instructions
   * differ in tla-en. */
  Func_02001fac();
  Func_02002044(0);
  if (!SeaOfTime_FlagCheck(SPLIT_CONSTANT(0x8dd))) {
    Func_02002004(4);
    GameFlag_SetBit(SPLIT_CONSTANT(0x8dd));
  } else {
    Func_02002004(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}
