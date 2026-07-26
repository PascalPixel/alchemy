#include "types.h"

s32 Func_080030f8(s32);
void Func_0800be70(u8 *object, u32 position);

void Func_0800befc(s32 arg0) {
    u32 var_r6;

    var_r6 = 0;
    do {
        Func_0800be70(arg0, var_r6);
        Func_0800be70(arg0, var_r6 + 1);
        Func_0800be70(arg0, var_r6 + 2);
        Func_0800be70(arg0, var_r6 + 3);
        var_r6 += 4;
        Func_080030f8(1);
    } while (var_r6 <= 0x7FU);
}
