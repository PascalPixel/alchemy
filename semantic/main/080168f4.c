typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08003f3c(u32);
void Func_08016178(s32, s32, s32, s32);
void Func_08016478(void *);
void Func_080167ac(void *);
void Func_080167d8(void *);
void Func_080167e0(s32);
void Func_080170f8(s32, s32, s32, s32);
void Func_0801868c(s32, s32 *, s32 *, s32 *, s32 *, void *, s32);
s32 Func_08018cac(void *, u32, s32, s32, s32);
void Func_0801999c(void *);
s32 Func_080199ec(void *);
void Func_080f9010(s32);

/*
 * Consume queued field-script commands for one moving map object.
 *
 * Commands update delays, animation, direction, collision bounds, position,
 * and sound state. Geometry-changing commands reconcile the old and new
 * rectangles before committing the object's next frame.
 */
s32 Func_080168f4(void *arg0) {
    s32 spC;
    void *sp10;
    s32 *sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 temp_r0;
    s32 temp_r4_2;
    s32 temp_r4_4;
    s32 var_r0;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r5;
    u16 temp_r1_2;
    u16 temp_r2;
    u16 temp_r2_2;
    u16 temp_r2_3;
    u16 temp_r2_4;
    u16 temp_r2_5;
    u16 temp_r2_6;
    u16 temp_r2_7;
    u16 temp_r3_2;
    u16 temp_r3_3;
    u16 temp_r4;
    u16 temp_r4_3;
    u16 temp_r7;
    u16 var_r2;
    u16 var_r3_2;
    u16 var_r3_3;
    u16 var_r4;
    u16 var_r7;
    u8 temp_sl;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r1;
    void *temp_r3;
    void *temp_r5;

    temp_r3 = *(void **)0x03001E8C;
    sp20 = (s32) M2C_FIELD(*(void **)0x0200044C, u8 *, 0x0807380B);
    if (M2C_FIELD(temp_r3, u8 *, 0xEA5) != 0) {
        var_r2 = *(u16 *)0x03001CD0;
        if ((s32) var_r2 > 2) {
            var_r2 = 2;
        }
        sp20 = (var_r2 * 5) + 3;
    }
    if (M2C_FIELD(arg0, u16 *, 0x1C) != 0) {
        Func_080167e0(1);
        M2C_FIELD(arg0, u16 *, 0x1C) = (u16) (M2C_FIELD(arg0, u16 *, 0x1C) - 1);
        goto block_115;
    }
    if (*(s32 *)0x03001AE8 == 0) {
        temp_r2 = M2C_FIELD(arg0, u16 *, 0x22);
        if (temp_r2 != 0) {
            M2C_FIELD(arg0, u16 *, 0x22) = (u16) (temp_r2 + 0xFFFF);
            goto block_115;
        }
    }
loop_12:
    var_r7 = 0;
    if (M2C_FIELD(arg0, u16 *, 0x20) == 0) {
        var_r7 = M2C_FIELD(temp_r3, u16 *, (M2C_FIELD(arg0, u16 *, 0x12) * 2) + 0xEB0);
    }
    switch ((u32) var_r7) {
    case 3:
        M2C_FIELD(arg0, u16 *, 4) = (u16) M2C_FIELD(arg0, u16 *, 0x1E);
        if (8 & M2C_FIELD(M2C_FIELD(arg0, void **, 0), u16 *, 0x16)) {
            temp_r2_2 = M2C_FIELD(arg0, u16 *, 6);
            if ((u32) temp_r2_2 > 0xCFFU) {
                Func_080167d8(arg0);
                sp20 = 1;
            } else {
                M2C_FIELD(arg0, u16 *, 6) = (u16) (temp_r2_2 + 0xD00);
            }
        } else {
            temp_r2_3 = M2C_FIELD(arg0, u16 *, 0x10);
            M2C_FIELD(arg0, u16 *, 6) = (u16) (M2C_FIELD(arg0, u16 *, 6) + 0xF00);
            if ((u32) temp_r2_3 > 2U) {

            } else {
                M2C_FIELD(arg0, u16 *, 0x10) = (u16) (temp_r2_3 + 1);
            }
        }
    case 11:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
block_77:
        if (M2C_FIELD(temp_r3, u8 *, 0xEA5) != 0) {

        } else {
            sp20 = 1;
        }
block_109:
        temp_r2_4 = M2C_FIELD(arg0, u16 *, 0x14);
        if ((temp_r2_4 == 0) || (temp_r3_2 = temp_r2_4 + 0xFFFF, M2C_FIELD(arg0, u16 *, 0x14) = temp_r3_2, ((temp_r3_2 << 0x10) == 0))) {
            M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        }
        temp_r0 = sp20 - 1;
        sp20 = temp_r0;
        if (temp_r0 != 0) {
            goto loop_12;
        }
block_115:
        return 0;
    case 1:
        if ((M2C_FIELD(temp_r3, u8 *, 0xEA4) != 0) && ((u32) M2C_FIELD(arg0, u16 *, 0x14) < 0x384U)) {
            *(s32 *)0x03001AF8 = 0;
        }
        M2C_FIELD(arg0, u16 *, 0x14) = 0x397U;
        if (Func_080199ec(arg0) == 0) {
            temp_r0_2 = M2C_FIELD(arg0, void **, 0);
            if (M2C_FIELD(temp_r0_2, u16 *, 8) == 0) {

            } else if (M2C_FIELD(temp_r0_2, u16 *, 0xA) == 0) {

            } else if (M2C_FIELD(temp_r3, u8 *, 0x12F8) != 0) {

            } else {
                Func_08018cac(temp_r0_2, 1U, (M2C_FIELD(temp_r0_2, u16 *, 8) * 4) - 8, (M2C_FIELD(temp_r0_2, u16 *, 0xA) * 8) - 0x10, 1);
                M2C_FIELD(temp_r3, u8 *, 0x12F8) = 1U;
            }
        } else {
            temp_r5 = M2C_FIELD(arg0, void **, 0);
            sp30 = (s32) M2C_FIELD(temp_r5, u16 *, 0xC);
            sp1C = (s32) M2C_FIELD(temp_r5, u16 *, 8);
            sp2C = (s32) M2C_FIELD(temp_r5, u16 *, 0xE);
            temp_r4 = M2C_FIELD(arg0, u16 *, 0x12);
            sp18 = (s32) M2C_FIELD(temp_r5, u16 *, 0xA);
            M2C_FIELD(temp_r3, u8 *, 0x12F8) = 0U;
            spC = (s32) temp_r4;
            Func_08016478(temp_r5);
            var_r4 = temp_r4;
            if ((M2C_FIELD(arg0, u16 *, 0x24) == 0) && ((M2C_FIELD(temp_r5, u16 *, 0xA) | M2C_FIELD(temp_r5, u16 *, 8)) != 0)) {
                Func_08016178(M2C_FIELD(temp_r5, u16 *, 0xC), M2C_FIELD(temp_r5, u16 *, 0xE), M2C_FIELD(temp_r5, u16 *, 8), M2C_FIELD(temp_r5, u16 *, 0xA));
                var_r4 = (u16) spC;
            }
            temp_r4_2 = (var_r4 + 1) & 0x1FF;
            if ((M2C_FIELD(temp_r3, u16 *, (temp_r4_2 * 2) + 0xEB0) != 0) && ((M2C_FIELD(temp_r5, u16 *, 0xA) | M2C_FIELD(temp_r5, u16 *, 8)) != 0)) {
                temp_r7 = M2C_FIELD(arg0, u16 *, 0x24);
                if (temp_r7 != 0) {
                    Func_08016178(M2C_FIELD(temp_r5, u16 *, 0xC), M2C_FIELD(temp_r5, u16 *, 0xE), M2C_FIELD(temp_r5, u16 *, 8), M2C_FIELD(temp_r5, u16 *, 0xA));
                } else {
                    temp_r1 = arg0 + 8;
                    spC = temp_r4_2;
                    Func_0801868c(
                        temp_r4_2, &sp30, &sp2C, &sp28,
                        &sp24, temp_r1, temp_r7);
                    temp_r1_2 = M2C_FIELD(temp_r5, u16 *, 0x16);
                    if (0x80 & temp_r1_2) {
                        if (sp18 != sp24) {
                            sp2C -= sp24 - sp18;
                        }
                        if (sp2C < 0) {
                            sp2C = (s32) temp_r7;
                        }
                    }
                    if (!(0x100 & temp_r1_2)) {
                        var_r3 = sp1C - sp28;
                        if (var_r3 < 0) {
                            var_r3 += 3;
                        }
                        sp30 += var_r3 >> 2;
                        Func_0801868c(
                            spC, &sp30, &sp2C, &sp28,
                            &sp24, temp_r1, 2);
                    }
                    M2C_FIELD(temp_r5, u16 *, 0xC) = (u16) sp30;
                    M2C_FIELD(temp_r5, u16 *, 0xE) = (u16) sp2C;
                    M2C_FIELD(temp_r5, u16 *, 8) = (u16) sp28;
                    M2C_FIELD(temp_r5, u16 *, 0xA) = (u16) sp24;
                }
                Func_080170f8(M2C_FIELD(temp_r5, u16 *, 0xC), M2C_FIELD(temp_r5, u16 *, 0xE), M2C_FIELD(temp_r5, u16 *, 8), M2C_FIELD(temp_r5, u16 *, 0xA));
            }
            M2C_FIELD(arg0, u16 *, 4) = (u16) M2C_FIELD(arg0, u16 *, 0x1E);
            M2C_FIELD(arg0, u16 *, 6) = 0U;
            M2C_FIELD(arg0, u16 *, 0x10) = 0U;
            Func_08003f3c((u32) M2C_FIELD(temp_r3, u16 *, 0x12B6));
            M2C_FIELD(temp_r3, u16 *, 0x12B6) = 0x63U;
        }
        goto block_77;
    case 2:
        if ((M2C_FIELD(temp_r3, u8 *, 0xEA4) != 0) && ((u32) M2C_FIELD(arg0, u16 *, 0x14) < 0x384U)) {
            *(s32 *)0x03001AF8 = 0;
        }
        if (Func_080199ec(arg0) != 0) {
            return 9;
        }
        M2C_FIELD(arg0, u16 *, 0x14) = 0x397U;
        goto block_77;
    case 5:
        if (M2C_FIELD(arg0, u16 *, 0x14) == 0) {
            var_r3_2 = 0x14;
block_65:
            M2C_FIELD(arg0, u16 *, 0x14) = var_r3_2;
        }
block_66:
        M2C_FIELD(temp_r3, u16 *, 0x12F6) = 0U;
        Func_0801999c(arg0);
        goto block_77;
    case 6:
        if (M2C_FIELD(arg0, u16 *, 0x14) == 0) {
            var_r3_2 = 0x78;
            goto block_65;
        }
        goto block_66;
    case 4:
        if (M2C_FIELD(arg0, u16 *, 0x14) == 0) {
            M2C_FIELD(arg0, u16 *, 0x14) = 0x3CU;
        }
        M2C_FIELD(temp_r3, u16 *, 0x12F6) = 0U;
        goto block_77;
    case 8:
        M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        M2C_FIELD(arg0, u16 *, 0x16) = M2C_FIELD(temp_r3, u16 *, (M2C_FIELD(arg0, u16 *, 0x12) * 2) + 0xEB0);
        Func_080167ac(arg0);
        goto block_77;
    case 9:
        M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        M2C_FIELD(arg0, u16 *, 0x18) = M2C_FIELD(temp_r3, u16 *, (M2C_FIELD(arg0, u16 *, 0x12) * 2) + 0xEB0);
        Func_080167ac(arg0);
        goto block_77;
    case 10:
        M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        var_r3_3 = M2C_FIELD(temp_r3, u16 *, (M2C_FIELD(arg0, u16 *, 0x12) * 2) + 0xEB0);
block_74:
        M2C_FIELD(arg0, u16 *, 0x1A) = var_r3_3;
        Func_080167ac(arg0);
        goto block_77;
    case 7:
        M2C_FIELD(arg0, u16 *, 0x18) = 0U;
        var_r3_3 = 0xA;
        M2C_FIELD(arg0, u16 *, 0x16) = 0xFU;
        goto block_74;
    case 15:
        M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        temp_r2_5 = M2C_FIELD(arg0, u16 *, 0x12);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0), u16 *, 0x12) = M2C_FIELD(temp_r3, u16 *, (temp_r2_5 * 2) + 0xEB0);
        M2C_FIELD(arg0, u16 *, 0x14) = 0xAU;
        M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((temp_r2_5 + 1) & 0x1FF);
        goto block_77;
    case 0:
    case 30:
        M2C_FIELD(arg0, u16 *, 0x20) = 1U;
        return 8;
    default:
        temp_r3_3 = M2C_FIELD(arg0, u16 *, 4);
        var_r2_2 = temp_r3_3 + 0x80;
        if (var_r2_2 < 0) {
            var_r2_2 = temp_r3_3 + 0x17F;
        }
        var_r5 = var_r2_2 >> 8;
        temp_r2_6 = M2C_FIELD(arg0, u16 *, 6);
        var_r3_4 = temp_r2_6 + 0x80;
        if (var_r3_4 < 0) {
            var_r3_4 = temp_r2_6 + 0x17F;
        }
        temp_sl = M2C_FIELD(*(void **)0x0200044C, u8 *, 0x0807380E);
        if (M2C_FIELD(temp_r3, u8 *, 0xEA4) != 0) {
            var_r5 += 8;
        }
        temp_r4_3 = M2C_FIELD(temp_r3, u16 *, (((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF) * 2) + 0xEB0);
        if (temp_r4_3 == 0xDE) {
            var_r3_5 = 0x4000;
            goto block_90;
        }
        if (temp_r4_3 == 0xDF) {
            var_r3_5 = 0x8000;
block_90:
            var_r7 |= var_r3_5;
            M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        }
        temp_r0_3 = M2C_FIELD(arg0, void **, 0);
        if (!(8 & M2C_FIELD(temp_r0_3, u16 *, 0x16)) && ((u32) var_r7 > 0x20U) && ((u32) temp_r4_3 > 0x20U) && ((u32) ((M2C_FIELD(((var_r7 - 0x20) << 5), u16 *, 0x08032224) + M2C_FIELD(((temp_r4_3 - 0x20) << 5), u16 *, 0x08032224)) << 0x10) <= 0xF0000U)) {
            var_r7 |= temp_r4_3 << 8;
            M2C_FIELD(arg0, u16 *, 0x12) = (u16) ((M2C_FIELD(arg0, u16 *, 0x12) + 1) & 0x1FF);
        }
        temp_r4_4 = Func_08018cac(temp_r0_3, (u32) var_r7, var_r5, var_r3_4 >> 8, 0);
        M2C_FIELD(arg0, u16 *, 0x22) = (u16) M2C_FIELD(*(void **)0x0200044C, u8 *, 0x08073808);
        if (temp_r4_4 != 0) {
            if (M2C_FIELD(temp_r3, u16 *, 0x12F4) != 0) {
                temp_r2_7 = M2C_FIELD(temp_r3, u16 *, 0x12F6);
                if (temp_r2_7 == 0) {
                    if (var_r7 != 0x20) {
                        spC = temp_r4_4;
                        Func_080f9010(M2C_FIELD(temp_r3, u16 *, 0x12F4) + (3 & var_r7));
                        M2C_FIELD(temp_r3, u16 *, 0x12F6) = (u16) temp_sl;
                    }
                } else {
                    M2C_FIELD(temp_r3, u16 *, 0x12F6) = (u16) (temp_r2_7 + 0xFFFF);
                }
            }
            var_r0 = temp_r4_4 << 8;
            if (var_r7 == 0x20) {
                var_r0 += M2C_FIELD(arg0, u16 *, (M2C_FIELD(arg0, u16 *, 0x10) * 2) + 8);
            }
            M2C_FIELD(arg0, u16 *, 4) = (u16) (M2C_FIELD(arg0, u16 *, 4) + var_r0);
        }
        if ((var_r7 == 0x20) && (M2C_FIELD(temp_r3, u8 *, 0xEA5) == 0)) {
            sp20 = 1;
        }
        goto block_109;
    }
}
