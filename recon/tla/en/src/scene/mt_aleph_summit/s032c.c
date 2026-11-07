#include "types.h"

void Func_0200032c(u16 *colors, s32 count, s32 delta) {
  s32 i;
  s32 r, g, b;

  i = 0;
  if (count == 0)
    return;
loop:
  r = (*colors & 0x1f) + delta;
  g = ((*colors >> 5) & 0x1f) + delta;
  b = ((*colors >> 10) & 0x1f) + delta;
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
  i++;
  if (i != count)
    goto loop;
}
