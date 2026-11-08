#include "local.h"

void Func_02000a98(void) {
  s32 i;

  Func_02003628();
  Func_02003688(16, 4, 16, 2);
  Func_02003688(22, 4, 16, 0);
  for (i = 5; i >= 0; i--) {
    Func_020036b8(33, 55);
    wait(15);
  }
  GameFlag_SetBit(0xa66);
}
