#include "types.h"

s32 Func_080770c0(s32);
s32 Func_08019d2c(s32);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);

void Func_0801ed40(u32 arg0, s32 arg1, s32 arg2)
{
    s32 sp8;
    s32 spC;
    u32 temp_r0;
    u32 var_r6;
    u8 *temp_r7;
    s32 offset;

    var_r6 = arg0;
    temp_r7 = *(u8 **)0x03001e8c;

    if (Func_080770c0(0x20) != 0) {
        if (arg1 == 0)
            arg1 = 0x12;
        if (arg1 == 1)
            arg1 = 0x13;
    }

    temp_r0 = Func_08019d2c(arg1);
    if (temp_r0 != -1U) {
        if (var_r6 > 1U) {
            if (*(u16 *)(temp_r7 + 0x12ee) == temp_r0) {
                var_r6 = 1;
            } else if (*(u16 *)(temp_r7 + 0x12ec) == temp_r0) {
                var_r6 = 0;
            } else {
                return;
            }
        }
        offset = 0x12f0 + var_r6 * 2;
        spC = *(u16 *)(temp_r7 + offset);
        Func_0801a4fc(temp_r0, arg2, &spC, &sp8, var_r6 + 0xe, 1);
    }
}
