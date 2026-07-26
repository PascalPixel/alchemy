#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_0801656c(s32 *arg0) {
    s32 *var_r2;
    s32 *var_r3;

    var_r3 = M2C_FIELD(arg0, s32 **, 0);
    var_r2 = arg0;
    if (var_r3 != NULL) {
        do {
            var_r2 = var_r3;
            var_r3 = *var_r2;
        } while (var_r3 != NULL);
    }
    M2C_FIELD(arg0, s32 **, 4) = var_r2;
}
