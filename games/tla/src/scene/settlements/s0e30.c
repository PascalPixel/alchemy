#include "../apojii_islands/local.h"
#include "map.h"

void Func_02000e30(void);
extern s32 gData_02002ce0;
extern u16 gData_02002ce4;
extern u16 gData_02002ce6;

void Func_02000e30(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002ce0) {
    value = gData_02002ce4;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002ce6;
    /* FAKEMATCH(one-pass): the once-only block orders this statement; as a
     * plain statement, 9 of 35 instructions differ in tla-en. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}
