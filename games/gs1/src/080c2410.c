#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_080c7420[];

u32 Func_080c2410(u32 arg0) {
    u32 temp_r3;
    u32 var_r0;
    u8 *table;

    if (arg0 > 0xABU) {
        return 0U;
    }
    table = Data_080c7420;
    temp_r3 = table[(arg0 * 8) + 2] >> 5;
    if (temp_r3 != 0) {
        var_r0 = temp_r3;
    } else {
        var_r0 = 0;
    }
    return var_r0;
}
