#include "types.h"

#define OverlayObject_ApplyField100 Func_02000d6c

extern void Func_02003ba4(s32 a, s32 b);

s32 OverlayObject_ApplyField100(s32 a) {
    Func_02003ba4(a, *(s16 *)(a + 100));
    return 0;
}
