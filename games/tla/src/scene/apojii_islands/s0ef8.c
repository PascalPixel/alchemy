#include "local.h"
#include "map.h"

void Func_02000ef8(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002a98) {
    value = gData_02002a9c;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002a9e;
    /* FAKEMATCH(one-pass): the once-only block orders this statement; without
     * it, 9 of 35 instructions differ in tla-en. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}
