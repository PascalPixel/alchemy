#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08004620();
s32 Func_080046c4(s32);

void Func_0800473c(s32 arg0, s32 arg1) {
    s32 var_r5;

    var_r5 = arg1;
    if ((u32) (var_r5 - 1) > 9U) {
        var_r5 = 0xA;
    }
    Func_08004620();
    Func_080046c4(0x03001F7A - var_r5);
}
