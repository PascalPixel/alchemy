#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08004080();
s32 Func_080150c8(s32, s32, s32, s32, s32);
s32 Func_080ae908(s32 arg0, s32 arg1);

s32 Func_080ae958(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;

    temp_r0 = Func_08004080();
    if (temp_r0 != 0x60) {
        Func_080ae908(arg3, temp_r0);
        Func_080150c8(temp_r0, 0x40000000, arg0, arg1, arg2);
    }
}
