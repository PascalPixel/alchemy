#include "types.h"
#define NULL ((void *)0)

void Func_02004968(s32, s32);
void Func_02004972(s32, s32);
void Func_02004768(s32);

void Func_020022f8(s32 arg0) {
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Func_02004968(arg0, 7);
    } else {
        Func_02004972(arg0, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Func_02004768(arg0);
    }
}
