#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0800b798(s32 arg0) {
    u32 var_r1;
    void *var_r2;

    var_r2 = *(s32 *)0x03001E68 + 0x1C;
    var_r1 = 0;
loop_1:
    if (M2C_FIELD(var_r2, s32 *, 0) == arg0) {
        return M2C_FIELD(var_r2, s32 *, 4);
    }
    var_r1 += 1;
    var_r2 += 8;
    if (var_r1 > 7U) {
        return 0;
    }
    goto loop_1;
}
