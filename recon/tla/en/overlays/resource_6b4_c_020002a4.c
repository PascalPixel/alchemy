// Near misses of overlay 6B4, three palette loops over 5-bit channels read
// straight from the colour halfword (a local copy sign-loads it).
// Draft, differ 11 of 48, skeleton gap 5 (tla-en, 2026-11-01): Func_0200032c,
// the worst of the three. Func_02000394 has differ 0. Func_020002a4 has differ
// 10 of 65, skeleton gap 0.
//
// Both near misses need the native's guard, cmp and beq on count, with count
// the lower priority of count and i. The guard written out gives count 4 refs
// (set, guard, twice the loop test), and global.c then ranks it above i in both
// functions: 8/36 over 14/70 here, 8/45 over 14/90 in Func_020002a4. So i and
// count swap (i in ip, count in r7 here; count in lr, i in r8 there). With the
// goto test entry count has 3 refs and allocates as the native does in both,
// and only the guard differs, with the strh and adds r7 order and the pool
// place that follows from it. Func_0200032c takes that entry because the gap
// is smaller (5 against 8); Func_020002a4 keeps the guard for its gap of 0
// (entry jump: differ 6, gap 6). A 3-ref guard is the open question.
//
// Tried with no gain: a test at the top (gap 7), i++ in other places, a copy
// of count (cprop folds it), unsigned i or count, a second i = 0. A real for
// or while duplicates the test as the native does but loop.c lifts the mask
// out of the loop, which the native keeps inside (differ 69); count != i only
// makes the compare look right over the swapped registers.
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

  goto test;
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
test:
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
