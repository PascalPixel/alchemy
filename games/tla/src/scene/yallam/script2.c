#include "types.h"

void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                   s32 a5, s32 a6) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

void Func_020004cc(void) { Yallam_02002598(67, 25, 2, 2, 87, 8, 1); }
