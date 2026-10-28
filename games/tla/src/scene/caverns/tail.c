#include "caverns.h"

void Func_02000f50(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    Call6((void (*)())Func_020010ac, 28, 0, 1, 1, 28, 5);
    Call6((void (*)())Func_020010ac, 36, 0, 1, 1, 36, 5);
  }
}

void Func_02000f98(void) {
  Func_02000454(gData_02001220);
  Actor_GetById(8)->priorityFlags |= 0x20;
  Actor_GetById(9)->priorityFlags |= 0x20;
  Actor_GetById(10)->collisionFlags |= 8;
  Actor_GetById(11)->collisionFlags |= 8;
}

void Func_02000fe4(void) {
  Func_020010dc();
  Func_0200113c(0);
  Func_02001164(158);
  FieldEvent_Wait(10);
  Call6((void (*)())Func_02001094, 7, 94, 6, 94, 1, 2);
  FieldEvent_Wait(16);
  Func_02001164(123);
  Func_02001134(8);
  Func_020010e4();
}
