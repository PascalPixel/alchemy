// Near miss, differ 4 of 76 (2026-10-15): the glow colour from a phase
// counter and a level, ⚓️'s form of ☀️'s
// GraphicsPalette_SetTintChannelsFromCounters returning RGB555 instead of
// storing a tint. Two reordered pairs: after the first modulo the native
// loads the table's address before it shifts the phase (ldr r2, =table;
// lsls r5, r5, #18), and after the second it loads the red byte before it
// starts the bias (ldrb r3; subs r6, #7); this does both the other way.
// Measured: the bias after the red read (56), an s16 level (7), the table
// in a local at three places (4 and 6), the modulo and the multiply split
// around the table's load (4, the load then lands between the shifts), an
// empty asm barrier after it (6), an asm reading the table (7); four
// minutes of permuter found nothing better. Belongs between
// Func_080e97c8 and Func_080e9940.
#include "types.h"

// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
// read a third of a turn apart for red, green and blue.
extern const u8 gData_080f0fe0[];

// The glow's colour for a step of its 24-step cycle and a brightness level
// (7 is neutral): three channels of the wave, each raised or lowered by the
// level and held to 0..31, packed as RGB555.
s32 Func_080e989c(s32 counter, s32 level) {
  s16 phase;
  s16 bias;
  s16 red;
  s16 green;
  s16 blue;

  phase = (counter + 12) % 24 * 4;
  bias = level - 7;
  red = gData_080f0fe0[(s16)(phase % 96)] + bias;
  green = gData_080f0fe0[(phase + 32) % 96] + bias;
  blue = gData_080f0fe0[(phase + 64) % 96] + bias;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return (blue << 10) | (green << 5) | red;
}
