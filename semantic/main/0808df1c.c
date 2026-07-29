typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Func_0808df1c(u32 arg0, s32 arg1) {
    u32 sp4;
    s32 sp0;
    u32 sp8;
    s32 var_r0_3;
    s32 temp_fp;
    s32 temp_r0;
    u8 *target;
    u8 *temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 sum_a;
    s32 sum_b;
    s32 sum;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r5;
    u32 temp_r0_4;
    u32 var_r9;

    sp8 = arg0;
    sp4 = -1U;
    sp0 = Func_0808ddb8(arg1);
    target = Func_0808ba1c(arg0);
    if (target == 0) {
    } else {
        temp_fp = (M2C_FIELD(target, u16 *, 6) + 0x2000) & 0xC000;
        var_r9 = 0;
        do {
            if (var_r9 != sp8) {
                temp_r0_2 = Func_0808ba1c(var_r9);
                if ((temp_r0_2 != 0) && !(8 & M2C_FIELD(temp_r0_2, u8 *, 0x59))) {
                    var_r0 = 0x80000;
                    if (arg1 == 0xD) {
                        var_r0 = 0x300000;
                    }
                    if (arg1 == 5) {
                        var_r0 = 0x400000;
                    }
                    if (arg1 == 2) {
                        var_r0 = 0x100000;
                    }
                    temp_r1 = M2C_FIELD(temp_r0_2, s32 *, 0xC);
                    temp_r3 = M2C_FIELD(target, s32 *, 0xC);
                    temp_r2 = temp_r1 - temp_r3;
                    if (temp_r2 >= 0) {
                        if (temp_r2 > var_r0) {
                        } else {
                            goto block_16;
                        }
                    } else if ((s32)(temp_r3 - temp_r1) <= var_r0) {
block_16:
                        var_r0_2 = M2C_FIELD(temp_r0_2, s32 *, 8) - M2C_FIELD(target, s32 *, 8);
                        if (var_r0_2 < 0) {
                            var_r0_2 += 0xFFFF;
                        }
                        var_r2 = M2C_FIELD(temp_r0_2, s32 *, 0x10) - M2C_FIELD(target, s32 *, 0x10);
                        temp_r0_3 = var_r0_2 >> 0x10;
                        if (var_r2 < 0) {
                            var_r2 += 0xFFFF;
                        }
                        temp_r3_2 = var_r2 >> 0x10;
                        temp_r1_2 = temp_r3_2 * temp_r3_2;
                        temp_r2_2 = temp_r0_3 * temp_r0_3;
                        sum_a = temp_r1_2;
                        sum_b = temp_r2_2;
                        sum = sum_a;
                        sum += sum_b;
                        var_r5 = Func_080072f0(sum, sum_a, sum_b, 0x030001D8);
                        if (0x10 & M2C_FIELD(temp_r0_2, u8 *, 0x59)) {
                            var_r5 = Func_080022ec(var_r5 * 2, 3);
                        }
                        if (var_r5 < sp0) {
                            temp_r0_4 = (u16)Func_080044d0(M2C_FIELD(temp_r0_2, s32 *, 0x10) - M2C_FIELD(target, s32 *, 0x10), M2C_FIELD(temp_r0_2, s32 *, 8) - M2C_FIELD(target, s32 *, 8));
                            var_r2_2 = 0x1800;
                            if (var_r5 > 0x13) {
                                var_r2_2 = 0x1000;
                            }
                            if (arg1 == 2) {
                                var_r2_2 = 0x2000;
                            }
                            if (var_r5 > 0xB) {
                                var_r0_3 = (s16)(temp_r0_4 - temp_fp);
                                if ((s32)var_r0_3 < 0) {
                                    var_r0_3 = 0 - var_r0_3;
                                }
                                if ((s32)var_r0_3 < var_r2_2) {
                                    goto block_31;
                                }
                            } else {
block_31:
                                sp4 = var_r9;
                                sp0 = var_r5;
                            }
                        }
                    }
                }
            }
            var_r9 += 1;
        } while ((s32)var_r9 <= 0x42);
    }
    return sp4;
}
