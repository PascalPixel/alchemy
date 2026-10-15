#include "room.h"

const u8 *Func_02000044(void) {
  if (!GameFlag_Test(0x960))
    return gData_0200057c;
  return gData_0200059c;
}
