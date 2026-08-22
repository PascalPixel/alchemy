#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_080097b8[];

void Func_0800f9cc(u8 *arg0, s32 arg1) {
    s32 var_r1;
    u8 *var_r0;
    u8 *table;

    var_r0 = arg0;
    table = Data_080097b8;
    var_r1 = arg1 - 1;
    if (var_r1 != -1) {
        do {
            var_r1 -= 1;
            *var_r0 = table[*var_r0];
            var_r0 += 1;
        } while (var_r1 != -1);
    }
}
