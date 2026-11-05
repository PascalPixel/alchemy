// Draft (tla-en, 5 November 2026), under -Os with plain loops. Fades five background colours, from palette entry 114, from one set to another over the given number of frames, one step a frame. Left: the registers are the native's; the reload scratch registers differ, starting with the first step's 0 (r0 in the native, r1 here).
#include "types.h"
#include "call.h"

void wait(s32 frames);

void Func_02000734(u8 *from, u8 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 offset;
    s32 left;

    for (left = 4, offset = 0; left >= 0; left--) {
      s32 c, d, red, green, blue, toRed, toGreen, toBlue;

      red = 31;
      c = *(u16 *)(from + offset);
      green = ((u16)c >> 5) & 31;
      d = *(u16 *)(to + offset);
      red &= c;
      blue = ((u16)c >> 10) & 31;
      toRed = 31;
      toRed &= d;
      toGreen = ((u16)d >> 5) & 31;
      toBlue = ((u16)d >> 10) & 31;
      red += step * (toRed - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      *(u16 *)(0x050000e4 + offset) = blue << 10 | green << 5 | red;
      offset += 2;
    }
    wait(1);
  }
}
