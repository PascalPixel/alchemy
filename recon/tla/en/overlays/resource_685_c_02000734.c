// Draft, differ 129 of 97: fades five colours of the last object palette from
// one set to another over the given number of frames, one step a frame.
#include "types.h"

void wait(s32 frames);

void Func_02000734(const u16 *from, const u16 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 left = 4;
    s32 offset = 0;

    do {
      u32 a = *(u16 *)((u8 *)from + offset) << 16;
      u32 b = *(u16 *)((u8 *)to + offset) << 16;
      s32 red = (a >> 16) & 31;
      s32 green = (a >> 21) & 31;
      s32 blue = (a >> 26) & 31;
      s32 toGreen = (b >> 21) & 31;
      s32 toBlue = (b >> 26) & 31;

      red += step * (((b >> 16) & 31) - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      *(u16 *)(0x050001e4 + offset) = blue << 10 | green << 5 | red;
      offset += 2;
    } while (--left >= 0);
    wait(1);
  }
}
