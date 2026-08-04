#include "types.h"
#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_02002282(s32, s32);

s32 Func_020000d8(void *arg0) {
    s32 var_r0;
    u16 temp_r3;
    s32 temp;
    void *temp_r1;
    temp_r1 = FIELD(arg0, void *, 0x68);
    if (temp_r1 != NULL) {
        FIELD(arg0, u8, 0x5A) = (u8)(0xFE & FIELD(arg0, u8, 0x5A));
        temp = (u16)Func_02002282(FIELD(temp_r1, s32, 0x10) - FIELD(arg0, s32, 0x10), FIELD(temp_r1, s32, 8) - FIELD(arg0, s32, 8));
        temp_r3 = FIELD(arg0, u16, 6);
        var_r0 = (s16)(temp - temp_r3);
        if (var_r0 != 0) {
            if (var_r0 > 0x1000) var_r0 = 0x1000;
            if (var_r0 < (s32)0xF896F001) var_r0 = (s32)0xF896F001;
            FIELD(arg0, u16, 6) = (u16)(temp_r3 + var_r0);
        }
    }
    return 1;
}
