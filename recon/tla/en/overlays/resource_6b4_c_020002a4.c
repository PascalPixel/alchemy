// Near misses of overlay 6B4, three palette loops over 5-bit channels read
// straight from the colour halfword (a local copy sign-loads it). Func_020002a4
// (65 instructions, 10 differ): the native counts in lr and keeps the count in
// r8, the draft the reverse. Func_0200032c (48, 24): the native counts in r7
// with the count in r12, loads the pool word 0x1f before the mask 31 and emits
// its pool after the green clamp. Func_02000394 (64, 12): the same pool word
// order and pool place.
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

void Func_0200032c(u16 *colors, s32 count, s32 delta) {
  s32 i = 0;

  if (count == 0)
    return;
next:
  {
    s32 r = (*colors & 31) + delta;
    s32 g = ((*colors >> 5) & 31) + delta;
    s32 b = ((*colors >> 10) & 31) + delta;

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
    i++;
    *colors = b << 10 | g << 5 | r;
  }
  colors++;
  if (i != count)
    goto next;
}

void Func_02000394(void) {
  u16 *src = (u16 *)0x050000c2;
  u16 *dst = (u16 *)0x05000082;
  s32 delta = (Math_Sine((gFrameCount << 11) >> 1) * 2) >> 16;
  s32 i = 0;

next:
  {
    s32 r = (*src & 31) + delta;
    s32 g = ((*src >> 5) & 31) + delta;
    s32 b = ((*src >> 10) & 31) + delta;

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
    *dst = b << 10 | g << 5 | r;
  }
  i++;
  src++;
  dst++;
  if (i != 15)
    goto next;
}
