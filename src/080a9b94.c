#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080a9b94(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_r5;
    void *temp_r0;
    void **var_r6;

    var_r5 = 0;
    var_r6 = *(s32 *)0x03001F2C + 0x48;
    do {
        temp_r0 = *var_r6++;
        if (temp_r0 != NULL) {
            Func_080a9bd8(temp_r0, var_r5, arg0, arg1, arg2);
        }
        var_r5 += 1;
    } while (var_r5 <= 0x1F);
}
