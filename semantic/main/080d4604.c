typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
void Func_080072f4(s32, void *, s32, s32, u32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080d6888(s16, s32, s32, s32, s32);

/*
 * Run the particle-and-sprite scene for a battle transition.
 *
 * The mode chooses the initial origin and palette. Multiple particle groups
 * are seeded, staged into view, advanced with gravity, and paired with
 * character effects before all temporary display resources are released.
 */
s32 Func_080d4604(s32 arg0, s32 arg1) {
    s32 sp8;
    void *spC;
    void *sp10;
    u32 sp14;
    void *sp18;
    void **sp1C;
    s32 sp20;
    s32 sp24;
    void *sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C[2];
    s32 sp44[2];
    s32 temp_r2;
    s32 temp_r2_4;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r6;
    s32 temp_r7_2;
    s32 temp_r8;
    s32 var_r0;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r7_2;
    s32 var_sl_2;
    s32 var_sl_3;
    s32 var_sl_4;
    s32 var_sl_5;
    u16 temp_r0_2;
    u16 temp_r7;
    u16 var_r3_3;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r3;
    u32 temp_r3_6;
    u32 var_r3_4;
    u32 var_sl;
    void *temp_r0;
    void *temp_r1;
    void *temp_r1_2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r6_2;
    void *temp_r6_3;
    void *var_r5;
    void *var_r7;

    sp38 = arg1;
    temp_r1 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp34 = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp28 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    M2C_FIELD(temp_r1, s32 *, 0x7828) = arg0;
    if (sp38 == 0) {
        Func_080cd594(1);
        sp24 = 0x3C;
        sp20 = 0x30;
    } else {
        if (sp38 == 1) {
            Func_080cd594(0);
            var_r3 = 0x40;
            sp24 = 0x3C;
        } else {
            Func_080cd594(0);
            Func_080e396c(M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 8), sp44);
            temp_r3 = M2C_FIELD(sp44, u32 *, 0);
            sp24 = (s32) (temp_r3 + (temp_r3 >> 0x1F)) >> 1;
            var_r3 = M2C_FIELD(sp44, s32 *, 4) + 0x30;
        }
        sp20 = var_r3;
    }
    *(s16 *)0x04000052 = 0x1010;
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp3C[0] = *(s32 *)0x03001F08;
    Func_080ed408(0x2F, 7, 7, 3, 3);
    temp_r0 = sp3C;
    sp18 = temp_r0;
    M2C_FIELD(temp_r0, s32 *, 4) = (s32) *(s32 *)0x03001F0C;
    Func_080e0524((void *)0x7D, temp_r1, 1, 1);
    Func_080e0524((void *)0x73, sp28, 0, 0);
    if (sp38 == 1) {
        Func_080072f0(0x05000000, Func_08002f40(0x87), 0x80, 0x03001388);
    } else if (sp38 == 2) {
        Func_080072f0(0x05000000, Func_08002f40(0xC4), 0x80, 0x03001388);
    }
    sp30 = 0;
    if (M2C_FIELD(((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262) == 0) {

    } else {
        spC = temp_r1;
loop_14:
        var_sl = 0;
        var_r7 = spC + 0x7080;
        do {
            temp_r6 = var_sl * 2;
            temp_r0_2 = (u16) Func_08004458();
            M2C_FIELD(var_r7, s32 *, 0) = (s32) (temp_r6 * Func_08002322((s32) temp_r0_2));
            M2C_FIELD(var_r7, s32 *, 4) = (s32) (0 - (temp_r6 * Func_0800231c((s32) temp_r0_2)));
            temp_r3_2 = ((s32) ((var_sl >> 0x1F) + var_sl) >> 1) + 0x19;
            var_sl += 1;
            M2C_FIELD(var_r7, s32 *, 0x18) = temp_r3_2;
            var_r7 += 0x1C;
        } while (var_sl != 0x10);
        var_sl_2 = 0;
        if (M2C_FIELD((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA), u16 *, 0x080EE262) != 0) {
            do {
                temp_r6_2 = (void *)((((M2C_FIELD((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA), u16 *, 0x080EE262) * sp30) + var_sl_2) * 0x1C) + 0x02010000);
                sp8 = 0x080EE262;
                temp_r8 = (0x3FF & Func_08004458()) + 0x20;
                temp_r5 = M2C_FIELD(temp_r1, s32 *, 0x7828);
                temp_r7 = (u16) Func_08004458();
                if (M2C_FIELD(temp_r5, s32 *, 4) == 1) {
                    var_r3_2 = (sp24 - M2C_FIELD(((((M2C_FIELD(temp_r5, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) + 0x1C;
                } else {
                    var_r3_2 = (sp24 + M2C_FIELD(((((M2C_FIELD(temp_r5, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) - 0x1C;
                }
                M2C_FIELD(temp_r6_2, s32 *, 0) = (s32) (var_r3_2 << 0x10);
                M2C_FIELD(temp_r6_2, s32 *, 4) = (s32) (sp20 << 0x10);
                M2C_FIELD(temp_r6_2, s32 *, 0xC) = (s32) ((s32) (temp_r8 * Func_08002322((s32) temp_r7)) >> 6);
                M2C_FIELD(temp_r6_2, s32 *, 0x10) = (s32) ((s32) (0 - (temp_r8 * Func_0800231c((s32) temp_r7) * 2)) >> 6);
                var_sl_2 += 1;
                M2C_FIELD(temp_r6_2, s32 *, 0x18) = (s32) ((7 & Func_08004458()) + 0x20);
            } while (var_sl_2 != M2C_FIELD((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA), u16 *, 0x080EE262));
        }
        spC += 0x1C0;
        sp30 += 1;
        if (sp30 != M2C_FIELD(((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262)) {
            goto loop_14;
        }
    }
    M2C_FIELD(temp_r1, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r1, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    sp2C = 0;
    if (M2C_FIELD(((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262) == 0x1FFFFFF9) {

    } else {
        sp14 = sp38 - 1;
        sp1C = temp_r1 + 0x7828;
loop_27:
        temp_r1_2 = *(void **)0x03001E80;
        temp_r2 = M2C_FIELD(temp_r1, s32 *, 0x7828);
        if ((M2C_FIELD(temp_r2, s32 *, 0x18) == 2) && (sp2C <= 0x33)) {
            if (M2C_FIELD(temp_r2, s32 *, 4) == 0) {
                var_r3_3 = M2C_FIELD(temp_r1_2, u16 *, 0x36) + 0x100;
            } else {
                var_r3_3 = M2C_FIELD(temp_r1_2, u16 *, 0x36) + 0xFFFFFF00;
            }
            M2C_FIELD(temp_r1_2, u16 *, 0x36) = var_r3_3;
        }
        if ((M2C_FIELD(*sp1C, s32 *, 0x18) == 3) && (sp2C == 4)) {
            Func_080072f0(sp34, 0x4000, 0x3F3F3F3F, 0x03000168);
        }
        if (sp14 <= 1U) {
            if (sp2C == 2) {
                Func_080b50e8(0x91);
            }
        } else {
            if (sp2C == 2) {
                Func_080f9010(0x91);
            }
            if (sp2C == 0x18) {
                Func_080b50e8(0x86);
            }
        }
        sp30 = 0;
        if (M2C_FIELD(((M2C_FIELD(*sp1C, s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262) == 0) {

        } else {
            sp10 = temp_r1;
loop_46:
            temp_r3_3 = sp30 * 8;
            if (sp2C == temp_r3_3) {
                M2C_FIELD(temp_r1, s32 *, 0x77A8) = 0xC;
            }
            if (sp2C < temp_r3_3) {

            } else {
                if (sp2C < (s32) (temp_r3_3 + 2)) {
                    temp_r2_2 = *sp1C;
                    if (M2C_FIELD(temp_r2_2, s32 *, 4) == 1) {
                        Func_080072f4(sp34, temp_r1, (sp24 - M2C_FIELD(((((M2C_FIELD(temp_r2_2, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) + 0xC, sp20 - 0x20, 0x20U, 0x40);
                    } else {
                        Func_080072f4(sp34, temp_r1, (sp24 + M2C_FIELD(((((M2C_FIELD(temp_r2_2, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) - 0x2C, sp20 - 0x20, 0x20U, 0x40);
                    }
                }
                if (sp2C >= temp_r3_3) {
                    var_sl_3 = 0;
                    var_r5 = sp10 + 0x7080;
                    do {
                        temp_r7_2 = M2C_FIELD(var_r5, s16 *, 6) + sp20;
                        temp_r2_3 = *sp1C;
                        if (M2C_FIELD(temp_r2_3, s32 *, 4) == 1) {
                            var_r6 = ((M2C_FIELD(var_r5, s16 *, 2) + sp24) - M2C_FIELD(((((M2C_FIELD(temp_r2_3, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) + 0x1C;
                        } else {
                            var_r6 = (M2C_FIELD(var_r5, s16 *, 2) + sp24 + M2C_FIELD(((((M2C_FIELD(temp_r2_3, s32 *, 0x18) * 5) + sp30) * 2) + 4), u16 *, 0x080EE262)) - 0x1C;
                        }
                        temp_r0_3 = M2C_FIELD(var_r5, u32 *, 0x18);
                        if (temp_r0_3 <= 0x11U) {
                            Func_080072f4(sp34, (M2C_FIELD((void *) Func_080022ec((s32) temp_r0_3, 3), u8 *, 0x080EE294) << 0xB) + temp_r1, var_r6 - 0x10, temp_r7_2 - 0x20, 0x20U, 0x40);
                        }
                        if ((s32) M2C_FIELD(var_r5, u32 *, 0x18) > 0) {
                            var_r3_4 = (s32) M2C_FIELD(var_r5, u32 *, 0x18) - 1;
                        } else {
                            var_r3_4 = -1U;
                        }
                        M2C_FIELD(var_r5, u32 *, 0x18) = var_r3_4;
                        var_sl_3 += 1;
                        var_r5 += 0x1C;
                    } while (var_sl_3 != 0xC);
                }
            }
            if (sp2C > (s32) (temp_r3_3 + 5)) {
                var_r7_2 = -0x1000;
                if (sp38 != 2) {
                    var_r7_2 = 0x1000;
                }
                var_sl_4 = 0;
                var_r0 = 0x080EE262;
                if (M2C_FIELD((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA), u16 *, 0x080EE262) != 0) {
                    do {
                        temp_r3_4 = ((M2C_FIELD((void *)var_r0, u16 *, M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA) * sp30) + var_sl_4) * 0x1C;
                        temp_r6_3 = (void *)(temp_r3_4 + 0x02010000);
                        if ((s32) M2C_FIELD(temp_r6_3, s32 *, 0x18) > 0) {
                            Func_080e3908(temp_r6_3, 0x3C, var_r7_2);
                            temp_r2_4 = M2C_FIELD(temp_r6_3, s32 *, 4);
                            temp_r3_5 = M2C_FIELD(temp_r6_3, s32 *, 0x18) - 1;
                            M2C_FIELD(temp_r6_3, s32 *, 0x18) = temp_r3_5;
                            if (temp_r2_4 > 0x6C0000) {
                                temp_r3_6 = 0 - M2C_FIELD(temp_r6_3, s32 *, 0x10);
                                M2C_FIELD(temp_r6_3, s32 *, 0x10) = (s32) ((s32) (temp_r3_6 + (temp_r3_6 >> 0x1F)) >> 1);
                            } else {
                                temp_r0_4 = M2C_FIELD(temp_r3_4, u32 *, 0x02010000);
                                if ((temp_r0_4 <= 0x7EFFFFU) && (temp_r2_4 >= 0)) {
                                    temp_r0_5 = Func_080022ec(temp_r3_5, 5) + 1;
                                    temp_r5_2 = temp_r0_5 * 2;
                                    Func_080072f4(sp34, sp28 + M2C_FIELD((temp_r5_2 - 2), u16 *, 0x080EDE48), ((s32) temp_r0_4 >> 0x10) - ((s32) (temp_r0_5 + (temp_r0_5 >> 0x1F)) >> 1), (temp_r2_4 >> 0x10) - temp_r0_5, temp_r0_5, temp_r5_2);
                                }
                            }
                        }
                        var_sl_4 += 1;
                        var_r0 = 0x080EE262;
                    } while (var_sl_4 != M2C_FIELD((void *)var_r0, u16 *, M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA));
                }
            }
            var_sl_5 = 0;
            if (M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x14) != 0) {
                var_r6_2 = 0x24;
                do {
                    if (sp2C == (temp_r3_3 + 6)) {
                        Func_080d6888(M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s16 *, var_r6_2), 7, 5, var_sl_5, 0xA);
                        Func_080b5088(M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s16 *, var_r6_2), 4);
                    }
                    var_sl_5 += 1;
                    var_r6_2 += 2;
                } while (var_sl_5 != M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x14));
            }
            sp10 += 0x1C0;
            sp30 += 1;
            if (sp30 != M2C_FIELD(((M2C_FIELD(*sp1C, s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262)) {
                goto loop_46;
            }
        }
        Func_080e155c(0x10, 0x10U);
        Func_080cd52c();
        M2C_FIELD(temp_r1, s32 *, 0x7824) = 1;
        Func_080030f8(1U);
        sp2C += 1;
        if (sp2C != ((M2C_FIELD(((M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s32 *, 0x18) * 0xA) + 2), u16 *, 0x080EE262) * 8) + 0x38)) {
            goto loop_27;
        }
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}
