// Near miss: differ 8 of 20 (2026-10-12). Only the schedule of the two register
// addresses differs: the native starts BLDALPHA's address (movs r7) after
// BLDCNT's is complete and loads the 0x10xx base between its two halves.
#include "types.h"
#include "io_reg.h"
#include "system.h"

void Func_08126978(void) {
  s32 i;

  REG_BLDCNT = 0x2044;
  for (i = 1; i <= 16; i += 2) {
    REG_BLDALPHA = i + 0x1000;
    wait(1);
  }
}
