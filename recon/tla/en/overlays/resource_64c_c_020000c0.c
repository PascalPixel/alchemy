// Draft, differ 71 of 58: adds a red, green and blue step to each colour
// of a palette, clamped to 0 to 31. Left: register choice (the native keeps
// count in r7 and the shifted colour in r8, and reads the blue step from its
// stack slot each pass) and the 31 mask from the pool.
#include "types.h"

void Func_020000c0(u16 *colors, s32 count, s32 red, s32 green, s32 blue) {
  s32 i;

  for (i = 0; i != count; i++) {
    u32 color = *colors;
    s32 r = (color & 31) + red;
    s32 g = (((color << 16) >> 21) & 31) + green;
    s32 b = (((color << 16) >> 26) & 31) + blue;

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
