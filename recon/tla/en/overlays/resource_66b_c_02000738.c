// Near miss, differ 2 of 20 (was 17): the listing loads the table address
// (ldr r4) before it shifts the palette address (lsls r0, r0, #19); this
// draft shifts first. The mask 3 inside the loop is settled: the index
// starts as 3 and is masked with the phase, and the palette address is
// declared before the phase. Measured: a register-input asm naming the
// frame count and the table keeps the shift first and swaps the ldrh
// operands (differ 6); the address started before an in-out barrier moves
// the registers (differ 27).
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
