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
  Object_GetById(gAdventure.currentOwner)->priorityFlags += 254;
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
  if (!Flag_Test(0x221) && !Flag_Test(0x220)) {
    Func_02002030(11, 73, 10, 10, 64, 50);
    Func_02002030(42, 73, 10, 10, 64, 60);
    CopyCells2(42, 11, 10, 4, 11, 75);
    Flag_Set(0x220);
  }
}

void Func_020012c0(void) {
  if (Flag_Test(0x220)) {
    Func_02002030(64, 50, 10, 10, 11, 73);
    Func_02002030(64, 60, 10, 10, 42, 73);
    Flag_Clear(0x220);
  }
}
