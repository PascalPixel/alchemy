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
