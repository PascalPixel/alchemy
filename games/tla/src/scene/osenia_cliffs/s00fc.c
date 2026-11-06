/// @file
/// The palette cycle Func_02000074 schedules: every 16 frames it copies four
/// of the eight colours at 0x05000180, starting one further each time, to the
/// colours at 0x050000c2.
#include "types.h"

extern u32 gData_020001b0[];
extern u32 gFrameCount;

void Func_020000fc(void) {
  if ((gFrameCount & 15) == 0) {
    volatile u16 *colors;
    s32 i;

    if (gData_020001b0[0] == 8)
      gData_020001b0[0] = 0;
    i = gData_020001b0[0]++;
    /* FAKEMATCH(volatile): the colours read through a volatile view; as plain
     * u16, 22 of 49 instructions differ in tla-en. */
    colors = (volatile u16 *)0x05000180;
    *(u16 *)0x050000c2 = colors[i];
    *(u16 *)0x050000c4 = colors[(i + 1) & 7];
    *(u16 *)0x050000c6 = colors[(i + 2) & 7];
    *(u16 *)0x050000c8 = colors[(i + 3) & 7];
  }
}
