typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
void Func_08004760(void);
void Func_0800479c(void);
void Func_080052f4(s32, s32);

/*
 * Interactive palette editor. Draw the selected palette as an RGB grid,
 * navigate its bank/index/component, edit one five-bit channel at a time,
 * and flash-preview the selected colour until the user exits.
 */
void Func_0808d0c8(void) {
    s32 sp0;
    s32 *sp4;
    s32 sp8;
    s16 *var_r7_2;
    s16 var_r0;
    s32 *var_r1_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 var_fp;
    s32 var_r1_3;
    s32 var_r1_4;
    s32 var_r3;
    s32 var_r4;
    s32 var_r8;
    s32 var_r9;
    s32 var_sl;
    u16 *var_r7;
    u16 temp_r6;
    u16 temp_r6_2;
    u16 temp_r6_3;
    u16 temp_r6_4;
    u32 var_ip;
    u32 var_r2;
    u32 var_r3_2;
    u32 var_r5;
    u32 var_r5_2;
    void *temp_r2;
    void *temp_r2_2;
    void *var_r1;

    var_r9 = 0;
    var_sl = 1;
    var_r8 = 1;
    Func_080052f4(0x0809E4CE, 0x06001A00);
    sp8 = 0;
    var_fp = 0;
loop_5:
    M2C_FIELD((void *)0x0600205A, s16 *, 0) = (s16) (var_r9 + 0xFFFFF0E0);
    temp_r2 = (void *)0x0600205A + 0x40;
    M2C_FIELD((void *)0x0600205A, s16 *, 0x40) = 0xF052;
    M2C_FIELD(temp_r2, s16 *, 0x40) = 0xF047;
    M2C_FIELD((temp_r2 + 0x40), s16 *, 0x40) = 0xF042;
    var_r0 = sp8 + 0xD1;
    var_r7 = (u16 *)(var_fp + 0x05000002);
    var_r1 = (void *)0x0600205A + 2;
    var_ip = 1;
    do {
        M2C_FIELD(var_r1, s16 *, 0) = var_r0;
        temp_r6 = *var_r7;
        temp_r2_2 = var_r1 + 0x40;
        M2C_FIELD(var_r1, s16 *, 0x40) = (s16) ((temp_r6 & 0x1F) + 0xF0E0);
        M2C_FIELD(temp_r2_2, s16 *, 0x40) = (s16) (((temp_r6 >> 5) & 0x1F) + 0xF0E0);
        M2C_FIELD((temp_r2_2 + 0x40), s16 *, 0x40) = (s16) (((temp_r6 >> 0xA) & 0x1F) + 0xF0E0);
        var_ip += 1;
        var_r0 += 1;
        var_r7 += 1;
        var_r1 += 2;
    } while (var_ip <= 0xFU);
    Func_080030f8(1U);
    var_r1_2 = (s32 *)0x03001B04;
    var_r4 = 0x1F;
loop_8:
    if (*var_r1_2 & 0x40) {
        var_sl -= 1;
        if (var_sl <= 0) {
            var_sl = 3;
        }
    }
    if (*var_r1_2 & 0x80) {
        var_sl += 1;
        if (var_sl > 3) {
            var_sl = 1;
        }
    }
    if (*var_r1_2 & 0x20) {
        var_r8 -= 1;
        if (var_r8 <= 0) {
            var_r8 = 0xF;
        }
    }
    if (*var_r1_2 & 0x10) {
        var_r8 += 1;
        if (var_r8 > 0xF) {
            var_r8 = 1;
        }
    }
    if (*var_r1_2 & 0x200) {
        var_r9 -= 1;
        if (var_r9 >= 0) {
            sp8 = var_r9 << 0xC;
            var_fp = var_r9 << 5;
        } else {
            var_r9 = 0xD;
            var_r1_3 = 0xD;
            var_r3 = 0xD000;
block_3:
            var_r1_4 = var_r1_3 << 5;
            sp8 = var_r3;
block_4:
            var_fp = var_r1_4;
        }
        goto loop_5;
    }
    if (*var_r1_2 & 0x100) {
        var_r9 += 1;
        if (var_r9 <= 0xD) {
            var_r3 = var_r9 << 0xC;
            var_r1_3 = var_r9;
            goto block_3;
        }
        var_r1_4 = 0;
        var_r9 = 0;
        sp8 = 0;
        goto block_4;
    }
    if (*var_r1_2 & 1) {
        temp_r3 = ((var_r9 * 0x10) + var_r8) * 2;
        var_r7_2 = (s16 *)(temp_r3 + 0x05000000);
        temp_r6_2 = M2C_FIELD(temp_r3, u16 *, 0x05000000);
        var_r5 = temp_r6_2 & var_r4;
        var_r2 = (temp_r6_2 >> 5) & var_r4;
        var_r3_2 = (temp_r6_2 >> 0xA) & var_r4;
        if ((var_sl == 1) && (var_r5 <= 0x1EU)) {
            var_r5 += 1;
        }
        if ((var_sl == 2) && (var_r2 <= 0x1EU)) {
            var_r2 += 1;
        }
        if ((var_sl == 3) && (var_r3_2 <= 0x1EU)) {
            var_r3_2 += 1;
        }
block_48:
        *var_r7_2 = (var_r3_2 << 0xA) | (var_r2 << 5) | var_r5;
        goto loop_5;
    }
    if (*var_r1_2 & 2) {
        temp_r3_2 = ((var_r9 * 0x10) + var_r8) * 2;
        var_r7_2 = (s16 *)(temp_r3_2 + 0x05000000);
        temp_r6_3 = M2C_FIELD(temp_r3_2, u16 *, 0x05000000);
        var_r5 = temp_r6_3 & var_r4;
        var_r2 = (temp_r6_3 >> 5) & var_r4;
        var_r3_2 = (temp_r6_3 >> 0xA) & var_r4;
        if ((var_sl == 1) && (var_r5 != 0)) {
            var_r5 -= 1;
        }
        if ((var_sl == 2) && (var_r2 != 0)) {
            var_r2 -= 1;
        }
        if ((var_sl == 3) && (var_r3_2 != 0)) {
            var_r3_2 -= 1;
        }
        goto block_48;
    }
    if (*var_r1_2 & 8) {
        temp_r3_3 = ((var_r9 * 0x10) + var_r8) * 2;
        temp_r6_4 = M2C_FIELD(temp_r3_3, u16 *, 0x05000000);
loop_61:
        var_r5_2 = 0;
loop_62:
        sp4 = var_r1_2;
        sp0 = var_r4;
        Func_080030f8(1U);
        if (*(s32 *)0x03001AE8 & 8) {
            if (var_r5_2 == 0) {
                M2C_FIELD(temp_r3_3, u16 *, 0x05000000) = 0x7FFFU;
            }
            if (var_r5_2 == 0xA) {
                M2C_FIELD(temp_r3_3, u16 *, 0x05000000) = temp_r6_4;
            }
            if (var_r5_2 == 0x14) {
                M2C_FIELD(temp_r3_3, u16 *, 0x05000000) = 0U;
            }
            if (var_r5_2 == 0x1E) {
                M2C_FIELD(temp_r3_3, u16 *, 0x05000000) = temp_r6_4;
            }
            var_r5_2 += 1;
            if (var_r5_2 > 0x27U) {
                goto loop_61;
            }
            goto loop_62;
        }
        M2C_FIELD(temp_r3_3, u16 *, 0x05000000) = temp_r6_4;
    }
    if (!(*var_r1_2 & 4)) {
        sp4 = var_r1_2;
        sp0 = var_r4;
        Func_080030f8(1U);
        goto loop_8;
    }
    Func_0800479c();
    Func_08004760();
}
