#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08004080();
s32 Func_0801a32c(s32, s32, s32);
s32 Func_0801eadc(s32, s32, s32, s32, s32);

s32 Func_0801ec24(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;
    s32 var_r0;

    temp_r0 = Func_08004080();
    var_r0 = 0;
    if (temp_r0 != 0x60) {
        Func_0801a32c(arg0, 0, temp_r0);
        var_r0 = Func_0801eadc(temp_r0, 0x40000000, arg1, arg2, arg3);
    }
    return var_r0;
}
