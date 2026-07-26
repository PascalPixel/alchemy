#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_08185000();

void Func_0800b9f4(void *arg0, s32 arg1) {
    s32 temp_r7;
    void *temp_r0;
    s32 temp_r2;

    temp_r7 = 0x80 & arg1;
    if (M2C_FIELD(arg0, s32, 0xC) != 0) {
        temp_r0 = Func_08185000((s32) M2C_FIELD(arg0, s16, 0));
        if (arg1 < (s32) M2C_FIELD(temp_r0, u8, 5)) {
            temp_r2 = *(s32 *)((u8 *)M2C_FIELD(arg0, s32, 0xC) + (arg1 * 4));
            M2C_FIELD(arg0, u8, 4) = (u8) M2C_FIELD(temp_r0, u8, 4);
            M2C_FIELD(arg0, s32, 0x10) = temp_r2;
            M2C_FIELD(arg0, s8, 0x15) = 0x10;
            if (temp_r7 == 0) {
                M2C_FIELD(arg0, s8, 0x14) = temp_r7;
                M2C_FIELD(arg0, s16, 2) = (s16) temp_r7;
            }
        }
    }
}
