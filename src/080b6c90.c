#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080b6c08(s32, void *);
s32 Func_080b7b6c(void *, s32);

void Func_080b6c90(void) {
    u8 local[28];
    Func_080b6c08(3, local);
    Func_080b7b6c(local, 1);
}
