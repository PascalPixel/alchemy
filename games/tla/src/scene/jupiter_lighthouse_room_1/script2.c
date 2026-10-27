#include "types.h"

void WaitTicks(s32 a0);
void Func_02000178(void);

void Func_020001f4(void) {
  WaitTicks(10);
  Func_02000178();
}
