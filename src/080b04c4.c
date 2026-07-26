#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080030f8(s32);
s32 Func_080f9048(void);

void Func_080b04c4(void) {
    while (Func_080f9048() != 0) {
        Func_080030f8(1);
    }
}
