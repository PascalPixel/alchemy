/// @file
/// The hblank split: above the line Func_02000ed0 sets, BG1 scrolls by one value and
/// blending is off; below it, by the other, with BG1 blended.
#include "../settlements/local.h"
#include "io_reg.h"
#include "system/io_write.h"

void Func_02000e30(void) {
  if (REG_VCOUNT < gData_02002ce0) {
    Io_Write16(gData_02002ce4, &REG_BG1HOFS);
    REG_BLDCNT = 0;
  } else {
    Io_Write16(gData_02002ce6, &REG_BG1HOFS);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}
