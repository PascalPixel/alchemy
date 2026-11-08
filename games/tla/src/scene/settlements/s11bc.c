#include "local.h"

void Func_020011bc(void) {
  Func_02002198();
  Func_02000f2c();
}

void Func_020011c8(void) {
  Func_02002198();
  Func_02000fd0();
}

void Func_020011d4(void) {
  Func_02002198();
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags += 254;
  Func_02000f2c();
  Func_02000fd0();
}

void Func_02001200(void) {
  Func_02002030(11, 73, 10, 10, 64, 0);
  Func_02002030(42, 73, 10, 10, 64, 10);
}

void Func_0200122c(void) {
  Func_02002030(64, 0, 10, 10, 11, 73);
  Func_02002030(64, 10, 10, 10, 42, 73);
}

void Func_0200125c(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 44 instructions
   * differ in tla-en. */
  if (!GameFlag_Test(SPLIT_CONSTANT(0x122, 255)) && !Flag_Test(0x220)) {
    Func_02002030(11, 73, 10, 10, 64, 50);
    Func_02002030(42, 73, 10, 10, 64, 60);
    CopyCells2(42, 11, 10, 4, 11, 75);
    GameFlag_SetBit(0x220);
  }
}

void Func_020012c0(void) {
  if (Flag_Test(0x220)) {
    Func_02002030(64, 50, 10, 10, 11, 73);
    Func_02002030(64, 60, 10, 10, 42, 73);
    GameFlag_ClearBit(0x220);
  }
}
