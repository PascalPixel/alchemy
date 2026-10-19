#include "types.h"

void Func_02004c28(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

static inline void MapCopy(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004c28(a0, a1, a2, a3, a4, a5);
}

void Func_02000c68(void) { MapCopy(25, 9, 11, 3, 26, 9); }
