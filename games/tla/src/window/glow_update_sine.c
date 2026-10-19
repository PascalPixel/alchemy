/// @file
/// Sine-driven glow for colors 8 through 11 of background palette 14.

#include "edition.h"
#include "types.h"
#include "fixed_math.h"

extern u32 gFrameTick;

/// Updates four RGB555 colors with an eight-frame phase offset between them.
/// For s = sin / 16384, red is 22 + 2s, green 16 + s, and blue 20 + s.
void PaletteGlow_UpdateSine(void) {
  s32 *tick;
  volatile u16 *pal;
  s32 col;
  s32 s;
  s32 i;

  tick = (s32 *)&gFrameTick;
  pal = (volatile u16 *)0x050001D0; // Background palette 14, color 8.
  i = 0;
  do {
#ifdef UBFIX
    // The unsigned frame phase wraps without overflowing signed arithmetic.
    s = Trig_Sin((((u32)*tick + i * 8U) * 0x300U) & 0xffffU);
#else
    s = Trig_Sin((*tick + i * 8) * 0x300);
#endif
    col = s / 16384;
    {
#ifdef UBFIX
      // The sine can be negative; shift its bits as an unsigned value.
      u32 r = (u32)col << 1;
#else
      u32 r = col << 1;
#endif
      u32 g = col;

      r += 22;
      g += 16;
      col += 20;
      /* FAKEMATCH: one-pass block; its loop notes hold the blue shift after the
       * three adds; as a plain shift, 6 instructions differ in tla-ja. */
      do {
        col <<= 10;
      } while (0);
      g <<= 5;
      col |= g;
      col |= r;
    }
    /* FAKEMATCH: one-pass block; it keeps the store ahead of the loop counter
     * increment; as a plain store, 2 instructions differ in tla-ja. */
    do {
      *pal++ = col;
    } while (0);
  } while (++i <= 3);
}
