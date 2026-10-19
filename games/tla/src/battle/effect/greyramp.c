#include "types.h"

/// Fills BG palette colours 1 to 63 with a grey ramp, each level i / 2.
void Graphics_LoadGreyRamp(void) {
  s32 i = 0;
  u16 *color = (u16 *)0x05000002;

next:
  {
    s32 level = i / 2;

    i++;
    *color = level << 10 | level << 5 | level;
  }
  color++;
  if (i != 63)
    goto next;
}
