#include "kibombo.h"

void Func_020050ac(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Func_02005c04(0);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 3);
  Func_02005c54();
  Func_02005c5c(0, 0x9e9, 12, 13);
  Actor_GetById(22)->collisionFlags |= 0x80;
  Actor_GetById(23)->collisionFlags |= 0x80;
}

void Func_02005114(void) {
  Func_02005b0c(11, 0x580000, 0x13a0000);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->scaleY = 0x8000;
  Func_020050ac();
  GameFlag_ClearBit(0x86b);
  Func_02005b8c(25);
  Actor_SetModeById(25, 2);
  KIBOMBO_TASK_ADD(Func_02003030, 0x480)
  Func_02003184();
}
