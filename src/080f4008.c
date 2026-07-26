#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080f4168();
s32 Func_080f9010(s32);

s32 Func_080f4008(void) {
    *(s16 *)0x04000000 = 0x40;
    Func_080f9010(9);
    Func_080f4168();
    return 0;
}
