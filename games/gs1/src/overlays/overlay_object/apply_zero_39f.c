#include "types.h"

#define OverlayObject_ApplyZero Func_02000d80

extern void Func_02003ae2(s32 a, s32 b);

s32 OverlayObject_ApplyZero(s32 a) {
    Func_02003ae2(a, 0);
    return 0;
}
