#include "alhafra_interiors.h"

u8 *Func_0200004c(void) {
  if (GameFlag_Test(SPLIT_CONSTANT(0x7a8, 255)))
    return gData_0200116c;
  if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171)))
    return gData_02000f14;
  return gData_02000cbc;
}
