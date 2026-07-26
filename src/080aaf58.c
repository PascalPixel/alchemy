#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s16 Func_080ac8fc(void *, s32, s32);

s32 Func_080aaf58(void *arg0) {
    void *temp_r3;
    s32 var_r4;
    void *var_r5;
    u16 *var_r6;
    s8 *var_r7;
    s16 temp_r0;
    s32 count;

    temp_r3 = *(void **)0x03001F2C;
    var_r4 = 0;
    if (var_r4 < M2C_FIELD(temp_r3, u8, 0x219)) {
        var_r7 = (s8 *)arg0 + 0xA0;
        var_r6 = (u16 *)((u8 *)temp_r3 + 0x208);
        var_r5 = arg0;
        do {
            temp_r0 = Func_080ac8fc(var_r5, *var_r6, -1);
            count = M2C_FIELD(temp_r3, u8, 0x219);
            var_r4 += 1;
            *var_r7 = temp_r0;
            var_r6 += 1;
            var_r7 += 1;
            var_r5 = (u8 *)var_r5 + 0x14;
        } while (var_r4 < count);
    }
}
