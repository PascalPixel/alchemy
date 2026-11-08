/// @file
/// Menu: make brightness-adjusted copies of BG palettes.

#include "types.h"

/// Adds d to the RGB components of the 16 colours of BG palettes 15, 5 and 7,
/// clamped to 0-31, and writes palettes 14, 4 and 6. Palette 7 uses -12
/// instead of d.
void Graphics_AdjustPalettes(s32 d) {
  s32 n;
  s32 paletteNumber;
  s32 col;
  s32 mask;
  s32 idx;

  paletteNumber = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 v;

      idx = paletteNumber * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += d;
      g += d;
      r += d;
      if (b > 31)
        b = 31;
      if (g > 31)
        g = 31;
      if (r > 31)
        r = 31;
      if (b < 0)
        b = 0;
      if (g < 0)
        g = 0;
      if (r < 0)
        r = 0;
      v = b << 10;
      v |= g << 5;
      v |= r;
      ((s16 *)0x04FFFFE0)[idx] =
          v; // 16 colors earlier (the preceding palette).
    }
    if (n == 0) {
      paletteNumber = 5;
    } else {
      paletteNumber = 7;
      d = -12;
    }
    n++;
  } while (n <= 2);
}
