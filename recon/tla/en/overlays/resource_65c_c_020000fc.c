// Draft, differ 33 of 49: the palette cycle Func_02000074 schedules. With the
// counter read as volatile every access up to the first colour matches (the
// native reloads it after the conditional reset; unqualified, the reset is
// threaded into a register, differ 54). Left: the native keeps the colour table
// 0x05000180 in r0 for all four reads and builds 0x050000c4 in r4; this reloads
// the table into r4 for the last three and turns r0 into the destination (subs
// r0, #188). An empty one-pass block after the reset reloads too but copies the
// counter's address (differ 32); the table as a forced temporary indexes the
// reads (ldrh r3, [r3, r4], differ 43).
#include "types.h"

extern volatile u32 gData_020001b0[];
extern u32 gFrameCount;

void Func_020000fc(void) {
  if ((gFrameCount & 15) == 0) {
    u16 *colors = (u16 *)0x05000180;
    s32 i;

    if (gData_020001b0[0] == 8)
      gData_020001b0[0] = 0;
    i = gData_020001b0[0]++;
    *(u16 *)0x050000c2 = colors[i];
    *(u16 *)0x050000c4 = colors[(i + 1) & 7];
    *(u16 *)0x050000c6 = colors[(i + 2) & 7];
    *(u16 *)0x050000c8 = colors[(i + 3) & 7];
  }
}
