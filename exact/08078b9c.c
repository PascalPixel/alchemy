#include "types.h"

extern const u8 Data_0807ee58[];

s32 Func_08078b9c(s32 arg0) {
    u32 var_r0;

    var_r0 = arg0 & 0x3FFF;
    if (var_r0 >= 0x208U) {
        var_r0 = 0;
    }
    return (s32)(Data_0807ee58 + var_r0 * 0x10);
}
