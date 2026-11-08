#include "tundaria_tower.h"

void Func_02002fd0(void);

void Func_02000e78(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 6 of 59 instructions
   * differ in tla-en. */
  Func_02002fd0();
  GameFlag_SetBit(SPLIT_CONSTANT(0x200, 18));
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Func_02002fc0(43, 100, 53, 100, 3, 3);
    Call6((void (*)())Func_02002fe8, 43, 37, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 > 38)
      MapCell_SetAttributes(0, 54, 37, 0x1f00);
    else
      MapCell_SetAttributes(0, 54, 39, 0x1f00);
  }
}
