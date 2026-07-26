#include "types.h"

s32 Func_080064f4(void) {
    s32 var_r0;

    var_r0 = 0;
    if (*(s32 *)0x02002080 != 0) {
        var_r0 = 1;
    }
    if (*(s32 *)0x020023AC != 0) {
        var_r0 |= 2;
    }
    return var_r0;
}
