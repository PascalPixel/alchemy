#include "types.h"
#define NULL ((void *)0)

s32 *Func_02003f68(s32);
void Func_02003656(s32, s32, s32, s32, s32);

void Func_02001af4(void) {
    s32 *p = Func_02003f68(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_02003656(12, 22, v + 1, 22, v);
    }
}
