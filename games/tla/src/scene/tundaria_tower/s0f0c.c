#include "tundaria_tower.h"

void Func_02000f0c(s32 step) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 73 instructions
   * differ in tla-en. */
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    Func_02002fc0(18, 97, 6, 97, 3, 4);
    Func_02002fc0(82, 34, 70, 34, 3, 2);
    Call6((void (*)())Func_02002fe8, 7, 36, 1, 1, 7, 35);
    GameFlag_SetBit(SPLIT_CONSTANT(0x900, 75));
    break;
  case 2:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 1, SPLIT_CONSTANT(0xcc00, 204));
    break;
  case 3:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}
