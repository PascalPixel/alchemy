// Draft, differ 40 of 35: the hblank split of Func_02000e90's values. The
// native rebuilds each register address for each store (0x04000014 twice,
// 0x04000050 by subs from 0x04000052) and loads the zero for BLDCNT from the
// pool; plain, volatile and Io_Write16 stores share one base (differ 33-40).
#include "../../../../games/tla/src/scene/settlements/local.h"
#include "io_reg.h"

void Func_02000e30(void) {
  if (REG_VCOUNT < gData_02002ce0) {
    REG_BG1HOFS = gData_02002ce4;
    REG_BLDCNT = 0;
  } else {
    REG_BG1HOFS = gData_02002ce6;
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}
