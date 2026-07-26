#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08004080();
s32 Func_0801eadc(s32 arg0, s32 arg1, struct Input *arg2, s32 arg3, s32 arg4);
s32 Func_08021b30(s32 arg0, s32 arg1);

s32 Func_08022a38(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;

    temp_r0 = Func_08004080();
    if (temp_r0 != 0x60) {
        Func_08021b30(arg3, temp_r0);
        Func_0801eadc(temp_r0, 0x40000000, arg0, arg1, arg2);
    }
}
