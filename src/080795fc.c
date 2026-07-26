#include "types.h"

s32 Func_08079338(s32);

s32 Func_080795fc(void) {
    s32 var_r5;
    s32 var_r6;

    var_r6 = 0;
    var_r5 = 0;
    do {
        if (Func_08079338(var_r5) != 0) {
            var_r6 += 1;
        }
        var_r5 += 1;
    } while (var_r5 <= 7);
    return var_r6;
}
