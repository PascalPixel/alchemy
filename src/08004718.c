#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080045f0();
s32 Func_080046c4(s32);

void Func_08004718(s32 arg0, s32 arg1) {
    s32 var_r5;

    var_r5 = arg1;
    if ((u32) (var_r5 - 1) > 7U) {
        var_r5 = 8;
    }
    Func_080045f0();
    Func_080046c4(0x03001F78 - var_r5);
}
