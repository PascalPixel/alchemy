typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);

/* Runs the approach, impact, displacement, and return object presentation. */
void Func_0809ae64(void) {
    void *sp0;
    s32 sp4[3];
    s32 sp10[3];
    s32 sp1C[3];
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r5_7;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r7;
    s32 var_r7_2;
    void *temp_r0;
    void *temp_r5;
    void *temp_r7;

    temp_r5 = *(void **)0x03001F30;
    temp_r7 = M2C_FIELD(temp_r5, void **, 0x10);
    sp0 = M2C_FIELD(temp_r5, void **, 0x14);
    M2C_FIELD(&sp10, s32 *, 0) = (s32) M2C_FIELD(temp_r7, s32 *, 8);
    M2C_FIELD(&sp10, s32 *, 4) = (s32) (M2C_FIELD(temp_r7, s32 *, 0xC) + 0x100000);
    M2C_FIELD(&sp10, s32 *, 8) = (s32) M2C_FIELD(temp_r7, s32 *, 0x10);
    if ((s8) M2C_FIELD(temp_r5, u8 *, 0x20) != 0) {
        M2C_FIELD(&sp4, s32 *, 0) = M2C_FIELD(temp_r7, s32 *, 8);
        M2C_FIELD(&sp4, s32 *, 4) = (s32) (M2C_FIELD(temp_r7, s32 *, 0xC) + 0x200000);
        M2C_FIELD(&sp4, s32 *, 8) = (s32) M2C_FIELD(temp_r7, s32 *, 0x10);
        Func_0800447c(0x200000, M2C_FIELD(temp_r5, s32 *, 0), &sp4);
    } else {
        M2C_FIELD(&sp4, s32 *, 0) = M2C_FIELD(temp_r5, s32 *, 4);
        M2C_FIELD(&sp4, s32 *, 4) = (s32) (M2C_FIELD(temp_r5, s32 *, 8) + 0x200000);
        M2C_FIELD(&sp4, s32 *, 8) = (s32) M2C_FIELD(temp_r5, s32 *, 0xC);
    }
    temp_r1 = M2C_FIELD(temp_r5, s32 *, 4);
    M2C_FIELD(&sp1C, s32 *, 0) = temp_r1;
    temp_r2 = M2C_FIELD(temp_r5, s32 *, 8) + 0x200000;
    M2C_FIELD(&sp1C, s32 *, 4) = temp_r2;
    temp_r3 = M2C_FIELD(temp_r5, s32 *, 0xC);
    M2C_FIELD(&sp1C, s32 *, 8) = temp_r3;
    temp_r0 = Func_08096c80(0xD7, temp_r1, temp_r2, temp_r3);
    if (temp_r0 == 0) {
        return;
    }
    Func_08097384();
    Func_080f9010(0x8A);
    M2C_FIELD(temp_r0, u16 *, 6) = (u16) M2C_FIELD(temp_r7, u16 *, 6);
    M2C_FIELD(temp_r0, s32 *, 0x30) = 0x14CCC;
    M2C_FIELD(temp_r0, s8 *, 0x55) = 0;
    Func_08009080((s32) temp_r0, 5);
    Func_08009240((s32) temp_r0, 1);
    var_r7 = 0;
    do {
        temp_r5_2 = M2C_FIELD(&sp10, s32 *, 0);
        M2C_FIELD(temp_r0, s32 *, 8) = (s32) (temp_r5_2 + Func_080022ec(var_r7 * (M2C_FIELD(&sp4, s32 *, 0) - temp_r5_2), 0xA));
        temp_r5_3 = M2C_FIELD(&sp10, s32 *, 4);
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (temp_r5_3 + Func_080022ec(var_r7 * (M2C_FIELD(&sp4, s32 *, 4) - temp_r5_3), 0xA));
        temp_r5_4 = M2C_FIELD(&sp10, s32 *, 8);
        M2C_FIELD(temp_r0, s32 *, 0x10) = (s32) (temp_r5_4 + Func_080022ec(var_r7 * (M2C_FIELD(&sp4, s32 *, 8) - temp_r5_4), 0xA));
        temp_r0_2 = Func_080022ec(var_r7 * 0xC000, 0xA) + 0x4000;
        M2C_FIELD(temp_r0, s32 *, 0x18) = temp_r0_2;
        M2C_FIELD(temp_r0, s32 *, 0x1C) = temp_r0_2;
        var_r7 += 1;
        Func_080030f8(1U);
    } while (var_r7 < 0xB);
    Func_080030f8(0xAU);
    Func_08009080((s32) temp_r0, 6);
    Func_080030f8(0xFU);
    var_r5 = 9;
    do {
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r0, s32 *, 0xC) + 0xFFFE0000);
        var_r5 -= 1;
        Func_080030f8(1U);
    } while (var_r5 >= 0);
    Func_08009080((s32) temp_r0, 5);
    Func_080f9010(0x84);
    if (sp0 != 0) {
        Func_080090f0(sp0, -0x90000, M2C_FIELD(sp0, s32 *, 0xC), -0x90000);
    }
    Func_080030f8(0x14U);
    var_r5_2 = 0xC;
    do {
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r0, s32 *, 0xC) + 0x18000);
        var_r5_2 -= 1;
        Func_080030f8(1U);
    } while (var_r5_2 >= 0);
    Func_080030f8(0xAU);
    Func_080f9010(0x72);
    var_r7_2 = 0;
    do {
        temp_r5_5 = M2C_FIELD(&sp4, s32 *, 0);
        M2C_FIELD(temp_r0, s32 *, 8) = (s32) (temp_r5_5 + Func_080022ec(var_r7_2 * (M2C_FIELD(&sp10, s32 *, 0) - temp_r5_5), 0xA));
        temp_r5_6 = M2C_FIELD(&sp4, s32 *, 4);
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (temp_r5_6 + Func_080022ec(var_r7_2 * (M2C_FIELD(&sp10, s32 *, 4) - temp_r5_6), 0xA));
        temp_r5_7 = M2C_FIELD(&sp4, s32 *, 8);
        M2C_FIELD(temp_r0, s32 *, 0x10) = (s32) (temp_r5_7 + Func_080022ec(var_r7_2 * (M2C_FIELD(&sp10, s32 *, 8) - temp_r5_7), 0xA));
        temp_r0_3 = Func_080022ec(var_r7_2 * 0xFFFF4000, 0xA) + 0x10000;
        M2C_FIELD(temp_r0, s32 *, 0x18) = temp_r0_3;
        M2C_FIELD(temp_r0, s32 *, 0x1C) = temp_r0_3;
        var_r7_2 += 1;
        Func_080030f8(1U);
    } while (var_r7_2 < 0xB);
    Func_080090d0(temp_r0);
    Func_0809748c();
}
