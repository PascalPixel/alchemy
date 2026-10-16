// Near miss, differ 53 of 96 (2026-10-15; was 60): The Broken Seal's
// Graphics_UpdatePhasePalette with ⚓️'s levels (each scaled sine plus 16, times
// the index, over 64) and its statement order (the first sine is scaled before
// the second call, the other two after the third). Left: allocation. The native
// keeps the blue phase in r10 and the first level in r8, the index in r4 and
// the output in r5, and scales green and blue in place; here the blue phase
// takes r8, the level r10, and green's level is a copy. Tried: the first level
// pinned to r8 with or without the blue phase pinned to r10, the products
// written either way round, the output set before the index (differ 64 to 71).
// 2026-10-15: green's and blue's scaled sines folded into their levels (60 to
// 53; in place, green += 16: 65; every level in place: 69; red's too: 68). The
// native still keeps the output pointer in r5 and the index in r4 (here r1 and
// r5), and copies the first sine through r3, here r2.
#include "types.h"
#include "iwram_call.h"

s32 Trig_Sin(s32 angle);

void Func_081496c8(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase) {
  u16 palette[64];
  u16 *output;
  s32 phase;
  s32 red_level;
  s32 green_level;
  s32 blue_level;
  s32 index;
  s32 red;
  s32 green;
  s32 blue;

  phase = frame * 0x400;
  red = (Trig_Sin(phase + red_phase) * 16) >> 15;
  green = Trig_Sin(phase + green_phase);
  blue = Trig_Sin(phase + blue_phase);

  palette[0] = 0;
  red_level = red + 16;
  green_level = ((green * 16) >> 15) + 16;
  blue_level = ((blue * 16) >> 15) + 16;
  index = 1;
  output = &palette[1];
  do {
    s32 r;
    s32 g;
    s32 b;

    r = red_level * index / 64;
    g = green_level * index / 64;
    b = blue_level * index / 64;

    if (r < 0)
      r = 0;
    if (r > 31)
      r = 31;
    if (g < 0)
      g = 0;
    if (g > 31)
      g = 31;
    if (b < 0)
      b = 0;
    if (b > 31)
      b = 31;

    *output++ = (b << 10) | (g << 5) | r;
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)0x05000002, palette, sizeof(palette));
}
