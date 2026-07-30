#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_0801179c(void) {
    s32 temp_r2_2;
    s32 var_r0;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r2;
    u16 temp_r0;
    u16 temp_r1;
    u16 temp_r2;
    u16 temp_r2_3;
    u32 var_sl;
    void *temp_r3;
    void *temp_r4;
    void *temp_r4_2;
    void *var_r5;

    temp_r3 = *(void **)0x03001E70;
    var_sl = 0;
    var_r5 = temp_r3 + 0x18;
    do {
        if ((M2C_FIELD(var_r5, void **, 0) != NULL) && (M2C_FIELD(var_r5, u16 *, 0xA) == 0)) {
loop_3:
            temp_r2 = M2C_FIELD(var_r5, u16 *, 8);
            if (temp_r2 == 0) {
                temp_r4 = M2C_FIELD(var_r5, void **, 4);
                temp_r0 = M2C_FIELD(temp_r4, u16 *, 0);
                temp_r4_2 = temp_r4 + 2;
                if (temp_r0 == 0xFFFF) {
                    M2C_FIELD(var_r5, void **, 4) = (void *) M2C_FIELD(var_r5, void **, 0);
                    goto loop_3;
                }
                if ((0xFF00 & temp_r0) == 0xFE00) {
                    temp_r2_2 = 0xFF & temp_r0;
                    if (temp_r2_2 != 0xFF) {
                        M2C_FIELD(var_r5, void **, 4) = (void *) (M2C_FIELD(var_r5, void **, 0) + (temp_r2_2 * 4));
                        goto loop_3;
                    }
                } else {
                    temp_r2_3 = M2C_FIELD(temp_r4, u16 *, 2);
                    temp_r1 = M2C_FIELD(temp_r4_2, u16 *, 2);
                    M2C_FIELD(var_r5, u16 *, 8) = (u16) M2C_FIELD((temp_r4_2 + 2), u16 *, 2);
                    if (M2C_FIELD(temp_r3, u8 *, 0x16) == 0) {
                        if ((u32) temp_r0 >= 0x600U) {
                            var_r2 = temp_r2_3 * 8;
                            var_r0 = (temp_r0 << 5) + 0x0201C000;
                            var_r1 = (temp_r1 << 5) + 0x06004000;
                        } else {
                            var_r2 = temp_r2_3 * 8;
                            var_r0 = (temp_r0 << 5) + 0x06004000;
                            var_r1 = (temp_r1 << 5) + 0x06004000;
                        }
                    } else {
                        if ((u32) temp_r0 >= 0x200U) {
                            var_r2 = temp_r2_3 * 0x10;
                            var_r1_2 = temp_r1 << 6;
                            var_r0 = (temp_r0 << 6) + 0x02020000;
                        } else {
                            var_r2 = temp_r2_3 * 0x10;
                            var_r1_2 = temp_r1 << 6;
                            var_r0 = (temp_r0 << 6) + 0x06008000;
                        }
                        var_r1 = var_r1_2 + 0x06008000;
                    }
                    M2C_FIELD((void *)0x040000D4, s32 *, 0) = var_r0;
                    M2C_FIELD((void *)0x040000D4, s32 *, 4) = var_r1;
                    M2C_FIELD((void *)0x040000D4, s32 *, 8) = (s32) (var_r2 | 0x84000000);
                    M2C_FIELD(var_r5, void **, 4) = (void *) (M2C_FIELD(var_r5, void **, 4) + 8);
                    goto loop_3;
                }
            } else {
                M2C_FIELD(var_r5, u16 *, 8) = (u16) (temp_r2 + 0xFFFF);
            }
        }
        var_sl += 1;
        var_r5 += 0xC;
    } while (var_sl <= 0xFU);
}
