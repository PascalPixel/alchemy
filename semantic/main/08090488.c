#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_08090488(void) {
    s16 *var_r2;
    s16 var_r3;
    s32 temp_r2_2;
    s8 temp_r2;
    s8 temp_r3;
    u16 temp_r5;
    void *temp_r6;
    void *temp_r7;

    temp_r6 = *(void **)0x03001ECC;
    temp_r2 = M2C_FIELD(temp_r6, s8 *, 0x53C);
    temp_r7 = *(void **)0x03001E70;
    if (temp_r2 != 0) {
        if ((s32) M2C_FIELD(temp_r6, s8 *, 0x53D) >= (s32) temp_r2) {
            M2C_FIELD(temp_r6, s8 *, 0x53C) = 0;
            Func_08004278((void *)0x08090489);
            Func_0800307c(1, 0, 0);
            return;
        }
        temp_r2_2 = M2C_FIELD(temp_r6, s8 *, 0x53B) - M2C_FIELD(temp_r6, s8 *, 0x53A);
        temp_r3 = (u8) M2C_FIELD(temp_r6, s8 *, 0x53D) + 1;
        M2C_FIELD(temp_r6, s8 *, 0x53D) = temp_r3;
        M2C_FIELD(temp_r6, u16 *, 0x52A) = (u16) (M2C_FIELD(temp_r6, s8 *, 0x53A) + Func_080072f0(temp_r3 * temp_r2_2, (s32) M2C_FIELD(temp_r6, s8 *, 0x53C), temp_r2_2, 0x03000380));
        goto block_4;
    }
block_4:
    temp_r5 = M2C_FIELD(temp_r6, u16 *, 0x52A);
    if ((u32) temp_r5 > 0x4FU) {
        M2C_FIELD(temp_r7, s16 *, 0x100) = 0xC8;
        var_r2 = temp_r7 + 0x102;
        var_r3 = 0xFA;
        goto block_10;
    }
    if ((temp_r5 != 0) && (*(s32 *)0x03001E40 & 1)) {
        M2C_FIELD(temp_r7, s16 *, 0x100) = (s16) (temp_r5 + 0x50);
        M2C_FIELD(temp_r7, s16 *, 0x102) = (s16) (0x50 - temp_r5);
        return;
    }
    M2C_FIELD(temp_r7, s16 *, 0x100) = 0;
    var_r2 = temp_r7 + 0x102;
    var_r3 = 0x9F;
block_10:
    *var_r2 = var_r3;
}
