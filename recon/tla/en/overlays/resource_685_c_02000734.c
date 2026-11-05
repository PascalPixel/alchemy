// Draft, differ 12 of 97 (tla-en, 5 November 2026). Fades five background colours, from
// palette entry 114, from one set to another over the given number of frames, one step a
// frame. Each channel reads the colour again, which PRE merges, and the order of the two
// for-loop initial values follows the reload rotation. Left: three scheduling pairs (add sl
// after adds r1; the colour's mov r0, r8 before movs r3; the loop's movs r2 before ldr r3).
#include "types.h"
#include "call.h"

void wait(s32 frames);

void Func_02000734(u16 *from, u16 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 offset;
    s32 left;

    for (offset = 0, left = 4; left >= 0; left--) {
      s32 red, green, blue, toRed, toGreen, toBlue;

      red = *(u16 *)(offset + (s32)from) & 31;
      green = *(u16 *)(offset + (s32)from) >> 5 & 31;
      blue = *(u16 *)(offset + (s32)from) >> 10 & 31;
      toRed = *(u16 *)(offset + (s32)to) & 31;
      toGreen = *(u16 *)(offset + (s32)to) >> 5 & 31;
      toBlue = *(u16 *)(offset + (s32)to) >> 10 & 31;
      red += step * (toRed - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      { s32 color = blue << 10 | green << 5 | red; *(u16 *)(0x050000e4 + offset) = color; }
      offset += 2;
    }
    wait(1);
  }
}
