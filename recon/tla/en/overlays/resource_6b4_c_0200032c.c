// Draft, differ 22 of 48: the palette shift loop (each 5-bit component plus
// delta, clamped to 0..31), written as a goto loop because the native keeps the
// green and blue mask's pool load inside the loop, which a for loop hoists
// (differ 69). Left: the native keeps the index in r7 and the count in ip, the
// draft the reverse, which adds two instructions and moves the literal pool.
// Tried: u32 count and index, i = 0 after the count test, ++i, a guarding if, a
// 1-minute permute. Func_02000394 inlines the same loop over palette 0x050000c2
// into 0x05000082.
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
  if (count != i)
    goto loop;
}
