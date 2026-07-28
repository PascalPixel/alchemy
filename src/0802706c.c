#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080772b8(void *);

s32 Func_0802706c(const u8 *object) {
    s32 sign_byte;
    s32 kind;
    s32 temp_r2;
    s32 var_r5;
    u8 temp_r3;

    var_r5 = 0;
    temp_r2 = object[1] & 0xF;
    if (temp_r2 == 1) {
        var_r5 = 1;
    }
    if (temp_r2 == 0xB) {
        var_r5 = 2;
    }
    sign_byte = ((const s8 *)object)[3]; kind = (s32)(sign_byte);
    temp_r3 = kind;
    if (temp_r3 == 3) {
        var_r5 = 3;
    }
    if (temp_r3 == 4) {
        var_r5 = 4;
    }
    if (temp_r3 == 0x40) {
        var_r5 = 6;
    }
    if (Func_080772b8(object[3]) != 0) {
        var_r5 = 5;
    }
    return var_r5;
}
