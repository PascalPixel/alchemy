#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_08015270(s32);
void Func_080030f8(u32);
void Func_08015080(s32, s32, s32, s32);
void Func_080a2268(void *, s32, s32, s32, s32, u32);

s32 Func_080a6a98(s32 arg0, s32 arg1, void *arg2)
{
    s32 var_r5;
    s32 var_r6;
    s32 temp_sl;
    u16 temp_r2;
    u16 *temp_r7;

    temp_r7 = *(u16 **)0x03001F2C;
    M2C_FIELD(arg2, s32, 0x18) =
        (M2C_FIELD(arg2, s32, 8) * 5) + M2C_FIELD(arg2, s32, 0x10);
    if (Func_080770c0(0x151) == 0) {
        Func_08015270(M2C_FIELD(temp_r7, s32, 0x2C));
        Func_080030f8(1U);
        temp_r2 = temp_r7[M2C_FIELD(arg2, s32, 0x18) + 0xE4];
        if (temp_r2 != 0) {
            Func_08015080((0x1FF & temp_r2) + 0x53A,
                          M2C_FIELD(temp_r7, s32, 0x2C), 0, 0);
        }
    } else {
        Func_080770d0(0x2FF);
    }
    temp_sl = 1;
    var_r6 = 0;
    var_r5 = 1;
    do {
        if (var_r6 == M2C_FIELD(arg2, s32, 0x10)) {
            Func_080a2268(M2C_FIELD(temp_r7, void *, 0x20), 0,
                          var_r5, 0xF, temp_sl, 0xEU);
        } else {
            Func_080a2268(M2C_FIELD(temp_r7, void *, 0x20), 0,
                          var_r5, 0xF, temp_sl, 0xFU);
        }
        var_r6 += 1;
        var_r5 += 2;
    } while (var_r6 <= 4);
    Func_080030f8(1U);
    return 1;
}
