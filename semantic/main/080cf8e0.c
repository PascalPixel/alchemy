typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
void Func_080072f4(s32, void *, s32, s32, s32, s32);
void Func_080072f8(s32, s32, s32);
void **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080d6888(s16, s32, s32, s32, s32);

/*
 * Run a multi-character projectile and impact scene.
 *
 * Per-character streams travel toward the active target, spawn secondary
 * impact particles on contact, and use one of two sprite families selected by
 * the scene variant before presentation and cleanup.
 */
s32 Func_080cf8e0(s32 arg0, s32 arg1) {
    s32 sp8;
    s32 spC;
    void *sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    void *sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp44[3];
    s32 loaded_handles[2];
    volatile s32 setup_stage;
    s32 *var_r3;
    s32 *var_r3_2;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_4;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r3_3;
    s32 var_r4;
    s32 var_r5;
    s32 var_r7;
    s32 var_r9;
    s32 var_sl;
    s32 var_sl_2;
    s32 var_sl_3;
    s32 var_sl_4;
    s32 var_sl_5;
    s8 temp_r6_2;
    u32 var_r3_4;
    u8 temp_r4_2;
    u8 temp_r5_2;
    void *temp_r0;
    void *temp_r0_4;
    void *temp_r1_3;
    void *temp_r2;
    void *temp_r4;
    void *temp_r5;
    void *temp_r6;
    void *var_r1;
    void *var_r8;

    sp38 = arg0;
    temp_r0 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp34 = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp24 = *(s32 *)0x03001E80;
    M2C_FIELD(temp_r0, s32 *, 0x7828) = sp38;
    Func_080cd594(1);
    *(s16 *)0x04000052 = 0x1010;
    temp_r0_2 = Func_08002f40(0xBF);
    Func_080072f8(0x05000000, temp_r0_2, 0x80);
    Func_08005340(temp_r0_2 + 0x80, (s32) temp_r0);
    temp_r0_3 = Func_08002f40(0x9E);
    Func_080072f8(0x05000000, temp_r0_3, 0x80);
    Func_08005340(temp_r0_3 + 0x80, (s32) (temp_r0 + 0x3E80));
    switch (arg1) {
    case 0:
        var_r0 = 0x9F;
        break;
    case 1:
        var_r0 = 0x59;
        break;
    case 2:
        var_r0 = 0xA0;
        break;
    case 4:
        var_r0 = 0xBF;
        break;
    case 6:
        var_r0 = 0x8D;
        break;
    default:
        var_r0 = 0x77;
        break;
    }
    Func_080072f0(0x05000000, Func_08002f40(var_r0), 0x80, 0x03001388);
    var_sl = 0;
    var_r3 = (s32 *)0x02010018;
    do {
        var_sl += 1;
        *var_r3 = -1;
        var_r3 += 0x1C;
    } while (var_sl != 0x400);
    temp_r6 = *Func_080b5098(M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 8));
    sp2C = 0;
    var_r4 = 0x40000;
    if (M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14) != 0) {
        var_r7 = 0;
        do {
            sp8 = var_r4;
            temp_r0_4 = *Func_080b5098((s32) M2C_FIELD(M2C_FIELD(temp_r0, void **, 0x7828), s16 *, (sp2C * 2) + 0x24));
            var_sl_2 = 0;
            var_r1 = (void *)(var_r7 + 0x02010000);
loop_18:
            M2C_FIELD(var_r1, s32 *, 4) = var_r4;
            M2C_FIELD(var_r1, s32 *, 0) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
            M2C_FIELD(var_r1, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
            M2C_FIELD(var_r1, s32 *, 0xC) = (s32) ((s32) (M2C_FIELD(temp_r0_4, s32 *, 8) - M2C_FIELD(temp_r6, s32 *, 8)) >> 4);
            M2C_FIELD(var_r1, s32 *, 0x10) = 0x40000;
            M2C_FIELD(var_r1, s32 *, 0x14) = (s32) ((s32) (M2C_FIELD(temp_r0_4, s32 *, 0x10) - M2C_FIELD(temp_r6, s32 *, 0x10)) >> 4);
            var_sl_2 += 1;
            M2C_FIELD(var_r1, s32 *, 0x18) = 0;
            var_r1 += 0x1C;
            if (var_sl_2 != 0x10) {
                goto loop_18;
            }
            temp_r1 = sp2C + 1;
            sp2C = temp_r1;
            var_r7 += 0x1C0;
        } while (temp_r1 != M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14));
    }
    var_sl_3 = 0;
    var_r3_2 = (s32 *)0x02011C18;
    do {
        var_sl_3 += 1;
        *var_r3_2 = -1;
        var_r3_2 += 0x1C;
    } while (var_sl_3 != 0x100);
    if (M2C_FIELD(sp38, s32 *, 4) == 0) {
        Func_080ed408(0x2E, 7, 7, 3, 2);
        setup_stage = 1;
        temp_r1_2 = arg1 * 2;
        sp3C = *(s32 *)0x03001F08;
        sp1C = temp_r1_2;
        if (M2C_FIELD(temp_r1_2, s8 *, 0x080EE0B6) == 0) {
            goto block_29;
        }
        Func_080ed408(0x2F, 7, 7, 7, 2);
        setup_stage = 2;
        temp_r4 = loaded_handles;
        sp20 = temp_r4;
        M2C_FIELD(temp_r4, s32 *, 4) = (s32) *(s32 *)0x03001F0C;
    } else {
        Func_080ed408(0x2E, 7, 7, 7, 2);
        setup_stage = 3;
        temp_r0_5 = arg1 * 2;
        sp3C = *(s32 *)0x03001F08;
        sp1C = temp_r0_5;
        if (M2C_FIELD(temp_r0_5, s8 *, 0x080EE0B6) == 0) {
            Func_080ed408(0x2F, 7, 7, 3, 3);
            setup_stage = 4;
            temp_r1_3 = loaded_handles;
            sp20 = temp_r1_3;
            M2C_FIELD(temp_r1_3, s32 *, 4) = *(s32 *)0x03001F0C;
        } else {
block_29:
            Func_080ed408(0x2F, 7, 7, 3, 2);
            setup_stage = 5;
            temp_r2 = loaded_handles;
            sp20 = temp_r2;
            M2C_FIELD(temp_r2, s32 *, 4) = *(s32 *)0x03001F0C;
        }
    }
    M2C_FIELD(temp_r0, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r0, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    if (M2C_FIELD(sp1C, s8 *, 0x080EE0B6) == 0) {
        var_r3_3 = (M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14) * 8) + 0x48;
    } else {
        var_r3_3 = (M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14) * 8) + 0x38;
    }
    sp28 = var_r3_3;
    Func_080f9010(0x67);
    sp30 = 0;
    if (sp28 == 0) {

    } else {
        sp14 = sp24 + 0xC;
loop_36:
        Func_080049ac();
        Func_080051d8(sp24, sp14);
        sp2C = 0;
        if (M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14) == 0) {

        } else {
            sp10 = (void *)0x02010000;
            spC = 0;
loop_39:
            if (sp30 < spC) {

            } else {
                if (sp30 == (spC + 0x11)) {
                    Func_080d6888(M2C_FIELD(M2C_FIELD(temp_r0, void **, 0x7828), s16 *, (sp2C * 2) + 0x24), 7, 5, sp2C, 0x10);
                    Func_080b50e8(0x85);
                }
                if ((s32) M2C_FIELD(sp10, s32 *, 0x18) < 0) {

                } else {
                    var_r5 = Func_080022ec(sp30 - spC, 3);
                    if (var_r5 > 9) {
                        var_r5 = 9;
                    }
                    Func_080e3944((s32) sp10, sp44);
                    temp_r2_2 = (s32) M2C_FIELD(sp44, s32 *, 0) >> 1;
                    M2C_FIELD(sp44, s32 *, 0) = temp_r2_2;
                    if (var_r5 > 4) {
                        Func_080072f4(sp34, (var_r5 * 0x300) + temp_r0, temp_r2_2 - 0x10, M2C_FIELD(sp44, s32 *, 4) - 0xC, 0x20, 0x18);
                    } else {
                        Func_080072f4(sp34, (var_r5 * 0x300) + temp_r0, temp_r2_2 - 0xC, M2C_FIELD(sp44, s32 *, 4) - 0x10, 0x18, 0x20);
                    }
                    if (M2C_FIELD(sp10, s32 *, 0x18) == 0) {
                        Func_080e38b8(sp10, 0x3F, -0x8000);
                    }
                    if ((s32) M2C_FIELD(sp10, s32 *, 4) < 0) {
                        M2C_FIELD(sp10, s32 *, 4) = 0;
                        M2C_FIELD(sp10, s32 *, 0x18) = 1;
                        var_r9 = 4;
                        if (M2C_FIELD(sp1C, s8 *, 0x080EE0B6) != 0) {
                            var_r9 = 0x10;
                        }
                        var_sl_4 = 0;
                        if (var_r9 != 0) {
                            sp18 = sp2C * 4;
                            do {
                                temp_r3 = ((sp18 * 8) + var_sl_4) * 0x1C;
                                temp_r5 = (void *)(temp_r3 + 0x02011C00);
                                M2C_FIELD(temp_r3, s32 *, 0x02011C00) = (s32) M2C_FIELD(sp10, s32 *, 0);
                                M2C_FIELD(temp_r5, s32 *, 4) = (s32) M2C_FIELD(sp10, s32 *, 4);
                                M2C_FIELD(temp_r5, s32 *, 8) = (s32) M2C_FIELD(sp10, s32 *, 8);
                                temp_r6_2 = M2C_FIELD(sp1C, s8 *, 0x080EE0B6);
                                if (temp_r6_2 == 0) {
                                    M2C_FIELD(temp_r5, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xB);
                                    M2C_FIELD(temp_r5, s32 *, 0x10) = (s32) temp_r6_2;
                                    var_r0_2 = ((Func_08004458() & 0x3F) - 0x20) << 0xB;
                                } else {
                                    M2C_FIELD(temp_r5, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xD);
                                    M2C_FIELD(temp_r5, s32 *, 0x10) = (s32) (((0x1F & Func_08004458()) + 0x20) << 0xC);
                                    var_r0_2 = ((Func_08004458() & 0x3F) - 0x20) << 0xD;
                                }
                                M2C_FIELD(temp_r5, s32 *, 0x14) = var_r0_2;
                                var_sl_4 += 1;
                                M2C_FIELD(temp_r5, s32 *, 0x18) = 0;
                            } while (var_sl_4 != var_r9);
                        }
                    }
                }
            }
            sp10 += 0x1C0;
            temp_r1_4 = sp2C + 1;
            spC += 8;
            sp2C = temp_r1_4;
            if (temp_r1_4 != M2C_FIELD(M2C_FIELD(temp_r0, s32 *, 0x7828), s32 *, 0x14)) {
                goto loop_39;
            }
        }
        var_sl_5 = 0;
        var_r8 = (void *)0x02011C00;
        do {
            if (((u32) M2C_FIELD(var_r8, u32 *, 0x18) <= 0x2CU) && ((s32) M2C_FIELD(var_r8, s32 *, 4) >= 0)) {
                Func_080e3944((s32) var_r8, sp44);
                temp_r2_3 = (s32) M2C_FIELD(sp44, s32 *, 0) >> 1;
                M2C_FIELD(sp44, s32 *, 0) = temp_r2_3;
                if (M2C_FIELD(sp1C, s8 *, 0x080EE0B6) == 0) {
                    var_r3_4 = M2C_FIELD(var_r8, u32 *, 0x18);
                    if ((s32) var_r3_4 < 0) {
                        var_r3_4 += 7;
                    }
                    Func_080072f4(sp34, (((s32) var_r3_4 >> 3) * 0x480) + temp_r0 + 0x3E80, temp_r2_3 - 0xC, M2C_FIELD(sp44, s32 *, 4) - 0x18, 0x18, 0x30);
                } else {
                    var_r0_3 = Func_080022ec((s32) M2C_FIELD(var_r8, u32 *, 0x18), 5);
                    if (var_sl_5 & 1) {
                        var_r0_3 += 9;
                    }
                    if ((s32) M2C_FIELD(var_r8, s32 *, 0xC) > 0) {

                    }
                    temp_r5_2 = M2C_FIELD(var_r0_3, u8 *, 0x080EE0C4);
                    temp_r4_2 = M2C_FIELD(var_r0_3, u8 *, 0x080EE0D6);
                    Func_080072f4(sp34, M2C_FIELD((var_r0_3 * 2), u16 *, 0x080EE0E8) + temp_r0 + 0x1E00, M2C_FIELD(sp44, s32 *, 0) - (temp_r5_2 >> 1), M2C_FIELD(sp44, s32 *, 4) - (temp_r4_2 >> 1), (s32) temp_r5_2, (s32) temp_r4_2);
                }
                if (M2C_FIELD(sp1C, s8 *, 0x080EE0B6) == 0) {
                    Func_080e38b8(var_r8, 0x3E, 0x800);
                } else {
                    Func_080e38b8(var_r8, 0x3E, -0x8000);
                }
                M2C_FIELD(var_r8, u32 *, 0x18) = (u32) (M2C_FIELD(var_r8, u32 *, 0x18) + 1);
            }
            var_sl_5 += 1;
            var_r8 += 0x1C;
        } while (var_sl_5 != 0x100);
        Func_080e155c(2, 2U);
        Func_080cd52c();
        M2C_FIELD(temp_r0, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        temp_r3_2 = sp30 + 1;
        sp30 = temp_r3_2;
        if (temp_r3_2 != sp28) {
            goto loop_36;
        }
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}
