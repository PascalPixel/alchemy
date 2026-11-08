#include "alhafra_interiors.h"

u8 *Func_0200004c(void) {
  if (GameFlag_Test(0x8a7))
    return gData_0200116c;
  if (GameFlag_Test(0x8ab))
    return gData_02000f14;
  return gData_02000cbc;
}
