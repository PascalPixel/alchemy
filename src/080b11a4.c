#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08015080(s32, s32, s32, s32);
s32 Func_08015270();

void Func_080b11a4(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        Func_08015270();
        Func_08015080(arg1, arg0, 0, 0);
    }
}
