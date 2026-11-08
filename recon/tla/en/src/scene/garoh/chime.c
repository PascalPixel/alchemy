#include "../../../../../../games/tla/src/scene/garoh/garoh.h"

extern s32 gData_0200632c;

void Func_02000a68(void) {
  if (!Flag_IsSet(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 27))) && Flag_IsSet(0x310)) {
    gData_0200632c++;
    if ((gData_0200632c & 0x3ff) == 0)
      Func_02005394(SPLIT_CONSTANT(SPLIT_CONSTANT(0x10e, 255)));
  }
}
