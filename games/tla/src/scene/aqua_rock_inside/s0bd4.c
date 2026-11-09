#include "local.h"

void Func_02000bd4(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(0x249)) {
    Func_02000b24(12, 38);
    Func_02003628();
    Func_02003688(12, 2, 45, 1);
    for (i = 3; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(0x248);
    if (result == 1)
      Event_SetFlag(0xa64);
  }
}
