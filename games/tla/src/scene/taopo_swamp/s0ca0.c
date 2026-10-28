#include "local.h"

void Func_02000ca0(void) {
  s32 timer = --gData_0200345c;
  s32 step = timer / 8;

  Func_02002838_(47 - (timer & 1) * 2, 64 - step, 2, step, 29, 15 - step);
  if (gData_0200345c == 0)
    Task_RemoveFar(Func_02000ca0);
}
