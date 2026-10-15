#include "types.h"

void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

static __inline__ void Func_02004ff4_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02004ff4(a, b, c, d, e, f, g);
}

void Func_02000cf8(void) { Func_02004ff4_(18, 17, 0, 3, 3, 31, 0); }
