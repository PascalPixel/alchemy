// Matches: differ 0 (tla-en, 2026-11-08)
// The transform is called through The Broken Seal's typed inline from
// iwram_call.h (as f1693d0.c calls Iwram_CopyWords), tagged as a wrapper: the
// inlined call keeps CSE from merging the view's two addresses, so the
// native's mov r1, sp before the call and mov r6, sp after it both come out,
// and the literal load is scheduled before adds r7, r1. Called directly, 4 of
// 82 differ (one sp copy shared across the call); this replaces the operand
// barrier after the call (differ 2), a worse device.
// Earlier, without the inline: the transform through a function-pointer
// local, a (s32 *) cast of the view, a copy of screen, a once-only block
// before the call, the projection set first or depth negated as its own
// statement (4 to 8); a memory, cc or plain asm, a once-only block, a label, a
// block-scoped view pointer or a separate view.z local (4 to 57); 20 permuter
// minutes in all found nothing under 4.
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

// The Broken Seal's typed inline from iwram_call.h, the layer every call goes
// through there.
#undef Iwram_TransformVector
static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to the
   * fixed entry, 4 of 82 instructions differ in tla-en. */
  Iwram_TransformVectorEntry(vector, result);
}

extern struct Projection gCameraSceneParameters;
s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen) {
  struct ProjectedPoint view;
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 x;
  s32 y;

  Iwram_TransformVector(point, &view.x);
  depth = -view.z;
  projection = &gCameraSceneParameters;
  if (depth >= projection->near && depth <= projection->far) {
    screen->z = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb;
    }
    x = projection->centerX + Iwram_MulQ16(view.x, scale) / 0x10000;
    y = projection->centerY - Iwram_MulQ16(view.y, scale) / 0x10000;
    screen->x = x;
    screen->y = y;
    return scale;
  }
  *screen = view;
  return 0;
}
