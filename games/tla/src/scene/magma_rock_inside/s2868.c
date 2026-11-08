#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002868(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 27 instructions
   * differ in tla-en. */
  if (!GameFlag_Test(SPLIT_CONSTANT(0x305))) {
    Actor_SetPartAttribute(Actor_GetById(11), 15);
    Func_02004da0(11, PIXELS(136), PIXELS(360));
    Actor_GetById(11)->scaleX = Actor_GetById(11)->scaleY = 0x1000;
  }
}
