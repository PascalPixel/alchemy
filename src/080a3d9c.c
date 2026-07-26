#include "types.h"

s32 Func_08077008();

s32 Func_080a3d9c(s32 arg0, s32 arg1) {
    s32 var_r1;
    s32 var_r5;
    u16 *var_r0;
    s32 temp_r2;
    u16 test;
    s32 temp_r3;
    s32 mask;
    s32 himask;

    var_r5 = 0;
    var_r0 = (u16 *) Func_08077008(arg0);
    mask = 0x1FF;
    himask = 0xF800;
    var_r1 = 0;
    var_r0 += 0x6C;
loop_1:
    temp_r2 = *var_r0;
    test = temp_r2;
    temp_r3 = test;
    var_r0 += 1;
    if ((temp_r3 != 0) && ((mask & temp_r2) == arg1)) {
        var_r5 = (u32) (himask & temp_r2) >> 0xB;
        var_r5 += 1;
    } else {
        var_r1 += 1;
        if (var_r1 <= 0xE) { goto loop_1; }
    }
    return var_r5;
}
