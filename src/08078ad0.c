#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_0807b490[];
extern s32 Func_08078aa0(s32 arg0, s32 arg1);

s32 Func_08078ad0(s32 arg0, s32 arg1) {
    s32 mask = 0x1ff;
    u8 temp;
    s32 var_r4 = 0;

    temp = Data_0807b490[arg0 & mask];
    if (temp != 0) {
        var_r4 = Func_08078aa0(temp - 1, arg1);
    }
    return var_r4;
}
