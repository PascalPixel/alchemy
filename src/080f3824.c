#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080f3078(s32, s32, s32, s32);

void Func_080f3824(s32 arg0, s32 arg1) {
    s32 temp_r1;

    temp_r1 = *(s32 *)0x03001ED0;
    if (temp_r1 != 0) {
        Func_080f3078(arg0, temp_r1, temp_r1 + 0x400, arg1);
    }
}
