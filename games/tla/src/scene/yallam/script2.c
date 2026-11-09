#include "types.h"

void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                   s32 a5, s32 a6) {
  Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

void Func_020004cc(void) { Yallam_02002598(67, 25, 2, 2, 87, 8, 1); }
