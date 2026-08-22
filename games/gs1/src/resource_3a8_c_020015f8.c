#include "types.h"

extern s32 Func_0200521a(s32);

void Func_020015f8(void) {
    u8 *p;
    s32 v;

    p = (u8 *)(Func_0200521a(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}
