#include "types.h"
#define NULL ((void *)0)

s32 *Func_02003f20(s32);
void Func_0200360e(s32, s32, s32, s32, s32);

void Func_02001aac(void) {
    s32 *p = Func_02003f20(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200360e(10, 13, v - 1, 13, v);
    }
}
