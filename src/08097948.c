#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080979a4(s32, s32, s32);

void Func_08097948(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    *arg1 = Func_080979a4(arg0 + 0x780000, 0, 0x1F0000);
    *arg2 = Func_080979a4(arg0, 0, 0x1F0000);
    *arg3 = Func_080979a4(arg0 + 0xFF880000, 0, 0x1F0000);
}
