// Matches (tla-en, 2026-11-07): each channel reads the colour again (as overlay
// 685's Func_02000734 does), which gives the native's pool word for 31; no device.
// Adds a red, green and blue step to each colour of a palette, clamped to 0 to 31.
#include "types.h"
#include "call.h"

void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue) {
  s32 i;

  for (i = 0; i != count; i++) {
    s32 r = (*colors & 31) + red;
    s32 g = (*colors >> 5 & 31) + green;
    s32 b = (*colors >> 10 & 31) + blue;

    if (r > 31)
      r = 31;
    if (r < 0)
      r = 0;
    if (g > 31)
      g = 31;
    if (g < 0)
      g = 0;
    if (b > 31)
      b = 31;
    if (b < 0)
      b = 0;
    *colors++ = (b << 10) | (g << 5) | r;
  }
}
