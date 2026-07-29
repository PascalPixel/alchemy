typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
void Func_080049e8(void);
void Func_08004a5c(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(void *);
void Func_080072f4(s32, void *, s32, s32, u32, s32);
s32 Func_080072f8();
void **Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080d6888(s32, s32, s32, s32, s32);

/*
 * Configure and run the shared projectile-orbit battle effect.
 *
 * The mode selects palette, duration, and target coordinates. Forty particles
 * interpolate toward the target, orbit through projected space, then seed a
 * second burst before the routine restores display state.
 */
s32 Func_080de2f8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5) {
    s32 sp8;
    void *spC;
    void *sp10;
    s32 sp14;
    void *sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    void *sp28;
    s32 sp2C;
    s32 sp30[3];
    s32 sp3C[3];
    s32 sp54[3];
    s32 sp60[3];
    s32 interpolation_delta[3];
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_5;
    s32 temp_r0_7;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r4_3;
    s32 temp_r4_4;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 var_r0;
    s32 var_r2;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r5;
    s32 var_r7_2;
    s32 var_r8;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r8_4;
    s32 var_r9;
    s32 var_sl;
    u16 temp_r6;
    u16 temp_r6_2;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_6;
    u32 temp_r0_8;
    void *temp_r1;
    void *temp_r9;
    void *var_r5_2;
    void *var_r6;
    void *var_r7;
    void *var_r7_3;

    sp2C = arg3;
    var_r5 = arg1;
    temp_r1 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp28 = temp_r1;
    sp24 = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp18 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    M2C_FIELD(temp_r1, void **, 0x7828) = arg0;
    Func_080cd594(0);
    M2C_FIELD(sp28, s32 *, 0x77B4) = 0x18;
    M2C_FIELD(sp28, s32 *, 0x77B8) = 0;
    if (var_r5 > 3) {
        var_r5 -= 4;
        sp14 = 0x54;
    } else {
        sp14 = 0x40;
    }
    switch (var_r5) {
    case 0:
        var_r0 = 0x94;
        break;
    case 1:
        var_r0 = 0x92;
        break;
    case 2:
        var_r0 = 0x8E;
        break;
    default:
        var_r0 = 0x90;
        break;
    }
    temp_r0_2 = Func_08002f40(var_r0);
    Func_080072f0(0x05000000, temp_r0_2, 0x80, 0x03001388);
    Func_08005340(temp_r0_2 + 0x80, (s32) sp28);
    Func_080e0524((void *)0x73, sp18, 0, 0);
    if (arg2 == 1) {
        Func_080ed408(0x2E, 7, 7, 7, 3);
        Func_080ed408(0x2F, 7, 7, 7, 2);
    } else {
        Func_080ed408(0x2E, 7, 7, 3, 3);
        Func_080ed408(0x2F, 7, 7, 3, 2);
    }
    sp20 = *(s32 *)0x03001F0C;
    sp1C = *(s32 *)0x03001F08;
    temp_r9 = *Func_080b5098(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s32 *, 8));
    sp10 = *Func_080b5098((s32) M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s16 *, 0x24));
    var_r7 = (void *)0x02010000;
    var_r8 = 0;
    do {
        temp_r6 = (u16) Func_08004458();
        temp_r0_3 = Func_08004458();
        M2C_FIELD(var_r7, s32 *, 0) = 0;
        temp_r3 = ((0x1F & Func_08004458()) + 0x14) << 0x10;
        M2C_FIELD(var_r7, s32 *, 8) = 0;
        M2C_FIELD(var_r7, s32 *, 4) = temp_r3;
        temp_r5 = (0xFF & temp_r0_3) + 0x80;
        M2C_FIELD(var_r7, s32 *, 0xC) = (s32) ((s32) (temp_r5 * Func_08002322((s32) temp_r6)) >> 5);
        M2C_FIELD(var_r7, s32 *, 0x10) = 0;
        M2C_FIELD(var_r7, s32 *, 0x14) = (s32) ((s32) (temp_r5 * Func_0800231c((s32) temp_r6)) >> 5);
        var_r8 += 1;
        M2C_FIELD(var_r7, s32 *, 0x18) = 0;
        var_r7 += 0x1C;
    } while (var_r8 != 0x40);
    M2C_FIELD(sp28, s32 *, 0x7780) = 2;
    M2C_FIELD(sp28, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    M2C_FIELD(sp60, s32 *, 0) = (s32) M2C_FIELD(temp_r9, s32 *, 8);
    M2C_FIELD(sp60, s32 *, 4) = 0;
    M2C_FIELD(sp60, s32 *, 8) = (s32) M2C_FIELD(temp_r9, s32 *, 0x10);
    switch (sp2C) {
    case 0:
        M2C_FIELD(sp54, s32 *, 0) = (s32) M2C_FIELD(sp10, s32 *, 8);
        M2C_FIELD(sp54, s32 *, 4) = 0x3C0000;
        M2C_FIELD(sp54, s32 *, 8) = (s32) M2C_FIELD(sp10, s32 *, 0x10);
        break;
    case 1:
        var_r3 = M2C_FIELD(sp10, s32 *, 8);
block_24:
        M2C_FIELD(sp54, s32 *, 0) = var_r3;
        M2C_FIELD(sp54, s32 *, 4) = 0x3C0000;
        M2C_FIELD(sp54, s32 *, 8) = 0;
        break;
    case 2:
        M2C_FIELD(sp54, s32 *, 0) = (s32) M2C_FIELD(temp_r9, s32 *, 8);
        M2C_FIELD(sp54, s32 *, 4) = 0x3C0000;
        M2C_FIELD(sp54, s32 *, 8) = (s32) M2C_FIELD(temp_r9, s32 *, 0x10);
        break;
    case 3:
        var_r3 = M2C_FIELD(temp_r9, s32 *, 8);
        goto block_24;
    case 4:
        M2C_FIELD(sp54, s32 *, 0) = 0;
        M2C_FIELD(sp54, s32 *, 4) = 0x3C0000;
        M2C_FIELD(sp54, s32 *, 8) = 0;
        break;
    default:
        break;
    }
    spC = interpolation_delta;
    M2C_FIELD(spC, s32 *, 0) = Func_080022ec(M2C_FIELD(sp54, s32 *, 0) - M2C_FIELD(sp60, s32 *, 0), 0x28);
    M2C_FIELD(spC, s32 *, 4) = Func_080022ec(M2C_FIELD(sp54, s32 *, 4) - M2C_FIELD(sp60, s32 *, 4), 0x28);
    M2C_FIELD(spC, s32 *, 8) = Func_080022ec(M2C_FIELD(sp54, s32 *, 8) - M2C_FIELD(sp60, s32 *, 8), 0x28);
    var_sl = 0;
    if (sp14 == 0) {

    } else {
loop_29:
        temp_r5_2 = *(s32 *)0x03001E80;
        if (var_sl > 0x4B) {
            *(s16 *)0x04000052 = (0xA8 - (var_sl * 2)) | 0x1000;
        }
        if (var_sl == 8) {
            Func_080f9010(0xD4);
        }
        Func_080049ac();
        Func_080051d8(temp_r5_2, temp_r5_2 + 0xC);
        if ((u32) (var_sl - 6) <= 0x27U) {
            M2C_FIELD(sp60, s32 *, 0) = (s32) (M2C_FIELD(sp60, s32 *, 0) + M2C_FIELD(spC, s32 *, 0));
            M2C_FIELD(sp60, s32 *, 4) = (s32) (M2C_FIELD(sp60, s32 *, 4) + M2C_FIELD(spC, s32 *, 4));
            M2C_FIELD(sp60, s32 *, 8) = (s32) (M2C_FIELD(sp60, s32 *, 8) + M2C_FIELD(spC, s32 *, 8));
        }
        Func_08004cb4(sp60);
        if (var_sl == 0) {
            Func_080d6888(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s32 *, 8), 7, -1, -1, var_sl);
        }
        if (var_sl == 0x18) {
            Func_080d6888(M2C_FIELD(M2C_FIELD(sp28, void **, 0x7828), s32 *, 8), 0, -1, -1, 0);
        }
        temp_r4 = 0 - var_sl;
        sp8 = temp_r4;
        var_r6 = (void *)0x02010000;
        var_r8_2 = 0;
        var_r7_2 = temp_r4 << 8;
        var_r9 = var_sl << 8;
loop_40:
        var_r3_2 = var_r8_2;
        if (var_r3_2 < 0) {
            var_r3_2 += 7;
        }
        if (var_sl < (s32) (var_r3_2 >> 3)) {

        } else if (M2C_FIELD(var_r6, s32 *, 0x18) != 0) {

        } else {
            Func_080049e8();
            temp_r3_2 = 3 & var_r8_2;
            switch (temp_r3_2) {
            case 0:
                Func_08004c1c(var_r9);
                break;
            case 1:
                Func_08004bd4(var_r7_2);
                break;
            case 2:
                Func_08004c6c(var_r7_2);
                break;
            case 3:
                Func_08004bd4(var_r7_2);
                Func_08004c6c(var_r7_2);
                break;
            }
            Func_080e3944((s32) var_r6, sp30);
            M2C_FIELD(sp30, s32 *, 0) = (s32) ((s32) M2C_FIELD(sp30, s32 *, 0) >> 1);
            Func_08004a5c();
            var_r2 = M2C_FIELD(sp30, s32 *, 8);
            if (var_r2 <= 0xF9) {
                M2C_FIELD(sp30, s32 *, 8) = 0xFA;
                var_r2 = 0xFA;
            }
            if (var_r2 > 0x27A) {
                M2C_FIELD(sp30, s32 *, 8) = 0x27A;
                var_r2 = 0x27A;
            }
            var_r3_3 = var_r2 - 0xFA;
            if (var_r3_3 < 0) {
                var_r3_3 += 0x3F;
            }
            temp_r0_4 = 8 - (var_r3_3 >> 6);
            temp_r4_2 = temp_r0_4 * 2;
            Func_080072f4(sp24, sp18 + M2C_FIELD((temp_r4_2 - 2), u16 *, 0x080EDE48), M2C_FIELD(sp30, s32 *, 0) - ((s32) (temp_r0_4 + (temp_r0_4 >> 0x1F)) >> 1), M2C_FIELD(sp30, s32 *, 4) - temp_r0_4, temp_r0_4, temp_r4_2);
            Func_080e38b8(var_r6, 0x3C, 0);
            var_r3_4 = var_r8_2;
            if (var_r3_4 < 0) {
                var_r3_4 += 7;
            }
            if (var_sl >= (s32) ((var_r3_4 >> 3) + 0x18)) {
                temp_r5_3 = (s32) (0 - M2C_FIELD(var_r6, s32 *, 0)) >> 7;
                temp_r4_3 = (s32) (0 - M2C_FIELD(var_r6, s32 *, 8)) >> 7;
                temp_r2 = M2C_FIELD(var_r6, s32 *, 0x10) + ((s32) (0 - M2C_FIELD(var_r6, s32 *, 4)) >> 7);
                temp_r1_2 = M2C_FIELD(var_r6, s32 *, 0xC) + temp_r5_3;
                temp_r0_5 = M2C_FIELD(var_r6, s32 *, 0x14) + temp_r4_3;
                var_r3_5 = temp_r1_2 * 0x3E;
                M2C_FIELD(var_r6, s32 *, 0xC) = temp_r1_2;
                M2C_FIELD(var_r6, s32 *, 0x10) = temp_r2;
                M2C_FIELD(var_r6, s32 *, 0x14) = temp_r0_5;
                if (var_r3_5 < 0) {
                    var_r3_5 += 0x3F;
                }
                M2C_FIELD(var_r6, s32 *, 0xC) = (s32) (var_r3_5 >> 6);
                var_r3_6 = temp_r2 * 0x3E;
                if (var_r3_6 < 0) {
                    var_r3_6 += 0x3F;
                }
                M2C_FIELD(var_r6, s32 *, 0x10) = (s32) (var_r3_6 >> 6);
                var_r3_7 = temp_r0_5 * 0x3E;
                if (var_r3_7 < 0) {
                    var_r3_7 += 0x3F;
                }
                M2C_FIELD(var_r6, s32 *, 0x14) = (s32) (var_r3_7 >> 6);
                if (((u32) (temp_r5_3 + 0x7FF) <= 0xFFEU) && ((u32) (temp_r4_3 + 0x7FF) <= 0xFFEU)) {
                    M2C_FIELD(var_r6, s32 *, 0x18) = -1;
                }
            }
        }
        var_r7_2 += sp8 << 5;
        var_r8_2 += 1;
        var_r9 += var_sl << 5;
        var_r6 += 0x1C;
        if (var_r8_2 != 0x20) {
            goto loop_40;
        }
        if ((u32) (var_sl - 0x36) <= 0xFU) {
            M2C_FIELD(sp3C, s32 *, 0) = (s32) (Func_08002322(var_sl << 0xA) * 4);
            M2C_FIELD(sp3C, s32 *, 4) = 0;
            M2C_FIELD(sp3C, s32 *, 8) = 0;
            Func_080e3944((s32)sp3C, sp30);
            *arg4 = M2C_FIELD(sp30, s32 *, 0);
            *arg5 = M2C_FIELD(sp30, s32 *, 4);
            temp_r2_2 = (s32) M2C_FIELD(sp30, s32 *, 0) >> 1;
            M2C_FIELD(sp30, s32 *, 0) = temp_r2_2;
            Func_080072f4(sp24, sp28, temp_r2_2 - 0xA, M2C_FIELD(sp30, s32 *, 4) - 0x14, 0x14U, 0x28);
        }
        if (var_sl == 0x40) {
            var_r8_3 = 0;
            var_r7_3 = sp28 + 0x7080;
            do {
                temp_r6_2 = (u16) Func_08004458();
                temp_r0_6 = Func_08004458();
                M2C_FIELD(var_r7_3, s32 *, 0) = (s32) (*arg4 << 0xF);
                M2C_FIELD(var_r7_3, s32 *, 4) = (s32) (*arg5 << 0x10);
                temp_r5_4 = (0xFF & temp_r0_6) + 0x80;
                M2C_FIELD(var_r7_3, s32 *, 0xC) = (s32) ((s32) (temp_r5_4 * Func_08002322((s32) temp_r6_2)) >> 6);
                M2C_FIELD(var_r7_3, s32 *, 0x10) = (s32) ((s32) (temp_r5_4 * Func_0800231c((s32) temp_r6_2)) >> 5);
                var_r8_3 += 1;
                M2C_FIELD(var_r7_3, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 8);
                var_r7_3 += 0x1C;
            } while (var_r8_3 != 0x40);
        }
        if (var_sl > 0x3F) {
            var_r8_4 = 0;
            var_r5_2 = sp28 + 0x7080;
            do {
                temp_r0_7 = M2C_FIELD(var_r5_2, s32 *, 0x18);
                if (temp_r0_7 >= 0) {
                    temp_r0_8 = (temp_r0_7 >> 3) + 2;
                    temp_r4_4 = temp_r0_8 * 2;
                    Func_080072f4(sp24, sp18 + M2C_FIELD((temp_r4_4 - 2), u16 *, 0x080EDE48), M2C_FIELD(var_r5_2, s16 *, 2) - ((s32) (temp_r0_8 + (temp_r0_8 >> 0x1F)) >> 1), M2C_FIELD(var_r5_2, s16 *, 6) - temp_r0_8, temp_r0_8, temp_r4_4);
                    Func_080e3908(var_r5_2, 0x3C, 0);
                    M2C_FIELD(var_r5_2, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_2, s32 *, 0x18) - 1);
                }
                var_r8_4 += 1;
                var_r5_2 += 0x1C;
            } while (var_r8_4 != 0x40);
        }
        M2C_FIELD(sp28, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        var_sl += 1;
        if (var_sl != sp14) {
            goto loop_29;
        }
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_08004278((void *)0x080CD4B5);
    Func_080072f8(0x06004000, 0x4000);
    temp_r0 = Func_080072f8(sp24, 0x4000);
    *(s16 *)0x04000052 = 0x1010;
    return temp_r0;
}
