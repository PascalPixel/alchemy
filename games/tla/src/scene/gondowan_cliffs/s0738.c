#include "types.h"
#include "frame.h"

extern const u16 gData_02001ae0[];

void Func_02000738(void) {
  u16 *color = (u16 *)0x05000018;
  u32 phase = gFrameCount >> 4;
  s32 i;

  for (i = 3; i >= 0; i--) {
    u32 index = 3;

    index &= phase;
    *color++ = gData_02001ae0[index];
    phase++;
  }
}
