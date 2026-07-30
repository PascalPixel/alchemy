typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002dd8(s32);
s32 Func_080048b0(s32, s32);
u32 Func_08007308(void *);
u8 *Func_08017dd4(void *, s32, s32);
u16 Func_08017e88(s32, u16 *, u16, void *, s32, s32, s32 *);
void Func_080196c4(s32, s32, s32);
void Func_080198dc(void);
s32 Func_08019944(s32, s32);
void Func_08019bac(void *, s32);
u8 *Func_08077008(s32);
s32 Func_0808a5d0(s16, s32);

/*
 * Expand a localized text stream into the engine's circular command buffer.
 *
 * Printable glyphs are copied directly. Control codes inject names, numbers,
 * punctuation, formatting commands, and conditional separators while keeping
 * the 512-entry write cursor wrapped. A null source selects the current buffer
 * without parsing a new stream.
 */
s32 Func_08018038(s32 arg0, s32 arg1) {
    u16 *spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    u32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    void *sp2C;
    s32 sp30;
    s32 sp34;
    s32 expansion_index;
    volatile s32 expansion_path;
    u8 sp38[12];
    u8 sp44[16];
    u16 sp54[24];
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1_3;
    s32 temp_r2;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_4;
    s32 temp_r6_5;
    s32 temp_r6_6;
    s32 var_r0;
    u8 *var_r0_2;
    u8 *var_r0_4;
    u8 *var_r0_5;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r4_4;
    u16 *var_r1;
    u16 *var_r1_2;
    u16 *var_r1_4;
    u16 *var_r2;
    u16 *var_r2_2;
    u16 *var_r2_3;
    u16 temp_r3;
    u16 var_r0_3;
    u16 var_r2_4;
    u16 var_r2_5;
    u16 var_r6;
    u32 temp_r5;
    u32 temp_r5_5;
    u32 var_r4;
    u32 var_r4_2;
    u32 var_r4_3;
    u32 var_r7;
    u8 *var_r0_6;
    u8 *var_r1_3;
    void *temp_r1;
    void *temp_r1_2;

    sp30 = arg0;
    temp_r1 = M2C_FIELD((void *)0x03001E8C, void **, 0);
    sp2C = temp_r1;
    sp28 = 1;
    sp24 = 0;
    temp_r3 = M2C_FIELD(temp_r1, u16 *, 0x12B2);
    sp14 = 1;
    sp20 = (s32) temp_r3;
    var_r6 = temp_r3;
    var_r7 = 0;
    temp_r1_2 = temp_r1 + 0xEB0;
    sp1C = 0;
    sp18 = 0;
    sp34 = 0;
    expansion_index = 0;
    sp10 = 0;
    if (sp30 == -1) {
        sp20 = (s32) M2C_FIELD(sp2C, u16 *, 0x12B4);
    } else {
        temp_r1_3 = Func_080048b0(0x32, 0x140);
        M2C_FIELD((void *)0x040000D4, s32 *, 0) = 0x08015430;
        M2C_FIELD((void *)0x040000D4, s32 *, 4) = temp_r1_3;
        M2C_FIELD((void *)0x040000D4, s32 *, 8) = (s32) (0x84000000 | (0x140U >> 2));
        Func_08019bac(sp38, sp30);
        spC = sp54;
loop_3:
        temp_r5 = var_r7;
        var_r7 = Func_08007308(sp38);
        if (var_r7 > 0xFFU) {
            var_r7 = 0x40;
        }
        if (sp10 != 0) {
            if (var_r7 <= 0x1FU) {
                switch (var_r7) {
                case 18:
                case 29:
                case 17:
                    Func_08007308(sp38);
                    break;
                case 19:
                    Func_08007308(sp38);
                    var_r0 = 3;
block_36:
                    Func_08019944(var_r0, arg1);
                    break;
                default:
                    sp14 = 0;
                    break;
                case 22:
                    var_r0 = 5;
                    goto block_36;
                case 20:
                    Func_08007308(sp38);
                    var_r0 = 2;
                    goto block_36;
                case 21:
                    var_r0 = 4;
                    goto block_36;
                case 23:
                    var_r0 = 6;
                    goto block_36;
                case 1:
                    sp14 = 0;
                    var_r7 = 2;
                    break;
                }
            }
        } else {
            if ((M2C_FIELD(sp2C, u8 *, 0x12FA) != 0) && (sp28 == 0) && (var_r7 != 0xDE) && (var_r7 != 0xDF)) {
                M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 5;
                var_r6 = (var_r6 + 1) & 0x1FF;
            }
            if ((M2C_FIELD(sp2C, u8 *, 0x12FB) != 0) && (sp28 == 0) && (var_r7 != 0xDE) && (var_r7 != 0xDF)) {
                if ((temp_r5 <= 0x100U) && (temp_r5 > 0x7FU) && (temp_r5 != 0xDE) && (temp_r5 != 0xDF) && (temp_r5 != 0x20) && (temp_r5 != 0xA5) && (temp_r5 != 0xA1) && (temp_r5 != 0xA4)) {
                    M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0xDE;
                    var_r6 = (var_r6 + 1) & 0x1FF;
                }
            }
            if (var_r7 > 0x1FU) {
                if ((M2C_FIELD(sp2C, u8 *, 0x12FA) != 0) && ((var_r7 == 0x20) || (sp1C > 0xAU))) {
                    temp_r6 = (var_r6 + 1) & 0x1FF;
                    M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x2E;
                    temp_r6_2 = (temp_r6 + 1) & 0x1FF;
                    M2C_FIELD(temp_r1_2, u16 *, temp_r6 * 2) = 0x2E;
                    M2C_FIELD(temp_r1_2, u16 *, temp_r6_2 * 2) = 0x2E;
                    var_r6 = (temp_r6_2 + 1) & 0x1FF;
                    sp10 = 1;
                    if (sp1C > 0xAU) {
                        var_r7 = 0x20;
                    }
                }
                if (var_r7 == 0x22) {
                    temp_r2 = sp24 ^ 1;
                    sp24 = temp_r2;
                    if (temp_r2 != 0) {
                        var_r7 = 0x8E;
                    }
                }
                M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = (s16) var_r7;
                var_r6 = (var_r6 + 1) & 0x1FF;
                sp28 = 0;
            } else {
                expansion_index = 0;
                if (var_r7 == 0x14) {
                    temp_r5_2 = Func_08007308(sp38) - 1;
                    expansion_index = temp_r5_2;
                    expansion_path = 0x14;
                    Func_080196c4((Func_08019944(2, arg1) & 0x1FF) + 0x182, (s32) spC, 0x18);
                    if (expansion_path != 0x14)
                        expansion_index = temp_r5_2;
                    var_r1 = spC;
                    goto block_147;
                }
                if (var_r7 <= 0x14U) {
                    switch (var_r7) {
                    case 1:
block_157:
                        sp28 = 1;
                        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = (s16) var_r7;
                        var_r6 = (var_r6 + 1) & 0x1FF;
                        if ((var_r7 == 0x73) || (var_r7 == 0x53)) {
                            var_r3 = 1;
                        } else {
                            var_r3 = 0;
                        }
                        sp34 = var_r3;
                        break;
                    case 3:
                        goto block_157;
                    case 17:
                        var_r0_2 = Func_08077008(Func_08007308(sp38) - 1);
                        var_r1 = sp54;
                        var_r2 = var_r1;
                        var_r4 = 0;
                        do {
                            var_r4 += 1;
                            *var_r2 = (u16) *var_r0_2;
                            var_r0_2 += 1;
                            var_r2 += 2;
                        } while (var_r4 <= 0xEU);
block_146:
block_147:
                        var_r0_3 = Func_08017e88(
                            0, var_r1, var_r6, temp_r1_2,
                            expansion_index, sp24, &sp34);
                        goto block_148;
                    case 16:
                        var_r0_4 = Func_08077008(*(s32 *)0x02000434);
                        var_r1 = sp54;
                        var_r2_2 = var_r1;
                        var_r4_2 = 0;
                        do {
                            var_r4_2 += 1;
                            *var_r2_2 = (u16) *var_r0_4;
                            var_r0_4 += 1;
                            var_r2_2 += 2;
                        } while (var_r4_2 <= 0xEU);
                        goto block_146;
                    case 18:
                        temp_r5_3 = Func_08007308(sp38) - 1;
                        expansion_index = temp_r5_3;
                        var_r0_5 = Func_08077008(Func_08019944(1, arg1));
                        var_r1 = sp54;
                        var_r2_3 = var_r1;
                        var_r4_3 = 0;
                        do {
                            var_r4_3 += 1;
                            *var_r2_3 = (u16) *var_r0_5;
                            var_r0_5 += 1;
                            var_r2_3 += 2;
                        } while (var_r4_3 <= 0xEU);
                        goto block_147;
                    case 19:
                        temp_r5_4 = Func_08007308(sp38) - 1;
                        expansion_index = temp_r5_4;
                        expansion_path = 0x13;
                        Func_080196c4(Func_08019944(3, arg1) + 0x741, (s32) spC, 0x18);
                        if (expansion_path != 0x13)
                            expansion_index = temp_r5_4;
                        var_r1 = spC;
                        goto block_147;
                    }
                } else {
                    switch (var_r7) {
                    case 25:
                        if (sp18 != 0) {
                            if (sp34 != 0) {
                                M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x65;
                                var_r6 = (var_r6 + 1) & 0x1FF;
                            }
                            M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x73;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                        }
                        break;
                    case 23:
                        Func_080196c4(Func_0808a5d0((s16) Func_08019944(6, arg1), 1) + 0x99B, (s32) spC, 0x18);
                        var_r1_2 = spC;
                        var_r2_4 = *var_r1_2;
                        var_r0_3 = var_r6;
                        if (var_r2_4 != 0) {
                            do {
                                M2C_FIELD(temp_r1_2, u16 *, var_r0_3 * 2) = var_r2_4;
                                var_r1_2 += 2;
                                var_r2_4 = *var_r1_2;
                                var_r0_3 = (var_r0_3 + 1) & 0x1FF;
                            } while (var_r2_4 != 0);
                        }
block_148:
                        var_r6 = var_r0_3;
                        break;
                    case 24:
                        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x8F;
                        temp_r6_3 = (var_r6 + 1) & 0x1FF;
                        M2C_FIELD(temp_r1_2, u16 *, temp_r6_3 * 2) = 0x2D;
                        var_r6 = (temp_r6_3 + 1) & 0x1FF;
                        break;
                    case 26:
                        temp_r0 = (Func_08007308(sp38) - 1) * 2;
                        temp_r6_4 = (var_r6 + 1) & 0x1FF;
                        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = temp_r0 + 0x80;
                        M2C_FIELD(temp_r1_2, u16 *, temp_r6_4 * 2) = temp_r0 + 0x81;
                        var_r6 = (temp_r6_4 + 1) & 0x1FF;
                        break;
                    case 27:
                        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x27;
                        var_r6 = (var_r6 + 1) & 0x1FF;
                        if (sp34 == 0) {
                            M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = 0x73;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                        }
                        break;
                    case 30:
                    case 2:
                        sp14 = 0;
                        break;
                    case 29:
                        temp_r6_5 = (var_r6 + 1) & 0x1FF;
                        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = (s16) var_r7;
                        M2C_FIELD(temp_r1_2, u16 *, temp_r6_5 * 2) = Func_08007308(sp38) + 0xFFFF;
                        var_r6 = (temp_r6_5 + 1) & 0x1FF;
                        break;
                    case 22:
                        temp_r0_2 = Func_08019944(5, arg1);
                        var_r3_2 = temp_r0_2;
                        if (temp_r0_2 < 0) {
                            var_r3_2 = 0 - temp_r0_2;
                        }
                        sp18 = 1;
                        if (var_r3_2 <= 1) {
                            sp18 = 0;
                        }
                        var_r4_4 = Func_08017dd4(sp44, temp_r0_2, 0) - sp44;
                        if (var_r4_4 == 0x10) {

                        } else if (sp44[var_r4_4] == 0) {

                        } else {
                            var_r0_6 = sp44 + var_r4_4;
                            var_r1_3 = var_r0_6;
loop_124:
                            M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = (s16) *var_r1_3;
                            var_r4_4 += 1;
                            var_r1_3 += 1;
                            var_r6 = (var_r6 + 1) & 0x1FF;
                            if (var_r4_4 == 0x10) {

                            } else {
                                var_r0_6 += 1;
                                if (*var_r0_6 == 0) {

                                } else {
                                    goto loop_124;
                                }
                            }
                        }
                        break;
                    default:
                        Func_080196c4(Func_08019944(4, arg1) + 0x333, (s32) spC, 0x18);
                        var_r1_4 = spC;
                        var_r2_5 = *var_r1_4;
                        var_r0_3 = var_r6;
                        if (var_r2_5 != 0) {
                            do {
                                M2C_FIELD(temp_r1_2, u16 *, var_r0_3 * 2) = var_r2_5;
                                var_r1_4 += 2;
                                var_r2_5 = *var_r1_4;
                                var_r0_3 = (var_r0_3 + 1) & 0x1FF;
                            } while (var_r2_5 != 0);
                        }
                        goto block_148;
                    }
                }
            }
        }
        temp_r5_5 = sp1C + 1;
        sp1C = temp_r5_5;
        if ((sp14 != 0) && (temp_r5_5 <= 0x1FFU)) {
            goto loop_3;
        }
        M2C_FIELD(temp_r1_2, u16 *, var_r6 * 2) = (s16) var_r7;
        temp_r6_6 = (var_r6 + 1) & 0x1FF;
        M2C_FIELD(temp_r1_2, u16 *, temp_r6_6 * 2) = 0;
        M2C_FIELD(sp2C, s16 *, 0x12B2) = (s16) ((temp_r6_6 + 1) & 0x1FF);
        Func_08002dd8(0x32);
        M2C_FIELD(sp2C, u16 *, 0x12B4) = (u16) sp20;
    }
    if (arg1 != 0) {
        Func_080198dc();
    }
    return sp20;
}
