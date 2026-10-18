// Draft (ARM, psynergy diff --arm), differ 49 of 31: ColorBuffer_ScaleKernelCode,
// the ARM kernel ColorBuffer_Scale
// (games/tla/src/graphics/palette/color_scale_loader.s) copies to the stack
// and runs: scales count BGR555 colours by scale/256 (at most 1.0), each
// channel masked in place. No ☀️ twin. Left: other. The listing opens with a
// Thumb bx pc into ARM (a mode switch, which no C makes), keeps the three
// channel masks in r6 to r8 across the loop and folds each shift into its
// and (and r1, r8, r1, lsr #8), tests the clamp as cmp #256/movge and counts
// down with subs/bne; GCC 2.95.2 keeps the masks as immediates, clamps with
// cmp #255/movgt and adds a cmp. Likely handwritten (S1) rather than a near
// miss. Tried: a for loop (55), the count as a do-while (49).
#include "types.h"

s32 ColorBuffer_ScaleKernel(u16 *source, u16 *destination, s32 scale,
                            s32 count) {
  s32 factor = scale >> 8;
  s32 i;

  if (factor >= 0x100)
    factor = 0x100;
  if (count > 0) {
    i = count;
    do {
    u32 color = *source++;
    u32 red = ((color & 0x1f) * factor >> 8) & 0x1f;
    u32 green = ((color & 0x3e0) * factor >> 8) & 0x3e0;
    u32 blue = ((color & 0x7c00) * factor >> 8) & 0x7c00;

    *destination++ = red | green | blue;
    } while (--i != 0);
  }
  return 0;
}
