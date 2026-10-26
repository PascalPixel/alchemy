// Draft, differ 6 of 82 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 6 of 82 (2026-10-24): an empty asm after the transform call
// takes one reordered pair off. Left: the native forms the view's address
// twice (mov r1, sp before the call, mov r6, sp after); here one copy serves
// both.
// Draft, differ 8 of 82: the native passes the view's address to the
// transform as a fresh mov r1, sp and forms it again after the call (mov r6,
// sp); here one copy in r6 is made before the call and r1 is copied from it,
// which shifts three loads by a slot. Measured: a one-pass block after the
// call (55), the view through a pointer set after the call behind an operand
// barrier (13) or in a one-pass block (10); four minutes of permuter found
// nothing better.
#include "types.h"
#include "iwram_call.h"

struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX;
  s32 centerY;
};

struct ProjectedPoint {
  s32 x;
  s32 y;
  s32 z;
};

extern struct Projection gCameraSceneParameters;
s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen) {
  struct ProjectedPoint view;
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 x;

  Iwram_TransformVector(point, &view.x);
  __asm__ volatile("");
  projection = &gCameraSceneParameters;
  depth = -view.z;
  if (depth >= projection->near && depth <= projection->far) {
    screen->z = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb;
    }
    x = projection->centerX + Iwram_MulQ16(view.x, scale) / 0x10000;
    screen->y = projection->centerY - Iwram_MulQ16(view.y, scale) / 0x10000;
    screen->x = x;
    return scale;
  }
  *screen = view;
  return 0;
}
