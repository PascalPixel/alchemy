#include "types.h"
#include "event_inline.h"

void Func_02003048(void);
void Func_02003050(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void TundariaTower_02002fe8(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

static inline void TundariaTower_02002fc0(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  Map_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000548(void) {
  Func_02003048();
  TundariaTower_02002fe8(19, 55, 7, 8, 24, 46);
  TundariaTower_02002fc0(24, 102, 24, 110, 7, 8);
  GameFlag_SetBit(2370);
  Func_02003050();
}

void Func_0200058c(void) {
  Func_02003048();
  TundariaTower_02002fe8(37, 55, 7, 8, 32, 46);
  TundariaTower_02002fc0(32, 102, 32, 110, 7, 8);
  GameFlag_SetBit(2371);
  Func_02003050();
}
