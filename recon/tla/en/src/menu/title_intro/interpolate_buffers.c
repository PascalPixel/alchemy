/// @file
/// Palette: calculate the per-frame fade increments.

#include "types.h"
#include "iwram_call.h"

/// Writes to step, for each of the 1536 RGB components of 512 colours, the
/// difference between current and target values divided by frames.
/// src: current components.
/// dst: target components.
/// step: destination for component increments.
/// frames: fade duration; nothing is written if zero or negative.
void PaletteFade_CalculateSteps(s16 *src, s16 *dst, s16 *step, s32 frames) {
  s32 n;
  s32 from;
  s32 to;

  if (frames > 0) {
    for (n = 0; n < 3 * 512; n++) {
      from = *src;
      to = *dst;
      *step = Iwram_SignedDivide(to - from, frames);
      src++;
      dst++;
      step++;
    }
  }
}
