// Draft, differ 0 of 53: writes count RGB555 colours from source to
// destination brightened by 16 - cos(angle) / 4096 on each channel, each
// clamped at 31. Matches.
#include "types.h"
#include "fixed_math.h"

void Func_081b810c(u16 *source, u16 *destination, u32 count, s32 angle) {
  s32 add;
  u32 i;

  add = ((-Trig_Cos(angle)) * 16 >> 16) + 16;
  for (i = 0; i != count; i++) {
    s32 r = (*source & 31) + add;
    s32 g = ((*source >> 5) & 31) + add;
    s32 b = ((*source >> 10) & 31) + add;

    if (r > 31)
      r = 31;
    if (g > 31)
      g = 31;
    if (b > 31)
      b = 31;
    *destination = (b << 10) | (g << 5) | r;
    source++;
    destination++;
  }
}
