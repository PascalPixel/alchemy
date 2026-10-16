//--------------------------------------------------------------------------
// interpolate_buffers.c
// Palette: calculate the per-frame fade increments.
//--------------------------------------------------------------------------

#include "types.h"
#include "iwram_call.h"

//--------------------------------------------------------------------------
// For the 1536 RGB components of 512 colors, write to step the difference
// between current and target values divided by frames.
// src: current components.
// dst: target components.
// step: destination for component increments.
// frames: fade duration; do nothing if zero or negative.
//--------------------------------------------------------------------------
void PaletteFade_ComputeSteps(s16 *src, s16 *dst, s16 *step, s32 frames) {
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
