typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Work_80190000;

void Func_08002dd8(s32);
void Func_08002df0(void *);
void Func_080030f8(s32);
void Func_08003f3c(u32);
void *Func_080040b4(s32);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void *Func_080048f4(s32, s32);
s16 *Func_08004938(s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_08016738(void);
s32 Func_08017364(void);
void Func_08018efc(void *, s32, s32, s32, s32);
void Func_08019000(struct Work_80190000 *, s32, s32, s32, s32);
void Func_0801e318(void);
void Func_0801e71c(s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_0801e9d4(s32, s32, void *, s32, s32);
void Func_08021848(void);
void Func_080219c8(s32);
void Func_08021a18(void *);
s32 Func_08021af0(s32, void *);
s32 Func_08021b30(s32, void *);
s32 Func_08021b80(u32, void *);
s32 Func_08021c64(void *, s32);
void *Func_08021e48(s32, s32, s32);
u32 Func_08021e6c(s32);
s32 Func_0802281c(u16 *);
s32 Func_08023178(s16 *, s32, u32);
s32 Func_08023e70(s32, s32);
s32 Func_08024934(s32, s32, void *);
s32 Func_08025180(s32, s32);
s32 Func_08025200(s32, u16 *);
s32 Func_0802592c(s32, s16 *, s32);
s32 Func_08026080(s32, s32, s32, s32);
s32 Func_08026fa8(void);
s32 Func_0802706c(const u8 *);
void Func_080270ac(void);
void Func_080270d8(void);
u8 *Func_08077008(s32);
u8 *Func_08077018(s32);
u8 *Func_08077080(s32);
s32 Func_080770c0(s32);
u16 *Func_080771e0(s32);
s32 Func_080771e8(s32, s32);
s32 Func_08077208(s32, s32, s32);
s32 Func_080b5090(s32, s16 *);
void Func_080b50d0(s32);
void Func_080b50e0(u16 *, s32);
void Func_080b5130(s32, void *);
void Func_080f9010(s32);

/*
 * Build and edit the sequence of 16-byte output records requested by arg1.
 *
 * Each entry drives the selector interface, dispatches the chosen operation,
 * records its cost and operation-specific details, then advances to the next
 * identifier.  The shared-session handshake is established before editing and
 * released during the common cleanup path.
 *
 * Owner span: 0x08027114-0x08028194.  Long branches to labels inside that span
 * are continuations of this owner, not separately callable C functions.
 */
s32 Func_08027114(void *arg0, u16 *arg1, s32 arg2) {
    u8 *state;
    u16 selection[2];
    s32 sp4;
    u16 *sp8;
    u16 *spC;
    s32 sp10;
    u16 *sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    u8 **sp24;
    u8 *sp28;
    s32 sp2C;
    u8 *sp30;
    u8 *sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    u8 *sp48;
    u8 *sp4C;
    s32 sp50;
    u16 *sp54;
    u8 *sp58;
    s16 *temp_r0_9;
    s16 *temp_r3_2;
    s16 *temp_r6;
    s16 *var_r6_3;
    s32 *var_r3;
    u8 *temp_fp;
    u8 *temp_fp_2;
    u8 *temp_fp_3;
    u8 *temp_fp_4;
    u8 *temp_fp_5;
    u8 *temp_r0;
    s32 temp_r0_10;
    u8 *temp_r0_11;
    s32 temp_r0_13;
    s32 temp_r0_17;
    s32 temp_r0_18;
    u8 *temp_r0_22;
    u8 *temp_r0_23;
    u8 *temp_r0_8;
    s32 temp_r2;
    s32 temp_r2_4;
    s32 temp_r2_7;
    s32 temp_r3_4;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r4_3;
    s32 temp_r5;
    s32 temp_r5_7;
    u8 *temp_r5_9;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_5;
    s32 temp_r6_6;
    s32 temp_r6_9;
    s32 temp_r7_2;
    u8 *temp_r9;
    u8 *temp_sl;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r1;
    s32 var_r1_2;
    s32 var_r1_3;
    s32 var_r1_4;
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r2_4;
    s32 var_r2_5;
    s32 var_r3_3;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_5;
    s32 var_r4_6;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r6_4;
    s32 var_r6_9;
    s32 var_r7_2;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r8;
    s8 *temp_r3_6;
    s8 *var_r3_2;
    s8 *var_r7_5;
    s8 *var_r7_6;
    u16 *temp_r0_15;
    u16 *temp_r1_12;
    u16 *temp_r3_7;
    u8 *var_r0_3;
    u8 *var_r0_4;
    u16 *var_r2_6;
    u16 *var_r3_5;
    u8 *var_r6_5;
    u16 *var_r6_6;
    u16 *var_r6_7;
    u16 *var_r7;
    u16 temp_r0_6;
    u16 temp_r0_7;
    u16 temp_r2_2;
    u16 temp_r2_9;
    u16 var_r5_5;
    u16 var_r5_6;
    u16 var_r6_8;
    u32 var_r6;
    u32 var_r6_2;
    u8 *temp_r0_19;
    u8 *temp_r5_8;
    u8 *temp_r6_4;
    u8 *temp_r8_3;
    u8 *temp_sl_2;
    u8 *var_r7_7;
    u8 temp_r0_4;
    u8 temp_r3_3;
    u8 temp_r6_7;
    u8 temp_r6_8;
    u8 temp_r8;
    u8 temp_sl_3;
    u8 var_r3_4;
    u8 var_r4_4;
    u8 *temp_r0_12;
    u8 *temp_r0_14;
    u8 *temp_r0_16;
    u8 *temp_r0_20;
    u8 *temp_r0_21;
    u8 *temp_r0_2;
    u8 *temp_r0_3;
    u8 *temp_r0_5;
    u8 *temp_r1;
    u8 *temp_r1_10;
    u8 *temp_r1_11;
    u8 *temp_r1_2;
    u8 *temp_r1_3;
    u8 *temp_r1_4;
    u8 *temp_r1_5;
    u8 *temp_r1_6;
    u8 *temp_r1_7;
    u8 *temp_r1_8;
    u8 *temp_r1_9;
    u8 *temp_r2_3;
    u8 *temp_r2_5;
    u8 *temp_r2_6;
    u8 *temp_r2_8;
    u8 *temp_r3;
    u8 *temp_r3_5;
    u8 *temp_r5_10;
    u8 *temp_r5_11;
    u8 *temp_r5_12;
    u8 *temp_r5_2;
    u8 *temp_r5_3;
    u8 *temp_r5_4;
    u8 *temp_r5_5;
    u8 *temp_r5_6;
    u8 *temp_r7;
    u8 *temp_r8_2;
    u8 *var_r1_5;
    u8 *var_r1_6;
    u8 *var_r2_3;

    sp58 = (u8 *)arg0;
    sp44 = 0x100;
    sp54 = arg1;
    sp50 = arg2;
    sp3C = 0;
    sp38 = 0;
    temp_r0 = Func_080040b4(0x400);
    sp34 = temp_r0;
    sp30 = Func_080040b4(0x200);
    temp_r0_2 = Func_080048f4(0x39, 0x104);
    sp24 = &state;
    *sp24 = temp_r0_2;
    Func_08021c64(temp_r0, 0U);
    Func_08021a18((void *)0x06006000);
    Func_080219c8(0x06006680);
    Func_08021848();
    temp_r0_3 = *(void **)0x03001E74;
    var_r3 = (s32 *)((u8 *)*sp24 + 0xE4);
    var_r2 = 7;
    do {
        var_r2 -= 1;
        *var_r3 = 0x80000000;
        var_r3 += 1;
    } while (var_r2 >= 0);
    var_r3_2 = *sp24 + 0x24;
    var_r2_2 = 2;
    do {
        var_r2_2 -= 1;
        *var_r3_2 = 0;
        var_r3_2 += 1;
    } while (var_r2_2 >= 0);
    temp_r1 = *sp24;
    M2C_FIELD(temp_r1, s32, 0x28) = 0;
    M2C_FIELD(temp_r1, s32, 0x2C) = 0;
    M2C_FIELD(temp_r1, s32, 0x3C) = 0;
    M2C_FIELD(temp_r1, s32, 0x40) = 0;
    M2C_FIELD(temp_r1, s32, 0x50) = 0;
    M2C_FIELD(temp_r1, s32, 0x48) = 0;
    M2C_FIELD(temp_r1, s32, 0x44) = 0;
    M2C_FIELD(temp_r1, s32, 0x4C) = -1;
    if (M2C_FIELD(temp_r0_3, u8, 0x44) != 0) {
        M2C_FIELD(temp_r1, s32, 0x50) = 1;
        M2C_FIELD((void *)0x02002224, s16, 8) = 0x56;
        M2C_FIELD((void *)0x02002224, s16, 0xA) = 0x53;
        M2C_FIELD((void *)0x02002224, s16, 0xC) = 0x53;
        var_r5 = 0;
        M2C_FIELD((void *)0x02002224, s16, 0xE) = 0x54;
loop_23:
        temp_r0_4 = M2C_FIELD(temp_r0_3, u8, 0x52);
        temp_r2 = (1 ^ M2C_FIELD(temp_r0_3, u8, 0x50)) * 0x18;
        temp_r1_2 = (u8 *)(0x02002024 + temp_r2);
        if (temp_r0_4 != 0) {
            sp50 = -1;
            goto block_25;
        }
        if ((3 & *(u16 *)0x03001F64) != 3) {
            var_r5 += 1;
            if (var_r5 > 0x18) {
                sp50 = -1;
                M2C_FIELD(*sp24, s32, 0x50) = (s32) temp_r0_4;
            } else {
                goto block_22;
            }
        } else {
            temp_r2_2 = M2C_FIELD(temp_r1_2, u16, 8);
            var_r5 = 0;
            if (((temp_r2_2 != 0x56) || (M2C_FIELD(temp_r1_2, u16, 0xA) != 0x53) || (M2C_FIELD(temp_r1_2, u16, 0xC) != 0x53) || (M2C_FIELD(temp_r1_2, u16, 0xE) != 0x54)) && ((temp_r2_2 != 0x45) || (M2C_FIELD(temp_r1_2, u16, 0xA) != 0x44) || (M2C_FIELD(temp_r1_2, u16, 0xC) != 0x56) || (M2C_FIELD(temp_r1_2, u16, 0xE) != 0x53))) {
                if ((M2C_FIELD((u8 *)0x02002024, u16, temp_r2) != 0x45) || (M2C_FIELD(temp_r1_2, u16, 2) != 0x58) || (M2C_FIELD(temp_r1_2, u16, 4) != 0x45) || (M2C_FIELD(temp_r1_2, u16, 6) != 0x43)) {
                    sp50 = -1;
block_25:
                    M2C_FIELD(*sp24, s32, 0x50) = 0;
                } else {
block_22:
                    Func_080030f8(1U);
                    goto loop_23;
                }
            }
        }
    }
    Func_080041d8((const void *)0x08026E81, 0xC80);
outer_loop:
    Func_080270ac();
    temp_r0_5 = *sp24;
    M2C_FIELD(temp_r0_5, s8, 0x26) = 0;
    M2C_FIELD(temp_r0_5, s32, 0xE0) = -1;
    M2C_FIELD(temp_r0_5, s32, 0xD8) = 0;
    if (Func_080770c0(0x16E) != 0) {
        temp_r2_3 = *sp24;
        M2C_FIELD(temp_r2_3, s32, 0xD8) = 1;
        M2C_FIELD(temp_r2_3, s32, 0xDC) = 0x3C;
    }
    if (sp50 > 0) {
        var_r6 = Func_08021e6c(0);
    } else {
        var_r6 = 0xE;
    }
    if (var_r6 == 7) {
        temp_r6 = Func_08004938(0xC);
        if ((*(u8 *)0x03001F54 == 0) || (var_r0 = 2, ((*(s32 *)0x03001AE8 & 8) == 0))) {
            var_r0 = 1;
        }
        temp_r5 = Func_080b5090(var_r0, temp_r6);
        Func_080030f8(1U);
        Func_08023178(temp_r6, temp_r5, (u16) *temp_r6);
        Func_08002df0(temp_r6);
        goto outer_loop;
    }
    if (var_r6 == 4) {
        temp_r0_6 = Func_08026fa8();
        if (temp_r0_6 == 0) {
            sp4C = sp58;
            sp50 = 1;
            M2C_FIELD(sp58, u16, 0) = (u16) *sp54;
            M2C_FIELD(sp4C, u16, 4) = 0x7FFEU;
            M2C_FIELD(sp58, s16, 6) = 0x63;
            M2C_FIELD(sp4C, u16, 8) = temp_r0_6;
            M2C_FIELD(sp4C, u16, 0xA) = 0x100U;
            goto cleanup;
        } else {
            goto outer_loop;
        }
    }
    if (var_r6 != 0xE) {
        goto cleanup;
    }
    Func_080f9010(0x9A);
    var_r1 = 0;
    sp2C = 0;
    if (sp50 <= 0) {
        goto cleanup;
    }
    sp1C = var_r1;
    sp18 = var_r1;
    sp20 = var_r1;
    sp14 = selection;
process_entry:
    if (sp2C == 0) {
        Func_080b5130(0, *sp24 + 0x54);
    } else {
        var_r2_3 = sp1C + *sp24 + 0x50;
        var_r1_2 = 3;
        do {
            var_r1_2 -= 1;
            M2C_FIELD(var_r2_3, u8, 4) = (u8) M2C_FIELD(var_r2_3, u8, 0);
            var_r2_3 += 1;
        } while (var_r1_2 >= 0);
    }
    sp4C = sp58 + sp18;
    temp_r0_7 = M2C_FIELD(sp54, u16, sp20);
    sp40 = (s32) temp_r0_7;
    temp_r0_8 = Func_08077008((s32) temp_r0_7);
    sp48 = temp_r0_8;
    temp_r5_2 = *(void **)0x03001F34;
    M2C_FIELD(temp_r5_2, s32, 0xE0) = sp40;
    M2C_FIELD(temp_r5_2, s32, 0x40) = 0;
    temp_r5_3 = temp_r5_2 + 0x18;
    M2C_FIELD(temp_r5_3, s32, 4) = 0x80000400;
    M2C_FIELD(temp_r5_3, s32, 8) = 0;
    M2C_FIELD(temp_r5_3, u16, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r5_3, u16, 8)) | (Func_08021b80((u32) M2C_FIELD(temp_r0_8, u8, 0x128), sp30) & 0x3FF));
    M2C_FIELD(temp_r5_3, u16, 6) = (u16) (0xFFFFFE00 & M2C_FIELD(temp_r5_3, u16, 6));
    M2C_FIELD(temp_r5_3, u8, 4) = 0x80;
    M2C_FIELD(temp_r5_3, u8, 9) = (u8) ((0xF & M2C_FIELD(temp_r5_3, u8, 9)) | 0xE0);
    M2C_FIELD(*sp24, s8, 0x26) = 1;
interaction_loop:
    Func_0801e318();
    temp_r1_3 = *(u8 **)0x03001F34;
    M2C_FIELD(temp_r1_3, s8, 0x24) = 0;
    M2C_FIELD(temp_r1_3, s32, sp1C + 0xE4) = 0x80000000;
    M2C_FIELD(sp14, u16, 0) = sp40;
    M2C_FIELD(sp14, s16, 2) = 0xFF;
    Func_080b50e0(sp14, 1);
    Func_0802281c(sp14);
    Func_080030f8(1U);
    var_r6_2 = Func_08021e6c(1);
    Func_080030f8(1U);
    if (var_r6_2 == -2U) {
        temp_r0_9 = Func_08004938(0xC);
        temp_r6_2 = Func_080b5090(1, temp_r0_9);
        M2C_FIELD(*sp24, s8, 0x26) = 0;
        Func_080030f8(1U);
        Func_08023178(temp_r0_9, temp_r6_2, (u16) sp40);
        M2C_FIELD(*sp24, s8, 0x26) = 1;
        Func_08002df0(temp_r0_9);
        goto process_entry;
    }
    Func_080b50e0(sp14, 0);
    if (var_r6_2 == -1U) {
        var_r0_2 = sp2C;
        if (var_r0_2 == 0) {
            goto check_outer_loop;
        }
        temp_r0_10 = var_r0_2 - 1;
        sp2C = temp_r0_10;
        sp1C = temp_r0_10 * 4;
        sp18 = temp_r0_10 * 0x10;
        sp20 = temp_r0_10 * 2;
        goto process_entry;
    }
    temp_r5_4 = *(u8 **)0x03001F34;
    if (M2C_FIELD(temp_r5_4, s32, 0x4C) == 0) {
        var_r6_2 = 3;
    }
    M2C_FIELD(temp_r5_4, s32, 8) = 0;
    M2C_FIELD(temp_r5_4, s32, 4) = 0x80002400;
    M2C_FIELD(temp_r5_4, u16, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r5_4, u16, 8)) | (Func_08021c64(sp34, var_r6_2) & 0x3FF));
    if (var_r6_2 == 0xF) {
        var_r2_4 = 0xFFFFFE00 & M2C_FIELD(temp_r5_4, u16, 6);
        var_r3_3 = 0x80;
    } else {
        var_r2_4 = 0xFFFFFE00 & M2C_FIELD(temp_r5_4, u16, 6);
        var_r3_3 = 0x60;
    }
    M2C_FIELD(temp_r5_4, u16, 6) = (u16) (var_r2_4 | var_r3_3);
    M2C_FIELD(temp_r5_4, u8, 4) = 0x88;
    M2C_FIELD(*sp24, s8, 0x24) = 1;
    if (var_r6_2 > 0x10U) {
        goto finish_entry;
    }
    switch (var_r6_2) {
    case 0:
        temp_r0_11 = Func_080162d4(0xBU, 0x11U, 0xBU, 3U, 6);
        Func_0801e7c0(0x1F, temp_r0_11, 0x10, 0);
        temp_r1_4 = *sp24;
        M2C_FIELD(temp_r1_4, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_4, u16, 6)) | 0x40);
        Func_080f9010(0x70);
        temp_r6_3 = Func_08026080(sp40, 1U, 1U, 0);
        Func_08016418(temp_r0_11, 1);
        if (temp_r6_3 == -1) {
            goto interaction_loop;
        }
        sp3C = 0;
        sp44 = temp_r6_3;
        M2C_FIELD(sp4C, s16, 0xC) = 1;
        goto finish_entry;
    case 1:
        Func_080f9010(0x70);
        temp_r3 = *(u8 **)0x03001F34;
        M2C_FIELD(temp_r3, s32, 0x34) = 0;
        M2C_FIELD(temp_r3, s32, 0x30) = 0;
        M2C_FIELD(temp_r3, s32, 0x38) = 0;
loop_72:
        temp_r1_5 = *sp24;
        temp_r3_2 = (s16 *)(temp_r1_5 + 0x96);
        M2C_FIELD(temp_r1_5, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_5, u16, 6)) | 0x30);
        temp_r0_12 = temp_r1_5 + 0x74;
        var_r5_2 = 0x3FFF & M2C_FIELD(sp48, u16, 0x58);
        var_r4 = 0;
        var_r1_3 = 0;
        if (var_r5_2 != 0) {
            var_r6_3 = temp_r3_2;
            var_r7 = (u16 *)(sp48 + 0x58);
loop_74:
            sp10 = var_r1_3;
            sp4 = var_r4;
            if (0x80 & M2C_FIELD(Func_08077080(var_r5_2), u8, 1)) {
                *(temp_r0_12 + var_r4) = (s8) var_r1_3;
                *var_r6_3 = (s16) var_r5_2;
                var_r4 += 1;
                var_r6_3 += 1;
            }
            var_r1_3 += 1;
            if (var_r1_3 != 0x20) {
                var_r7 += 2;
                var_r5_2 = 0x3FFF & *var_r7;
                if (var_r5_2 != 0) {
                    goto loop_74;
                }
            }
        }
        *(temp_r0_12 + var_r4) = 0;
        temp_r3_2[var_r4] = 0;
        temp_r0_13 = Func_0802592c(sp40, temp_r3_2, var_r4);
        if (temp_r0_13 == -1) {
            goto interaction_loop;
        }
        temp_r2_4 = 0x3FFF & M2C_FIELD(sp48, u16, (*(temp_r0_12 + temp_r0_13) * 4) + 0x58);
        sp38 = temp_r2_4;
        temp_r6_4 = Func_08077080(temp_r2_4);
        temp_r5_5 = *sp24;
        temp_r8 = M2C_FIELD(temp_r6_4, u8, 8);
        temp_r7 = *(void **)0x03001E8C;
        temp_sl = Func_080040b4(0x80);
        temp_fp = Func_080162d4(8U, 0x11U, 0x12U, 3U, 6);
        temp_r0_14 = *sp24;
        M2C_FIELD(temp_r0_14, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r0_14, u16, 6)) | 0x28);
        temp_r5_6 = temp_r5_5 + 0xC;
        M2C_FIELD(temp_r5_6, s32, 4) = 0x40000400;
        M2C_FIELD(temp_r5_6, s32, 8) = 0;
        M2C_FIELD(temp_r5_6, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r5_6, u16, 6)) | (((M2C_FIELD(temp_fp, u16, 0xC) * 8) + 8) & 0x1FF));
        M2C_FIELD(temp_r5_6, u8, 4) = (u8) ((M2C_FIELD(temp_fp, u16, 0xE) * 8) + 4);
        M2C_FIELD(temp_r5_6, u16, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r5_6, u16, 8)) | (Func_08021b30(sp38, temp_sl) & 0x3FF));
        M2C_FIELD(*sp24, s8, 0x25) = 1;
        M2C_FIELD(temp_r7, s8, 0xEA7) = 5;
        if ((s32) M2C_FIELD(temp_r6_4, u8, 9) > (s32) M2C_FIELD(sp48, s16, 0x3A)) {
            Func_0801e71c(2);
        } else if (M2C_FIELD(sp48, u8, 0x13D) != 0) {
            Func_0801e71c(9);
        }
        Func_0801e7c0(sp38 + 0x333, temp_fp, 0x10, 0);
        Func_0801e9d4((s32) M2C_FIELD(temp_r6_4, u8, 9), 2, temp_fp, 0x68, 0);
        M2C_FIELD(temp_r7, s8, 0xEA7) = 0xF;
        Func_0801e71c(0xF);
        Func_08019000((struct Work_80190000 *) temp_fp, 0xF01F, 0xB, 0, 0);
        Func_08019000((struct Work_80190000 *) temp_fp, 0xF01E, 0xC, 0, 0);
        temp_r3_3 = M2C_FIELD(temp_r6_4, u8, 2);
        if (temp_r3_3 != 4) {
            Func_08019000((struct Work_80190000 *) temp_fp, temp_r3_3 + 0x5001, 0xF, 0, 0);
        }
        M2C_FIELD(sp4C, s16, 0xC) = (s16) temp_r8;
        Func_080f9010(0x70);
        var_r6_4 = Func_08026080(sp40, M2C_FIELD(temp_r6_4, u8, 0), temp_r8, Func_0802706c(temp_r6_4));
        M2C_FIELD(*sp24, s8, 0x25) = 0;
        Func_08003f3c((u32) temp_sl);
        Func_08016418(temp_fp, 1);
        if (var_r6_4 == -1) {
            goto loop_72;
        }
        sp3C = 1;
block_91:
        sp44 = var_r6_4;
        break;
    case 16:
        Func_080f9010(0x70);
        temp_r2_5 = *(u8 **)0x03001F34;
        M2C_FIELD(temp_r2_5, s32, 0x34) = 0;
        M2C_FIELD(temp_r2_5, s32, 0x30) = 0;
        M2C_FIELD(temp_r2_5, s32, 0x38) = 0;
loop_93:
        temp_r1_6 = *sp24;
        M2C_FIELD(temp_r1_6, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_6, u16, 6)) | 0x50);
        Func_0802281c(sp14);
        temp_r6_5 = Func_08024934(0, 0, *sp24 + sp1C + 0x54);
        Func_0802281c(sp14);
        if (temp_r6_5 == -1) {
            goto interaction_loop;
        }
        sp3C = 6;
        sp38 = temp_r6_5;
        Func_080030f8(1U);
        temp_r8_2 = *sp24 + 0xC;
        temp_r0_15 = Func_080771e0(temp_r6_5);
        temp_sl_2 = Func_08077080((s32) M2C_FIELD(temp_r0_15, u16, 0));
        sp28 = Func_080040b4(0x80);
        temp_r4 = sp1C + 0x54;
        temp_fp_2 = Func_080162d4(0xAU, 0x11U, 0x11U, 3U, sp3C);
        var_r7_2 = 0;
        if ((u32) M2C_FIELD(temp_r0_15, u8, 4) <= (u32) *(*sp24 + temp_r4)) {
            var_r0_3 = (u8 *)temp_r0_15 + 4;
            var_r1_4 = temp_r4;
loop_98:
            var_r7_2 += 1;
            var_r1_4 += 1;
            if (var_r7_2 <= 3) {
                var_r0_3 += 1;
                if ((u32) *var_r0_3 <= (u32) *(*sp24 + var_r1_4)) {
                    goto loop_98;
                }
            }
        }
        temp_r3_4 = 4 ^ var_r7_2;
        temp_r0_16 = *sp24;
        M2C_FIELD(temp_r0_16, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r0_16, u16, 6)) | 0x38);
        M2C_FIELD(temp_r8_2, s32, 4) = 0x40000400;
        temp_r4_2 = 1 - ((u32) ((0 - temp_r3_4) | temp_r3_4) >> 0x1F);
        M2C_FIELD(temp_r8_2, s32, 8) = 0;
        M2C_FIELD(temp_r8_2, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r8_2, u16, 6)) | (((M2C_FIELD(temp_fp_2, u16, 0xC) * 8) + 8) & 0x1FF));
        M2C_FIELD(temp_r8_2, u8, 4) = (u8) ((M2C_FIELD(temp_fp_2, u16, 0xE) * 8) + 4);
        sp4 = temp_r4_2;
        M2C_FIELD(temp_r8_2, u16, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r8_2, u16, 8)) | (Func_08021b30(0x3FFF & M2C_FIELD(temp_r0_15, u16, 0), sp28) & 0x3FF));
        var_r4_2 = temp_r4_2;
        M2C_FIELD(*sp24, s8, 0x25) = 1;
        if (var_r4_2 == 0) {
            Func_0801e71c(2);
            var_r4_2 = sp4;
        }
        sp4 = var_r4_2;
        Func_0801e7c0(*Func_080771e0(temp_r6_5) + 0x333, temp_fp_2, 0x10, 0);
        var_r4_3 = sp4;
        var_r7_3 = 0;
        var_r6_5 = (u8 *)temp_r0_15 + 4;
        var_r5_3 = 0xD;
        do {
            if (*var_r6_5 != 0) {
                sp4 = var_r4_3;
                Func_08019000((struct Work_80190000 *) temp_fp_2, var_r7_3 + 0x5001, var_r5_3, 0, 0);
                Func_08018efc(temp_fp_2, *var_r6_5 + 0x30, var_r5_3 + 1, 0, 0);
                var_r5_3 += 2;
            }
            var_r7_3 += 1;
            var_r6_5 += 1;
        } while (var_r7_3 <= 3);
        if (var_r4_3 != 0) {
            Func_080f9010(0x70);
        } else {
            Func_080f9010(0x72);
        }
        var_r6_4 = Func_08026080(sp40, M2C_FIELD(temp_sl_2, u8, 0), M2C_FIELD(temp_sl_2, u8, 8), Func_0802706c(temp_sl_2));
        M2C_FIELD(sp4C, s16, 0xC) = (s16) M2C_FIELD(temp_sl_2, u8, 8);
        M2C_FIELD(*sp24, s8, 0x25) = 0;
        Func_08003f3c((u32) sp28);
        Func_08016418(temp_fp_2, 1);
        if (var_r6_4 == -1) {
            goto loop_93;
        }
        var_r2_5 = sp1C + 0x54;
        var_r1_5 = *sp24;
        var_r0_4 = (u8 *)temp_r0_15 + 4;
        var_r5_4 = var_r2_5;
        var_r4_4 = M2C_FIELD(temp_r0_15, u8, 4);
        var_r3_4 = *(var_r1_5 + var_r5_4);
        var_r7_4 = 0;
        if ((u32) var_r4_4 > (u32) var_r3_4) {
            *(var_r1_5 + var_r5_4) = 0;
        } else {
loop_116:
            var_r7_4 += 1;
            *(var_r1_5 + var_r5_4) = var_r3_4 - var_r4_4;
            var_r0_4 += 1;
            var_r2_5 += 1;
            if (var_r7_4 > 3) {

            } else {
                var_r1_5 = *sp24;
                var_r5_4 = var_r2_5;
                var_r4_4 = *var_r0_4;
                var_r3_4 = *(var_r1_5 + var_r5_4);
                if ((u32) var_r4_4 > (u32) var_r3_4) {
                    *(var_r1_5 + var_r5_4) = 0;
                } else {
                    goto loop_116;
                }
            }
        }
        goto block_91;
    case 15:
        Func_080f9010(0x70);
        temp_r2_6 = *(u8 **)0x03001F34;
        M2C_FIELD(temp_r2_6, s32, 0x34) = 0;
        M2C_FIELD(temp_r2_6, s32, 0x30) = 0;
        M2C_FIELD(temp_r2_6, s32, 0x38) = 0;
loop_121:
        temp_r1_7 = *sp24;
        M2C_FIELD(temp_r1_7, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_7, u16, 6)) | 0x90);
        temp_r5_7 = sp1C + 0xE4;
        M2C_FIELD(*(u8 **)0x03001F34, s32, temp_r5_7) = 0x80000000;
        temp_r6_6 = Func_08023e70(sp40, 1);
        M2C_FIELD(sp4C, s16, 0xC) = 0;
        if (temp_r6_6 == -1) {
            goto interaction_loop;
        }
        sp3C = 5;
        sp38 = temp_r6_6;
        M2C_FIELD(*(u8 **)0x03001F34, s32, temp_r5_7) = temp_r6_6;
        temp_r4_3 = 0xFF & sp38;
        temp_r7_2 = (sp38 >> 8) & 0xF;
        sp4 = temp_r4_3;
        temp_r0_17 = Func_08077208(sp40, temp_r7_2, temp_r4_3);
        if (temp_r0_17 != 0) {
            temp_r5_8 = Func_08077080(Func_080771e8(temp_r7_2, temp_r4_3));
            temp_r6_7 = M2C_FIELD(temp_r5_8, u8, 8);
            temp_r1_8 = *sp24;
            temp_fp_3 = Func_080162d4(0xBU, 0x11U, 0xAU, 3U, 6);
            M2C_FIELD(temp_r1_8, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_8, u16, 6)) | 0x40);
            Func_08019000((struct Work_80190000 *) temp_fp_3, temp_r7_2 + 0x5001, 0, 0, 0);
            Func_0801e7c0((temp_r7_2 * 0x14) + sp4 + 0x45F, temp_fp_3, 0x10, 0);
            M2C_FIELD(sp4C, s16, 0xC) = (s16) temp_r6_7;
            Func_080030f8(1U);
            Func_080f9010(0x70);
            var_r6_4 = Func_08026080(sp40, M2C_FIELD(temp_r5_8, u8, 0), temp_r6_7, Func_0802706c(temp_r5_8));
            if (M2C_FIELD(*sp24, s32, 0xD8) != 0) {
                temp_r5_9 = Func_08021e48(0xC4E, 0xF, 8);
loop_129:
                if (Func_08017364() == 0) {
                    Func_080030f8(1U);
                    goto loop_129;
                }
                Func_08016418(temp_r5_9, 1);
                temp_r1_9 = *sp24;
                M2C_FIELD(temp_r1_9, s32, 0xD8) = (s32) (M2C_FIELD(temp_r1_9, s32, 0xD8) + 1);
                M2C_FIELD(temp_r1_9, s32, 0xDC) = 0x2D;
            }
            Func_08016418(temp_fp_3, 1);
            if (var_r6_4 == -1) {
                goto loop_121;
            }
        } else {
            sp4 = temp_r4_3;
            temp_fp_4 = Func_080162d4(0xBU, 0x11U, 0xAU, 3U, 6);
            temp_r1_10 = *sp24;
            M2C_FIELD(temp_r1_10, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_10, u16, 6)) | 0x40);
            Func_0801e71c(2);
            Func_08019000((struct Work_80190000 *) temp_fp_4, temp_r7_2 + 0x5001, 0, 0, temp_r0_17);
            Func_0801e7c0((temp_r7_2 * 0x14) + temp_r4_3 + 0x45F, temp_fp_4, 0x10, 0);
            Func_0801e71c(0xF);
            M2C_FIELD(sp4C, s16, 0xC) = 1;
            Func_080030f8(1U);
            Func_080f9010(0x70);
            var_r6_4 = Func_08026080(sp40, 4U, 0U, 7);
            Func_08016418(temp_fp_4, 1);
            if (var_r6_4 == -1) {
                goto loop_121;
            }
        }
        goto block_91;
    case 2:
        Func_080f9010(0x70);
        temp_r3_5 = *(u8 **)0x03001F34;
        M2C_FIELD(temp_r3_5, s32, 0x34) = 0;
        M2C_FIELD(temp_r3_5, s32, 0x30) = 0;
        M2C_FIELD(temp_r3_5, s32, 0x38) = 0;
loop_139:
        temp_r1_11 = *sp24;
        M2C_FIELD(temp_r1_11, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r1_11, u16, 6)) | 0x60);
        temp_r3_6 = (s8 *)(temp_r1_11 + 0x74);
        temp_r1_12 = (u16 *)(temp_r1_11 + 0x96);
        var_r5_5 = M2C_FIELD(sp48, u16, 0xD8);
        var_r8 = 0;
        var_r4_5 = 0;
        if (var_r5_5 != 0) {
            var_r3_5 = (u16 *)(sp48 + 0xD8);
            var_r7_5 = temp_r3_6;
            var_r6_6 = temp_r1_12;
loop_141:
            sp8 = var_r3_5;
            sp4 = var_r4_5;
            Func_08077018((s32) var_r5_5);
            if (Func_08025180(sp40, (s32) var_r5_5) == 0) {
                *var_r6_6 = var_r5_5;
                var_r8 += 1;
                *var_r7_5 = (s8) var_r4_5;
                var_r6_6 += 1;
                var_r7_5 += 1;
            }
            var_r4_5 += 1;
            if (var_r4_5 != 0xF) {
                var_r3_5 += 1;
                var_r5_5 = *var_r3_5;
                if (var_r5_5 != 0) {
                    goto loop_141;
                }
            }
        }
        var_r5_6 = M2C_FIELD(sp48, u16, 0xD8);
        var_r4_6 = 0;
        if (var_r5_6 != 0) {
            var_r2_6 = (u16 *)(sp48 + 0xD8);
            var_r7_6 = var_r8 + temp_r3_6;
            var_r6_7 = temp_r1_12 + var_r8;
loop_147:
            spC = var_r2_6;
            sp4 = var_r4_6;
            Func_08077018((s32) var_r5_6);
            if (Func_08025180(sp40, (s32) var_r5_6) != 0) {
                *var_r6_7 = var_r5_6;
                var_r8 += 1;
                *var_r7_6 = (s8) var_r4_6;
                var_r6_7 += 1;
                var_r7_6 += 1;
            }
            var_r4_6 += 1;
            if (var_r4_6 != 0xF) {
                var_r2_6 += 1;
                var_r5_6 = *var_r2_6;
                if (var_r5_6 != 0) {
                    goto loop_147;
                }
            }
        }
        temp_r1_12[var_r8] = 0;
        temp_r0_18 = Func_08025200(sp40, temp_r1_12);
        if (temp_r0_18 == -1) {
            goto interaction_loop;
        }
        temp_r6_8 = *(temp_r3_6 + temp_r0_18);
        sp38 = (s32) temp_r6_8;
        temp_r6_9 = (temp_r6_8 * 2) + 0xD8;
        temp_r0_19 = Func_08077080((s32) M2C_FIELD(Func_08077018((s32) M2C_FIELD(sp48, u16, temp_r6_9)), u16, 0x28));
        temp_r8_3 = temp_r0_19;
        temp_sl_3 = M2C_FIELD(temp_r0_19, u8, 8);
        temp_r5_10 = *sp24;
        temp_r9 = Func_080040b4(0x80);
        temp_fp_5 = Func_080162d4(9U, 0x11U, 0xFU, 3U, 6);
        temp_r0_20 = *sp24;
        M2C_FIELD(temp_r0_20, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r0_20, u16, 6)) | 0x30);
        temp_r5_11 = temp_r5_10 + 0xC;
        M2C_FIELD(temp_r5_11, s32, 4) = 0x40000400;
        M2C_FIELD(temp_r5_11, s32, 8) = 0;
        M2C_FIELD(temp_r5_11, u8, 4) = (u8) ((M2C_FIELD(temp_fp_5, u16, 0xE) * 8) + 4);
        M2C_FIELD(temp_r5_11, u16, 6) = (u16) ((0xFFFFFE00 & M2C_FIELD(temp_r5_11, u16, 6)) | (((M2C_FIELD(temp_fp_5, u16, 0xC) * 8) + 8) & 0x1FF));
        sp4 = 0x1FF;
        M2C_FIELD(temp_r5_11, u16, 8) = (u16) ((0xFFFFFC00 & M2C_FIELD(temp_r5_11, u16, 8)) | (Func_08021af0((s32) M2C_FIELD(sp48, u16, temp_r6_9), temp_r9) & 0x3FF));
        M2C_FIELD(*sp24, s8, 0x25) = 1;
        Func_0801e7c0((M2C_FIELD(sp48, u16, temp_r6_9) & 0x1FF) + 0x182, temp_fp_5, 0x18, 0);
        M2C_FIELD(sp4C, s16, 0xC) = (s16) temp_sl_3;
        Func_080f9010(0x70);
        var_r6_4 = Func_08026080(sp40, M2C_FIELD(temp_r8_3, u8, 0), temp_sl_3, Func_0802706c(temp_r8_3));
        M2C_FIELD(*sp24, s8, 0x25) = 0;
        Func_08003f3c((u32) temp_r9);
        Func_08016418(temp_fp_5, 1);
        if (var_r6_4 == -1) {
            goto loop_139;
        }
        sp3C = 2;
        goto block_91;
    case 3:
        sp3C = 3;
        break;
    }
finish_entry:
    Func_080f9010(0x6E);
    M2C_FIELD(sp4C, u16, 0) = (u16) sp40;
    var_r6_8 = M2C_FIELD(sp48, u16, 0x40);
    if (var_r6_8 != 0) {
        var_r6_8 += (u32) (M2C_FIELD(sp48, u16, 0x40) * Func_08004458()) >> 0x14;
    }
    M2C_FIELD(sp4C, u16, 4) = var_r6_8;
    if (sp2C != 0) {
        temp_r3_7 = sp54 + sp2C;
        if (*temp_r3_7 == *(temp_r3_7 - 1)) {
            M2C_FIELD(sp4C, u16, 4) = (u16) ((s32) ((s16) var_r6_8 + ((u32) (var_r6_8 << 0x10) >> 0x1F)) >> 1);
        }
    }
    if ((s32) (s16) M2C_FIELD(sp4C, u16, 4) < 0) {
        M2C_FIELD(sp4C, u16, 4) = 0x7D0U;
    }
    M2C_FIELD(sp4C, u16, 6) = (u16) sp3C;
    M2C_FIELD(sp4C, u16, 8) = (u16) sp38;
    M2C_FIELD(sp4C, u16, 0xA) = (u16) sp44;
    temp_r2_7 = sp2C + 1;
    sp1C = temp_r2_7 * 4;
    sp2C = temp_r2_7;
    sp18 = temp_r2_7 * 0x10;
    sp20 = temp_r2_7 * 2;
    if (temp_r2_7 < sp50) {
        goto process_entry;
    }
check_outer_loop:
    if (sp2C < sp50) {
        goto outer_loop;
    }
cleanup:
    temp_r0_21 = *sp24;
    if (M2C_FIELD(temp_r0_21, s32, 0x50) != 0) {
        M2C_FIELD((void *)0x02002224, s16, 8) = 0x45;
        M2C_FIELD((void *)0x02002224, s16, 0xA) = 0x44;
        M2C_FIELD((void *)0x02002224, s16, 0xC) = 0x56;
        M2C_FIELD((void *)0x02002224, s16, 0xE) = 0x53;
    }
    temp_r0_22 = M2C_FIELD(temp_r0_21, u8 *, 0x44);
    if (temp_r0_22 != 0) {
        Func_08016418(temp_r0_22, 1);
    }
    Func_08003f3c((u32) sp30);
    Func_08003f3c((u32) sp34);
    Func_08004278((void *)0x08026E81);
    temp_r2_8 = *sp24;
    if (M2C_FIELD(temp_r2_8, s32, 0x50) != 0) {
        temp_r5_12 = *(u8 **)0x03001E74;
        var_r6_9 = 0;
        if (M2C_FIELD(temp_r2_8, u8 *, 0x44) == 0) {
            var_r7_7 = temp_r5_12 + 0x52;
            if (M2C_FIELD(temp_r5_12, u8, 0x52) == 0) {
                M2C_FIELD(*sp24, u8 *, 0x44) = Func_080162d4(0U, 0x10U, 0x1EU, 4U, 0x2A);
                Func_08016738();
                Func_080270d8();
            } else {
                goto block_181;
            }
        } else {
            var_r7_7 = temp_r5_12 + 0x52;
block_181:
            M2C_FIELD(*sp24, u8 *, 0x44) = 0;
        }
        var_r1_6 = (u8 *)(0x02002024 + ((1 ^ M2C_FIELD(temp_r5_12, u8, 0x50)) * 0x18));
        if (*var_r7_7 != 0) {
            sp50 = -1;
        } else {
loop_184:
            if ((3 & *(u16 *)0x03001F64) != 3) {
                var_r6_9 += 1;
                if (var_r6_9 > 0x18) {
                    sp50 = -1;
                } else {
                    goto block_196;
                }
            } else {
                temp_r2_9 = M2C_FIELD(var_r1_6, u16, 8);
                var_r6_9 = 0;
                if ((temp_r2_9 != 0x45) || (M2C_FIELD(var_r1_6, u16, 0xA) != 0x44) || (M2C_FIELD(var_r1_6, u16, 0xC) != 0x56) || (M2C_FIELD(var_r1_6, u16, 0xE) != 0x53)) {
                    if ((temp_r2_9 != 0x56) || (M2C_FIELD(var_r1_6, u16, 0xA) != 0x53) || (M2C_FIELD(var_r1_6, u16, 0xC) != 0x53) || (M2C_FIELD(var_r1_6, u16, 0xE) != 0x54)) {
                        var_r6_9 = 1;
                    }
block_196:
                    Func_080030f8(1U);
                    var_r1_6 = (u8 *)(0x02002024 + ((1 ^ M2C_FIELD(temp_r5_12, u8, 0x50)) * 0x18));
                    if (*var_r7_7 != 0) {
                        sp50 = -1;
                    } else {
                        goto loop_184;
                    }
                }
            }
        }
        temp_r0_23 = M2C_FIELD(*sp24, u8 *, 0x44);
        if (temp_r0_23 != 0) {
            Func_08016418(temp_r0_23, 1);
        }
    }
    Func_080b50d0(0);
    Func_08002dd8(0x39);
    return sp50;
}
