/* Draft, differ 2 of 53: as Func_08164a4c over BG palette entries 96 to
 * 223, the deltas red first. Left: the same prologue order, the pooled 0x1f
 * mask loaded between the two halves of 0x050000c0 in the native. */
#include "types.h"

void Func_08164abc(s32 red_delta, s32 green_delta, s32 blue_delta) {
  u16 *color = (u16 *)0x050000c0;
  s32 i;

  for (i = 0; i != 128; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    red += red_delta;
    green += green_delta;
    blue += blue_delta;
    if (red > 31)
      red = 31;
    if (green > 31)
      green = 31;
    if (blue > 31)
      blue = 31;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }
}
