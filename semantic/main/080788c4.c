#include "types.h"

s32 Func_08077394(s32);
void Func_08077428(s32);

s32 Func_080788c4(s32 arg0, s32 arg1)
{
    s32 temp_r2;
    s32 temp_r5;
    s32 var_r6;
    s32 temp_r0;
    u32 temp_r3_2;
    u16 *temp_r6;
    u16 *var_r1;
    u16 *var_r4;
    u16 temp_r2_2;
    u16 temp_r3;

    temp_r0 = Func_08077394(arg0);
    temp_r5 = (arg1 * 2) + 0xD8;
    temp_r3 = *(u16 *)(temp_r0 + temp_r5);
    var_r6 = -1;
    if (temp_r3 != 0) {
        temp_r2 = 0xF800 & temp_r3;
        if (temp_r2 != 0) {
            *(u16 *)(temp_r0 + temp_r5) = temp_r3 - 0x800;
            var_r6 = 1;
        } else {
            temp_r6 = (u16 *)(temp_r0 + 0xD8);
            *(u16 *)(temp_r0 + temp_r5) = temp_r2;
            var_r4 = temp_r6;
            temp_r5 = 0;
            var_r1 = temp_r6;
            temp_r0 = 14;
            do {
                temp_r2_2 = *var_r4;
                temp_r3_2 = temp_r2_2;
                temp_r3_2 <<= 16;
                var_r4 += 1;
                if (temp_r3_2 != 0) {
                    *var_r1 = temp_r2_2;
                    temp_r5 += 1;
                    var_r1 += 1;
                }
                temp_r0 -= 1;
            } while (temp_r0 >= 0);
            if (temp_r5 <= 14) {
                temp_r0 = (temp_r5 * 2) + (s32)temp_r6;
                temp_r5 = 15 - temp_r5;
                do {
                    temp_r5 -= 1;
                    *(s16 *)temp_r0 = 0;
                    temp_r0 += 2;
                } while (temp_r5 != 0);
            }
            var_r6 = 2;
        }
    }
    Func_08077428(arg0);
    return var_r6;
}
