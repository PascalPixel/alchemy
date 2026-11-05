#include "types.h"

extern u32 gFrameCount;
s32 Math_Sine(s32 angle);

void Func_020002a4(u16 *colors, u16 *target, s32 count) {
  s32 i = 0;

  if (count == 0)
    return;
next:
  {
    s32 to_r = *target & 31;
    s32 to_g = (*target >> 5) & 31;
    s32 to_b = (*target >> 10) & 31;
    s32 r = *colors & 31;
    s32 g = (*colors >> 5) & 31;
    s32 b = (*colors >> 10) & 31;

    if (r < to_r)
      r++;
    else if (r > to_r)
      r--;
    if (g < to_g)
      g++;
    else if (g > to_g)
      g--;
    if (b < to_b)
      b++;
    else if (b > to_b)
      b--;
    *colors = b << 10 | g << 5 | r;
  }
  i++;
  target++;
  colors++;
  if (i != count)
    goto next;
}
