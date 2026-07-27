#include "types.h"

void *Func_08077008(s32 kind);

s32 Func_080a3d6c(s32 kind) {
    s32 temp_r3;
    s32 var_r1;
    s32 var_r5;
    u16 *var_r0;

    var_r5 = 0;
    var_r0 = (u16 *)((u8 *)Func_08077008(kind) + 0xD8);
    var_r1 = 0xE;
    do {
        temp_r3 = 0x1FF & *var_r0;
        var_r0 += 1;
        if (temp_r3 != 0) {
            var_r5 += 1;
        }
        var_r1 -= 1;
    } while (var_r1 >= 0);
    return var_r5;
}
