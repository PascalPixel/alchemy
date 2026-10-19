#include "types.h"

void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
void Func_02002468(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02002470(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                   s32 a5, s32 a6) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

void Func_02000328(void) {
  Func_02002598(67, 25, 5, 5, 67, 5, 1);
  Func_02002468(23, 29, 5, 6, 1, 1);
  Func_02002470(23, 29, 1, 1, 5, 8);
}

void Func_0200036c(void) { Yallam_02002598(67, 25, 2, 1, 79, 22, 1); }
