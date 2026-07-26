#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080e2974(void *, s32);

void Func_080e294c(void *arg0) {
    s32 temp_r3;

    temp_r3 = M2C_FIELD(arg0, s32 *, 0x18);
    if (temp_r3 == 0) {
        Func_080e2974(arg0, 3);
        return;
    }
    if (temp_r3 == 1) {
        Func_080e2974(arg0, 4);
        return;
    }
    Func_080e2974(arg0, 5);
}
