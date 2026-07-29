typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_08004458(void);
s32 Func_080091a8(s32, s32, s32);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);

/* Runs the staged arrival, particle-burst, and departure presentation. */
void Func_080999f0(void) {
    s32 sp0;
    s32 *sp4;
    s32 sp8[3];
    s32 sp20[3];
    s32 start_pos[3];
    s32 temp_r0_2;
    s32 temp_r0_5;
    s32 temp_r2;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r5_7;
    s32 temp_r5_8;
    s32 var_r8;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r8_4;
    s32 var_r9;
    s32 var_r9_2;
    void *temp_r0;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r1;
    void *temp_r6;
    void *temp_sl;

    temp_r6 = *(void **)0x03001F30;
    temp_sl = M2C_FIELD(temp_r6, void **, 0x10);
    var_r8 = 0;
    temp_r0 = Func_08096c80(0xEF, 0, 0, 0);
    if (temp_r0 == 0) {
        return;
    }
    Func_08097384();
    Func_080f9010(0x8A);
    if (M2C_FIELD(temp_r6, s32 *, 0x14) == 0) {
        M2C_FIELD(temp_r6, s32 *, 4) = (s32) M2C_FIELD(temp_sl, s32 *, 8);
        M2C_FIELD(temp_r6, s32 *, 0xC) = (s32) M2C_FIELD(temp_sl, s32 *, 0x10);
        Func_0800447c(0x100000, M2C_FIELD(temp_r6, s32 *, 0), temp_r6 + 4);
        M2C_FIELD(temp_r6, s32 *, 8) = Func_080091a8(0, M2C_FIELD(temp_r6, s32 *, 4), M2C_FIELD(temp_r6, s32 *, 0xC));
    }
    sp4 = start_pos;
    M2C_FIELD(sp4, s32 *, 0) = M2C_FIELD(temp_sl, s32 *, 8);
    M2C_FIELD(sp4, s32 *, 4) = (s32) (M2C_FIELD(temp_sl, s32 *, 0xC) + 0x100000);
    M2C_FIELD(sp4, s32 *, 8) = (s32) M2C_FIELD(temp_sl, s32 *, 0x10);
    M2C_FIELD(&sp8, s32 *, 0) = (s32) M2C_FIELD(temp_r6, s32 *, 4);
    temp_r2 = M2C_FIELD(temp_r6, s32 *, 8);
    M2C_FIELD(&sp8, s32 *, 4) = (s32) (temp_r2 + 0x200000);
    M2C_FIELD(&sp8, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0xC);
    if ((s8) M2C_FIELD(temp_r6, u8 *, 0x34) != 0) {
        M2C_FIELD(&sp8, s32 *, 4) = (s32) (temp_r2 + 0x500000);
    }
    do {
        temp_r5 = M2C_FIELD(sp4, s32 *, 0);
        M2C_FIELD(temp_r0, s32 *, 8) = (s32) (temp_r5 + Func_080022ec(var_r8 * (M2C_FIELD(&sp8, s32 *, 0) - temp_r5), 0xA));
        temp_r5_2 = M2C_FIELD(sp4, s32 *, 4);
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (temp_r5_2 + Func_080022ec(var_r8 * (M2C_FIELD(&sp8, s32 *, 4) - temp_r5_2), 0xA));
        temp_r5_3 = M2C_FIELD(sp4, s32 *, 8);
        M2C_FIELD(temp_r0, s32 *, 0x10) = (s32) (temp_r5_3 + Func_080022ec(var_r8 * (M2C_FIELD(&sp8, s32 *, 8) - temp_r5_3), 0xA));
        temp_r0_2 = Func_080022ec(var_r8 * 0xC000, 0xA) + 0x4000;
        M2C_FIELD(temp_r0, s32 *, 0x18) = temp_r0_2;
        M2C_FIELD(temp_r0, s32 *, 0x1C) = temp_r0_2;
        Func_080030f8(1U);
        var_r8 += 1;
    } while (var_r8 < 0xB);
    Func_080030f8(0xAU);
    if ((s8) M2C_FIELD(temp_r6, u8 *, 0x45) == 0) {
        var_r9 = 0xA;
        if ((s8) M2C_FIELD(temp_r6, u8 *, 0x20) == 0) {
            var_r9 = 0x18;
        }
        var_r8_2 = 0;
        if (var_r9 > 0) {
            sp0 = var_r9 - 1;
            do {
                M2C_FIELD(&sp20, s32 *, 0) = M2C_FIELD(temp_r0, s32 *, 8);
                M2C_FIELD(&sp20, s32 *, 4) = (s32) M2C_FIELD(temp_r0, s32 *, 0xC);
                M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r0, s32 *, 0x10);
                temp_r5_4 = (Func_08004458() * 5) + 0x30000;
                Func_0800447c(temp_r5_4, (s32) Func_08004458(), &sp20);
                if (var_r8_2 == sp0) {
                    Func_080030f8(0x19U);
                    M2C_FIELD(&sp20, s32 *, 0) = M2C_FIELD(temp_r0, s32 *, 8);
                    M2C_FIELD(&sp20, s32 *, 4) = (s32) M2C_FIELD(temp_r0, s32 *, 0xC);
                    M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r0, s32 *, 0x10);
                }
                temp_r0_3 = Func_08096c80(0xF0, M2C_FIELD(&sp20, s32 *, 0), M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
                if (temp_r0_3 != 0) {
                    M2C_FIELD(temp_r0_3, s32 *, 0x14) = (s32) (M2C_FIELD(&sp20, s32 *, 4) + 0xFFE00000);
                    M2C_FIELD(temp_r0_3, s32 *, 0x6C) = 0x08099921;
                    M2C_FIELD(temp_r0_3, s8 *, 0x55) = 2;
                }
                Func_080f9010(0x84);
                Func_080030f8(6U);
                var_r8_2 += 1;
            } while (var_r8_2 < var_r9);
        }
        Func_080030f8(0xAU);
    } else {
        var_r9_2 = 0xA;
        if ((s8) M2C_FIELD(temp_r6, u8 *, 0x20) == 0) {
            var_r9_2 = 0x1E;
        }
        if (var_r9_2 != 0) {
            var_r8_3 = var_r9_2;
            do {
                M2C_FIELD(&sp20, s32 *, 0) = M2C_FIELD(temp_r0, s32 *, 8);
                M2C_FIELD(&sp20, s32 *, 4) = (s32) M2C_FIELD(temp_r0, s32 *, 0xC);
                M2C_FIELD(&sp20, s32 *, 8) = (s32) M2C_FIELD(temp_r0, s32 *, 0x10);
                temp_r5_5 = (Func_08004458() * 5) + 0x30000;
                Func_0800447c(temp_r5_5, (s32) Func_08004458(), &sp20);
                temp_r0_4 = Func_08096c80(0x11C, M2C_FIELD(&sp20, s32 *, 0), M2C_FIELD(&sp20, s32 *, 4), M2C_FIELD(&sp20, s32 *, 8));
                if (temp_r0_4 != 0) {
                    M2C_FIELD(temp_r0_4, s32 *, 0x6C) = 0x080999A9;
                    M2C_FIELD(temp_r0_4, s8 *, 0x55) = 0;
                    temp_r1 = M2C_FIELD(temp_r0_4, void **, 0x50);
                    M2C_FIELD(temp_r1, u8 *, 9) = (u8) ((M2C_FIELD(temp_r1, u8 *, 9) & ~0xC) | 8);
                    Func_08009080((s32) temp_r0_4, 8);
                    Func_08009240((s32) temp_r0_4, 7);
                }
                Func_080030f8(6U);
                var_r8_3 -= 1;
            } while (var_r8_3 != 0);
        }
        Func_080030f8(0x46U);
    }
    var_r8_4 = 0;
    do {
        temp_r5_6 = M2C_FIELD(&sp8, s32 *, 0);
        M2C_FIELD(temp_r0, s32 *, 8) = (s32) (temp_r5_6 + Func_080022ec(var_r8_4 * (M2C_FIELD(sp4, s32 *, 0) - temp_r5_6), 0xA));
        temp_r5_7 = M2C_FIELD(&sp8, s32 *, 4);
        M2C_FIELD(temp_r0, s32 *, 0xC) = (s32) (temp_r5_7 + Func_080022ec(var_r8_4 * (M2C_FIELD(sp4, s32 *, 4) - temp_r5_7), 0xA));
        temp_r5_8 = M2C_FIELD(&sp8, s32 *, 8);
        M2C_FIELD(temp_r0, s32 *, 0x10) = (s32) (temp_r5_8 + Func_080022ec(var_r8_4 * (M2C_FIELD(sp4, s32 *, 8) - temp_r5_8), 0xA));
        temp_r0_5 = Func_080022ec(var_r8_4 * 0xFFFF4000, 0xA) + 0x10000;
        M2C_FIELD(temp_r0, s32 *, 0x18) = temp_r0_5;
        M2C_FIELD(temp_r0, s32 *, 0x1C) = temp_r0_5;
        Func_080030f8(1U);
        var_r8_4 += 1;
    } while (var_r8_4 < 0xB);
    Func_080090d0(temp_r0);
    Func_0809748c();
}
