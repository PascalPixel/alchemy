#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08015420(s32, s32);
s32 Func_08077008();
s32 Func_08077150(s32);

void Func_080917d0(s32 arg0, s32 arg1) {
    Func_08077008();
    Func_08077150(arg0);
    if (arg1 != 0) {
        Func_08015420(arg0, arg1);
    }
}
