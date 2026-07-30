typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct Work_02000000 {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    u8 padding_0c[8];
    s32 field_14;
    s32 field_18;
    s32 field_1c;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
s32 Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, s32);
s32 Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(s32, s32);
void Func_080072f0(s32, s32, s32, s32);
void Func_080072f4(s32, void *, s32, s32, u32, s32);
void Func_080072f8(s32, void *, s32, s32, s32, s32);
void Func_080072fc(s32, void *, s32, s32, s32, s32);
void Func_08007300(s32, void *, s32, s32, s32, s32);
u32 Func_080b5070(s32);
void Func_080b5088(s16, s32);
void **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
s32 Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e3908(void *, s32, s32);
void Func_080e3944(void *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

/*
 * Run one selector-driven battle effect from setup through final cleanup.
 *
 * The setup chooses and loads the selector's graphics, builds motion records
 * for each participating object, and clears the shared particle pool.  The
 * frame loop projects those records, advances their velocities, emits the
 * selector-specific sprite pieces, and maintains two auxiliary burst pools.
 *
 * Owner span: 0x080dea70-0x080df8b8.  The long branches to the frame loop and
 * cleanup are continuations of this owner, not separately callable functions.
 */
s32 Func_080dea70(struct Work_02000000 *arg0, s32 arg1) {
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    u8 *sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    u8 *sp38;
    s32 sp3C;
    u32 sp40;
    struct Work_02000000 **sp44;
    u32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
    s32 sp60;
    s32 sp64;
    u8 *sp68;
    s32 sp6C;
    s32 sp78;
    s32 sp7C;
    s32 sp80;
    s32 sp84;
    s32 sp88;
    s32 sp8C;
    s32 projected[3];
    s32 *var_r2;
    u8 *var_r3_6;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_6;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r1_5;
    s32 temp_r1_6;
    s32 temp_r2_2;
    s32 temp_r2_4;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r4;
    s32 temp_r4_4;
    s32 temp_r4_5;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r6_3;
    s32 temp_r7_2;
    s32 temp_r7_3;
    s32 temp_sl_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r1_2;
    s32 var_r1_3;
    s32 var_r1_4;
    s32 var_r2_2;
    s32 var_r2_3;
    s32 var_r2_4;
    u8 *var_r2_5;
    s32 var_r2_6;
    s32 var_r2_7;
    s32 var_r2_8;
    s32 var_r3;
    s32 var_r3_10;
    s32 var_r3_11;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_5;
    s32 var_r3_7;
    s32 var_r3_9;
    s32 var_r6;
    u32 draw_width;
    s32 draw_height;
    volatile u32 random_sample;
    struct Work_02000000 *temp_r2;
    struct Work_02000000 *temp_r5;
    u32 temp_r0_5;
    u32 temp_r0_7;
    u32 temp_r2_3;
    u32 temp_r3_4;
    u32 temp_r3_5;
    u32 temp_r3_6;
    u32 var_r0_5;
    u32 var_r3_8;
    u8 temp_r0_2;
    u8 temp_r4_2;
    u8 temp_r4_3;
    u8 temp_sl;
    u8 *temp_r0;
    u8 *temp_r3_3;
    u8 *temp_r6;
    u8 *temp_r6_2;
    u8 *temp_r7;
    void *var_r0;
    void *var_r0_2;
    u8 *var_r1;
    u8 *var_r1_5;
    u8 *var_r1_6;
    u8 *var_r5;
    u8 *var_r5_2;
    u8 *var_r5_3;
    u8 *var_r5_4;
    u8 *var_r5_5;
    u8 *var_r7;

    temp_r0 = M2C_FIELD((void *)0x03001EEC, u8 **, 0);
    sp84 = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp6C = *(s32 *)0x03001E80;
    sp68 = M2C_FIELD((void *)0x03001EEC, u8 **, 8);
    sp64 = 0;
    M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828) = arg0;
    sp5C = arg0->field_18;
    if (arg1 == 0xA) {
        Func_080cd594(0);
    } else {
        Func_080cd594(1);
    }
    temp_r2 = M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828);
    if (temp_r2->field_1c == 1) {
        Func_080de2f8(arg0, 1, temp_r2->field_4, 2, &sp8C, &sp88);
    }
    if (arg1 == 5) {
        if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_4 == 1) {
            var_r3 = 2;
            goto block_11;
        }
        Func_080ed408(0x2E, 7, 7, 3, 2);
    } else if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_4 == 1) {
        var_r3 = 3;
block_11:
        Func_080ed408(0x2E, 7, 7, 7, var_r3);
    } else {
        Func_080ed408(0x2E, 7, 7, 3, 3);
    }
    Func_080e0524((void *)0x73, sp68, 0, 0);
    switch (arg1) {
    case 0:
    case 8:
        if (arg1 == 5) {
        case 5:
            sp5C = 2;
        }
        if (arg1 == 8) {
            sp5C = 0;
        }
        if (sp5C == 0) {
            var_r0 = (void *)0x7F;
            goto block_24;
        }
        if (sp5C == 1) {
            var_r0 = (void *)0x80;
block_24:
            Func_080e0524(var_r0, temp_r0 + 0x1000, 1, 1);
        } else {
            Func_080e0524((void *)0x81, temp_r0 + 0x1000, 1, 1);
        }
        if (arg1 == 5) {
            Func_080072f0(0x05000000, Func_08002f40(0xB9), 0x80, 0x03001388);
        }
        Func_080e0524((void *)0xC7, temp_r0 + 0x2000, 1, 0);
        if (arg1 == 5) {
            Func_080072f0(0x05000000, Func_08002f40(0xB9), 0x80, 0x03001388);
        }
        M2C_FIELD(temp_r0, s32 *, 0x7780) = 2;
        var_r3_2 = 0x4B;
block_42:
        var_r2 = (s32 *)(temp_r0 + 0x7784);
        break;
    case 1:
        Func_080e0524((void *)0x5D, temp_r0, 1, 1);
        *(s16 *)0x04000050 = 0;
        M2C_FIELD(temp_r0, s32 *, 0x7780) = arg1;
        var_r2 = (s32 *)(temp_r0 + 0x7784);
        var_r3_2 = 0;
        break;
    case 2:
        Func_080072f0(0x05000000, Func_08002f40(0x7F), 0x80, 0x03001388);
        Func_080e0524((void *)0x5C, temp_r0, 0, 0);
        M2C_FIELD(temp_r0, s32 *, 0x7780) = arg1;
        var_r2 = (s32 *)(temp_r0 + 0x7784);
        var_r3_2 = 0x32;
        break;
    default:
        if (((u32) (arg1 - 3) <= 1U) || (arg1 == 0xB)) {
            var_r3_3 = 1;
            var_r0_2 = (void *)0x5B;
            var_r1 = temp_r0;
        } else if (arg1 == 6) {
            var_r3_3 = 1;
            var_r0_2 = (void *)0x68;
            var_r1 = temp_r0;
        } else {
            Func_080e0524((void *)0xB8, temp_r0, 1, 1);
            var_r3_3 = 0;
            var_r0_2 = (void *)0x92;
            var_r1 = temp_r0 + 0x65C0;
        }
        Func_080e0524(var_r0_2, var_r1, 1, var_r3_3);
        M2C_FIELD(temp_r0, s32 *, 0x7780) = 2;
        var_r3_2 = 0x32;
        goto block_42;
    }
    *var_r2 = var_r3_2;
    Func_080041d8((const void *)0x080CD261, 0x480);
    temp_r6 = *Func_080b5098(M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_8);
    sp54 = arg1 * 4;
    temp_r3 = arg1 * 5;
    sp58 = M2C_FIELD((void *)0x080EEBEC, u8 *, temp_r3);
    temp_sl = M2C_FIELD((void *)0x080EEBEC, u8 *, temp_r3 + 1);
    sp50 = M2C_FIELD((void *)0x080EEBEC, u8 *, temp_r3 + 2);
    sp4C = M2C_FIELD((void *)0x080EEBEC, u8 *, temp_r3 + 3);
    sp48 = Func_080b5070(M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_8);
    temp_r5 = M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828);
    if ((s32) (temp_r5->field_14 * sp58) > 0x3F) {
        temp_r5->field_14 = 1;
    }
    sp80 = 0;
    if (temp_r5->field_14 == 0) {

    } else {
        sp44 = (struct Work_02000000 **)(temp_r0 + 0x7828);
        sp18 = 0;
        sp14 = 0x24;
loop_48:
        temp_r7 = *Func_080b5098(M2C_FIELD(*sp44, s16 *, sp14));
        sp40 = Func_080b5070(M2C_FIELD(*sp44, s16 *, sp14));
        sp7C = 0;
        if (sp58 == 0) {

        } else {
            temp_r3_2 = (s32) (sp48 + (sp48 >> 0x1F)) >> 1;
            var_r5 = (sp18 * 0x1C) + temp_r0 + 0x7080;
loop_51:
            M2C_FIELD(var_r5, s32 *, 0) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
            switch (arg1) {
            case 7:
                var_r3_4 = M2C_FIELD(temp_r6, s32 *, 0xC) + ((0xF & Func_08004458()) << 0x10) + 0x3A0000;
block_67:
                M2C_FIELD(var_r5, s32 *, 4) = var_r3_4;
                break;
            case 10:
                var_r3_4 = M2C_FIELD(temp_r6, s32 *, 0xC) + temp_r3_2;
                goto block_67;
            case 6:
                var_r3_4 = M2C_FIELD(temp_r6, s32 *, 0xC) + temp_r3_2;
                goto block_67;
            case 9:
                /* Preserve selector 9's distinct RNG sample site. */
                random_sample = Func_08004458();
                var_r2_2 = 0x1F & random_sample;
                var_r3_5 = 0x10;
block_63:
                M2C_FIELD(var_r5, s32 *, 4) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + temp_r3_2 + ((var_r3_5 - var_r2_2) << 0x10));
                break;
            default:
                if ((u32) (arg1 - 3) <= 1U) {
                    var_r2_2 = 0x1F & Func_08004458();
                    var_r3_5 = 0x10;
                    goto block_63;
                }
                if (arg1 == 0xB) {
                    var_r2_2 = 0x3F & Func_08004458();
                    var_r3_5 = 0x20;
                    goto block_63;
                }
                if (arg1 == 5) {
                    var_r3_4 = M2C_FIELD(temp_r6, s32 *, 0xC) + temp_r3_2;
                } else {
                    var_r3_4 = M2C_FIELD(temp_r6, s32 *, 0xC) + sp48;
                }
                goto block_67;
            }
            M2C_FIELD(var_r5, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
            M2C_FIELD(var_r5, s32 *, 0xC) = Func_080022ec(M2C_FIELD(temp_r7, s32 *, 8) - M2C_FIELD(var_r5, s32 *, 0), (s32) temp_sl);
            switch (arg1) {
            case 7:
                var_r0_3 = ((M2C_FIELD(temp_r7, s32 *, 0xC) + ((0x3F & Func_08004458()) << 0x10)) - M2C_FIELD(var_r5, s32 *, 4)) + 0xFFF40000;
block_84:
                M2C_FIELD(var_r5, s32 *, 0x10) = Func_080022ec(var_r0_3, (s32) temp_sl);
                break;
            case 8:
                var_r0_3 = ((M2C_FIELD(temp_r7, s32 *, 0xC) + ((7 & Func_08004458()) << 0x10)) - M2C_FIELD(var_r5, s32 *, 4)) + 0x160000;
                goto block_84;
            case 9:
                var_r0_4 = M2C_FIELD(temp_r7, s32 *, 0xC) + ((0x40 - (0x3F & Func_08004458())) << 0x10);
block_83:
                var_r0_3 = var_r0_4 - M2C_FIELD(var_r5, s32 *, 4);
                goto block_84;
            case 10:
                var_r0_3 = ((M2C_FIELD(temp_r7, s32 *, 0xC) + ((0x1F & Func_08004458()) << 0x10)) - M2C_FIELD(var_r5, s32 *, 4)) + 0x40000;
                goto block_84;
            default:
                if (((u32) (arg1 - 3) <= 1U) || (arg1 == 0xB) || (arg1 == 5)) {
                    M2C_FIELD(var_r5, s32 *, 0x10) = 0;
                } else {
                    if (arg1 == 6) {
                        var_r0_4 = M2C_FIELD(temp_r7, s32 *, 0xC) + ((s32) (sp40 + (sp40 >> 0x1F)) >> 1) + ((0xF & Func_08004458()) << 0x10);
                    } else {
                        var_r0_4 = (M2C_FIELD(temp_r7, s32 *, 0xC) + sp40) - ((0xF & Func_08004458()) << 0x10);
                    }
                    goto block_83;
                }
                break;
            }
            M2C_FIELD(var_r5, s32 *, 0x14) = Func_080022ec(M2C_FIELD(temp_r7, s32 *, 0x10) - M2C_FIELD(var_r5, s32 *, 8), (s32) temp_sl);
            M2C_FIELD(var_r5, s32 *, 0x18) = 0;
            temp_r2_2 = sp7C + 1;
            var_r5 += 0x1C;
            sp7C = temp_r2_2;
            if (temp_r2_2 != sp58) {
                goto loop_51;
            }
        }
        temp_r1 = sp80 + 1;
        sp18 += sp58;
        sp14 += 2;
        sp80 = temp_r1;
        if (temp_r1 != M2C_FIELD(*sp44, s32 *, 0x14)) {
            goto loop_48;
        }
    }
    sp80 = 0;
    var_r3_6 = (u8 *)0x02010018;
    do {
        M2C_FIELD(var_r3_6, s32 *, 0) = 0;
        temp_r4 = sp80 + 1;
        var_r3_6 += 0x1C;
        sp80 = temp_r4;
    } while (temp_r4 != 0x400);
    if (arg1 == 6) {
        var_r3_7 = (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14 * sp4C) + (sp50 * sp58) + 0x20;
    } else {
        var_r3_7 = (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14 * sp4C) + (sp50 * sp58) + 0x10;
    }
    sp60 = var_r3_7;
    sp78 = 0;
    if (sp60 == 0) {
        goto cleanup;
    }
    sp30 = sp6C + 0xC;
frame_loop:
    if (sp64 > 0) {
        sp64 -= 1;
    }
    switch (arg1) {
    case 5:
        break;
    case 6:
        if (sp78 == 4) {
            Func_080f9010(0x88);
        }
        if (sp78 == 0x20) {
            Func_080b50e8(0x86);
        }
        break;
    case 7:
        if (sp78 == 0x30) {
            Func_080b50e8(0x85);
        }
        break;
    default:
        if (sp78 == 0x10) {
            Func_080b50e8(0x85);
        }
        break;
    }
    if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_1c == 1) {
        temp_r5_2 = sp78 << 0xB;
        temp_sl_2 = (((s32) ((0 - Func_08002322(temp_r5_2)) * 4) >> 0x10) + ((s32) (sp8C + ((u32) sp8C >> 0x1F)) >> 1)) - 0xA;
        var_r6 = (((s32) (Func_0800231c(temp_r5_2) * 2) >> 0x10) + sp88) - 0x18;
        if (sp78 > 0x45) {
            var_r6 = (var_r6 - (sp78 * 2)) + 0x8A;
        }
        if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_4 == 1) {
            Func_080ed408(0x2F, 7, 7, 7, 3);
        } else {
            Func_080ed408(0x2F, 7, 7, 3, 3);
        }
        if (sp78 <= 3) {
            Func_080072f8(sp84, temp_r0 + 0x65C0, temp_sl_2, var_r6, 0x14, 0x28);
        }
        Func_080072f8(sp84, temp_r0 + 0x65C0, temp_sl_2, var_r6, 0x14, 0x28);
        Func_08002dd8(0x2F);
    }
    Func_080049ac();
    Func_080051d8(sp6C, sp30);
    sp3C = 0;
    sp80 = 0;
    if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14 == 0) {

    } else {
        sp20 = 0x24;
        sp1C = 0;
loop_121:
        sp38 = *Func_080b5098(
            M2C_FIELD(
                M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828),
                s16 *,
                sp20));
        sp7C = 0;
        if (sp58 == 0) {

        } else {
            sp2C = sp1C;
            sp28 = (u8 *)0x02010000 + (sp3C * 0x1C);
            sp24 = 0;
loop_124:
            if ((s32) (sp24 + sp2C) >= sp78) {

            } else {
                temp_r3_3 = (((sp58 * sp80) + sp7C) * 0x1C) + temp_r0;
                temp_r6_2 = temp_r3_3 + 0x7080;
                Func_080e3944(temp_r6_2, projected);
                projected[0] >>= 1;
                M2C_FIELD(temp_r3_3, u32 *, 0x7080) = (u32) (M2C_FIELD(temp_r3_3, u32 *, 0x7080) + M2C_FIELD(temp_r6_2, s32 *, 0xC));
                M2C_FIELD(temp_r6_2, s32 *, 4) = (s32) (M2C_FIELD(temp_r6_2, s32 *, 4) + M2C_FIELD(temp_r6_2, s32 *, 0x10));
                M2C_FIELD(temp_r6_2, s32 *, 8) = (s32) (M2C_FIELD(temp_r6_2, s32 *, 8) + M2C_FIELD(temp_r6_2, s32 *, 0x14));
                if (arg1 == 6) {
                    var_r5_2 = (void *)0x02013800;
                    var_r2_3 = 0;
                    var_r1_2 = 0x200;
loop_128:
                    if (M2C_FIELD(var_r5_2, s32 *, 0x18) == 0) {
                        M2C_FIELD(var_r5_2, s32 *, 0) = projected[0] << 0x10;
                        M2C_FIELD(var_r5_2, s32 *, 4) = projected[1] << 0x10;
                        M2C_FIELD(var_r5_2, s32 *, 0xC) =
                            ((Func_08004458() & 0xFF) - 0x80) << 0xB;
                        M2C_FIELD(var_r5_2, s32 *, 0x10) =
                            ((Func_08004458() & 0xFF) - 0x80) << 0xB;
                        var_r2_3 += 1;
                        M2C_FIELD(var_r5_2, s32 *, 0x18) =
                            (7 & Func_08004458()) + 0x20;
                        if (var_r2_3 == 2) {
                            goto particle_pool_ready;
                        }
                    }
                    var_r1_2 += 1;
                    var_r5_2 += 0x1C;
                    if (var_r1_2 != 0x400) {
                        goto loop_128;
                    }
particle_pool_ready:
                    ;
                }
                if (M2C_FIELD(temp_r6_2, s32 *, 0x18) != 0) {

                } else {
                    temp_r1_2 = M2C_FIELD(sp38, s32 *, 8);
                    temp_r2_3 = M2C_FIELD(temp_r3_3, u32 *, 0x7080);
                    if (temp_r1_2 < 0) {
                        if ((temp_r2_3 >> 0x1F) == 0) {

                        } else {
                            goto block_138;
                        }
                    } else if (((u32) ~temp_r2_3 >> 0x1F) == 0) {

                    } else {
block_138:
                        var_r3_8 = temp_r2_3;
                        if ((s32) var_r3_8 < 0) {
                            var_r3_8 = 0 - var_r3_8;
                        }
                        var_r2_4 = temp_r1_2;
                        if (var_r2_4 < 0) {
                            var_r2_4 = 0 - var_r2_4;
                        }
                        if ((s32) var_r3_8 < var_r2_4) {

                        } else {
                            sp34 = 0;
                            var_r2_5 = sp28;
                            M2C_FIELD(temp_r6_2, s32 *, 0x18) = 1;
                            if (arg1 == 5) {
                                Func_080b50e8(0x86);
                                goto block_149;
                            }
                            if ((arg1 != 6) && (sp64 == 0)) {
                                sp64 = 8;
                                Func_080f9010(0x84);
block_149:
                                ;
                            }
                            if (arg1 == 2) {
                                temp_r5_3 = ((0x1F & Func_08004458()) + 0x20) << 0xC;
                                if (Func_08004458() & 1) {
                                    var_r3_9 = M2C_FIELD(temp_r6_2, s32 *, 0x10) + temp_r5_3;
                                } else {
                                    var_r3_9 = M2C_FIELD(temp_r6_2, s32 *, 0x10) - temp_r5_3;
                                }
                                M2C_FIELD(temp_r6_2, s32 *, 0x10) = var_r3_9;
                                M2C_FIELD(temp_r6_2, s32 *, 0xC) = (s32) (0 - M2C_FIELD(temp_r6_2, s32 *, 0xC));
                            }
                            M2C_FIELD(var_r2_5, s32 *, 0x18) = 1;
                            M2C_FIELD(var_r2_5, s32 *, 0) = projected[0];
                            M2C_FIELD(var_r2_5, s32 *, 4) = projected[1];
                            M2C_FIELD(var_r2_5, s32 *, 8) = 0;
                            if (arg1 != 7) {
                                M2C_FIELD(temp_r0, s32 *, 0x77A8) = 2;
                            }
                            Func_080d6888(
                                M2C_FIELD(
                                    M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828),
                                    s16 *,
                                    sp20),
                                7,
                                5,
                                sp80,
                                8);
                            switch (arg1) {
                            case 7:
                            case 9:
                            case 10:
                                break;
                            case 5:
                                Func_080b5088(
                                    M2C_FIELD(
                                        M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828),
                                        s16 *,
                                        sp20),
                                    4);
                                break;
                            default:
                                Func_080b5088(
                                    M2C_FIELD(
                                        M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828),
                                        s16 *,
                                        sp20),
                                    5);
                                break;
                            }
                            temp_r2_4 = 5 ^ arg1;
                            var_r2_6 = 0xC - ((u32) ((0 - temp_r2_4) | temp_r2_4) >> 0x1F);
                            var_r5_3 = (void *)0x02010AF0;
                            var_r1_3 = 0x64;
loop_164:
                            if (M2C_FIELD(var_r5_3, s32 *, 0x18) == 0) {
                                M2C_FIELD(var_r5_3, s32 *, 0) =
                                    projected[0] << 0x10;
                                M2C_FIELD(var_r5_3, s32 *, 4) =
                                    projected[1] << 0x10;
                                M2C_FIELD(var_r5_3, s32 *, 0xC) =
                                    ((u8)Func_08004458() - 0x80) << var_r2_6;
                                M2C_FIELD(var_r5_3, s32 *, 0x10) =
                                    ((u8)Func_08004458() - 0x80) << var_r2_6;
                                M2C_FIELD(var_r5_3, s32 *, 0x18) =
                                    (7 & Func_08004458()) + 0x10;
                                sp34 += 1;
                                if (sp34 == M2C_FIELD(
                                        (void *)0x080EEBEC,
                                        u8 *,
                                        sp54 + arg1 + 4)) {
                                    goto burst_pool_ready;
                                }
                            }
                            var_r1_3 += 1;
                            var_r5_3 += 0x1C;
                            if (var_r1_3 != 0x200) {
                                goto loop_164;
                            }
burst_pool_ready:
                            ;
                        }
                    }
                }
                switch (arg1) {
                case 0:
                case 5:
                case 8:
                    if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_4 == 1) {
                        Func_080ed408(0x2F, 7, 7, 7, 2);
                    } else {
                        Func_080ed408(0x2F, 7, 7, 3, 2);
                    }
                    temp_r0_2 = M2C_FIELD((void *)0x080EEBE9, u8 *, sp5C);
                    Func_080072f4(sp84, temp_r0 + 0x1000, projected[0] - 0x10, projected[1] - temp_r0_2, 0x20U, (s32) temp_r0_2);
                    Func_08002dd8(0x2F);
                    if (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_4 == 1) {
                        Func_080ed408(0x2F, 7, 7, 0xF, 2);
                    } else {
                        Func_080ed408(0x2F, 7, 7, 0xB, 2);
                    }
                    Func_080072f4(sp84, temp_r0 + 0x1000, projected[0] - 0x10, projected[1], 0x20U, M2C_FIELD((void *)0x080EEBE9, u8 *, sp5C));
                    Func_08002dd8(0x2F);
                    break;
                case 1:
                    var_r0_5 = M2C_FIELD(temp_r3_3, u32 *, 0x7080);
                    if ((s32) var_r0_5 < 0) {
                        var_r0_5 = 0 - var_r0_5;
                    }
                    var_r1_4 = M2C_FIELD(sp38, s32 *, 8);
                    if (var_r1_4 < 0) {
                        var_r1_4 = 0 - var_r1_4;
                    }
                    if ((s32) var_r0_5 <= var_r1_4) {
                        var_r1_5 = (Func_080022fc(sp78, 6) * 0x300) + temp_r0;
                        var_r2_7 = projected[0] - 0x10;
                        var_r3_10 = projected[1] - 0xC;
                        draw_width = 0x20U;
                        draw_height = 0x18;
block_197:
                        Func_080072f4(
                            sp84,
                            var_r1_5,
                            var_r2_7,
                            var_r3_10,
                            draw_width,
                            draw_height);
                    }
                    break;
                case 7:
                case 9:
                case 10:
                    var_r1_5 =
                        temp_r0 +
                        M2C_FIELD((void *)0x080EEC52, u16 *, (3 & sp7C) * 2);
                    var_r2_7 = projected[0] - 4;
                    var_r3_10 = projected[1] - 4;
                    draw_width = 8U;
                    draw_height = 8;
                    goto block_197;
                case 2:
                    var_r1_6 = (Func_080022fc(sp7C, 6) << 7) + temp_r0;
                    var_r2_8 = projected[0] - 4;
                    var_r3_11 = projected[1] - 8;
                    draw_width = 8U;
                    draw_height = 0x10;
block_194:
                    Func_08007300(
                        sp84,
                        var_r1_6,
                        var_r2_8,
                        var_r3_11,
                        draw_width,
                        draw_height);
                    break;
                case 3:
                    var_r1_5 = temp_r0 + 0x60;
                    var_r2_7 = projected[0] - 9;
                    var_r3_10 = projected[1] - 7;
                    draw_width = 0x12U;
                    draw_height = 0xD;
                    goto block_197;
                case 4:
                    var_r1_6 = temp_r0;
                    var_r2_8 = projected[0] - 6;
                    var_r3_11 = projected[1] - 4;
                    draw_width = 0xCU;
                    draw_height = 8;
                    goto block_194;
                case 11:
                    var_r1_5 = temp_r0 + 0x14A;
                    var_r2_7 = projected[0] - 0xF;
                    var_r3_10 = projected[1] - 0xC;
                    draw_width = 0x1DU;
                    draw_height = 0x17;
                    goto block_197;
                default:
                    Func_080072fc(sp84, temp_r0, projected[0] - 0x14, projected[1] - 0x20, 0x28, 0x40);
                    break;
                }
            }
            temp_r1_3 = sp7C + 1;
            sp24 += sp50;
            sp7C = temp_r1_3;
            sp28 += 0x1C;
            sp3C += 1;
            if (temp_r1_3 != sp58) {
                goto loop_124;
            }
        }
        temp_r1_4 = sp80 + 1;
        sp20 += 2;
        sp1C += sp4C;
        sp80 = temp_r1_4;
        if (temp_r1_4 != M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14) {
            goto loop_121;
        }
    }
    if ((arg1 != 0) && (arg1 != 5) && (arg1 != 8)) {

    } else {
        sp80 = 0;
        if ((M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14 * sp58) == 0) {

        } else {
            var_r7 = (void *)0x02010000;
loop_210:
            temp_r1_5 = M2C_FIELD(var_r7, s32 *, 0x18);
            if (temp_r1_5 != 1) {

            } else {
                temp_r3_4 = M2C_FIELD(var_r7, u32 *, 8);
                temp_r6_3 = (s32) (temp_r3_4 + (temp_r3_4 >> 0x1F)) >> 1;
                if (!(sp80 & temp_r1_5)) {
                    Func_080ed408(0x2F, 7, 7, 3, 2);
                    temp_r5_4 = temp_r6_3 * 2;
                    temp_r4_2 = M2C_FIELD((void *)0x080EEC28, u8 *, temp_r6_3);
                    Func_080072f4(sp84, temp_r0 + M2C_FIELD((void *)0x080EEC44, u16 *, temp_r5_4), M2C_FIELD(var_r7, s32 *, 0) - temp_r4_2, M2C_FIELD(var_r7, s32 *, 4) - M2C_FIELD((void *)0x080EEC3D, u8 *, temp_r6_3), (u32) temp_r4_2, M2C_FIELD((void *)0x080EEC2F, u8 *, temp_r6_3));
                    Func_08002dd8(0x2F);
                    Func_080ed408(0x2F, 7, 7, 0xF, 2);
                    Func_080072f4(sp84, temp_r0 + M2C_FIELD((void *)0x080EEC44, u16 *, temp_r5_4), M2C_FIELD(var_r7, s32 *, 0), (M2C_FIELD(var_r7, s32 *, 4) + M2C_FIELD((void *)0x080EEC36, u8 *, temp_r6_3)) - M2C_FIELD((void *)0x080EEC3D, u8 *, temp_r6_3), M2C_FIELD((void *)0x080EEC28, u8 *, temp_r6_3), M2C_FIELD((void *)0x080EEC2F, u8 *, temp_r6_3));
                    Func_08002dd8(0x2F);
                } else {
                    Func_080ed408(0x2F, 7, 7, 3, 2);
                    temp_r5_5 = temp_r6_3 * 2;
                    temp_r4_3 = M2C_FIELD((void *)0x080EEC28, u8 *, temp_r6_3);
                    Func_080072f4(sp84, temp_r0 + 0x128A + M2C_FIELD((void *)0x080EEC44, u16 *, temp_r5_5), M2C_FIELD(var_r7, s32 *, 0) - M2C_FIELD((void *)0x080EEC3D, u8 *, temp_r6_3), M2C_FIELD(var_r7, s32 *, 4) - temp_r4_3, M2C_FIELD((void *)0x080EEC2F, u8 *, temp_r6_3), (s32) temp_r4_3);
                    Func_08002dd8(0x2F);
                    Func_080ed408(0x2F, 7, 7, 0xF, 2);
                    Func_080072f4(sp84, temp_r0 + 0x128A + M2C_FIELD((void *)0x080EEC44, u16 *, temp_r5_5), (M2C_FIELD(var_r7, s32 *, 0) + M2C_FIELD((void *)0x080EEC36, u8 *, temp_r6_3)) - M2C_FIELD((void *)0x080EEC3D, u8 *, temp_r6_3), M2C_FIELD(var_r7, s32 *, 4), M2C_FIELD((void *)0x080EEC2F, u8 *, temp_r6_3), M2C_FIELD((void *)0x080EEC28, u8 *, temp_r6_3));
                    Func_08002dd8(0x2F);
                }
                temp_r3_5 = M2C_FIELD(var_r7, u32 *, 8) + 1;
                M2C_FIELD(var_r7, u32 *, 8) = temp_r3_5;
                if (temp_r3_5 == 0xC) {
                    M2C_FIELD(var_r7, s32 *, 0x18) = 0;
                }
            }
            temp_r0_3 = sp80 + 1;
            sp80 = temp_r0_3;
            var_r7 += 0x1C;
            if (temp_r0_3 != (M2C_FIELD(temp_r0, struct Work_02000000 **, 0x7828)->field_14 * sp58)) {
                goto loop_210;
            }
        }
    }
    sp80 = 0x64;
    var_r5_4 = (void *)0x02010AF0;
    do {
        temp_r0_4 = M2C_FIELD(var_r5_4, s32 *, 0x18);
        if (temp_r0_4 > 0) {
            temp_r0_5 = (temp_r0_4 >> 3) + 1;
            temp_r4_4 = temp_r0_5 * 2;
            Func_080072f4(sp84, sp68 + M2C_FIELD((void *)0x080EDE48, u16 *, temp_r4_4 - 2), M2C_FIELD(var_r5_4, s16 *, 2) - ((s32) (temp_r0_5 + (temp_r0_5 >> 0x1F)) >> 1), M2C_FIELD(var_r5_4, s16 *, 6) - temp_r0_5, temp_r0_5, temp_r4_4);
            Func_080e3908(var_r5_4, 0x3C, 0x1000);
            if ((s32) M2C_FIELD(var_r5_4, s32 *, 4) > 0x700000) {
                temp_r3_6 = 0 - M2C_FIELD(var_r5_4, s32 *, 0x10);
                M2C_FIELD(var_r5_4, s32 *, 0x10) = (s32) ((s32) (temp_r3_6 + (temp_r3_6 >> 0x1F)) >> 1);
            }
            M2C_FIELD(var_r5_4, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_4, s32 *, 0x18) - 1);
        }
        temp_r7_2 = sp80 + 1;
        var_r5_4 += 0x1C;
        sp80 = temp_r7_2;
    } while (temp_r7_2 != 0x200);
    var_r5_5 = (void *)0x02013800;
    do {
        temp_r0_6 = M2C_FIELD(var_r5_5, s32 *, 0x18);
        if (temp_r0_6 > 0) {
            temp_r0_7 = (temp_r0_6 >> 4) + 1;
            temp_r4_5 = temp_r0_7 * 2;
            Func_080072f4(sp84, sp68 + M2C_FIELD((void *)0x080EDE48, u16 *, temp_r4_5 - 2), M2C_FIELD(var_r5_5, s16 *, 2) - ((s32) (temp_r0_7 + (temp_r0_7 >> 0x1F)) >> 1), M2C_FIELD(var_r5_5, s16 *, 6) - temp_r0_7, temp_r0_7, temp_r4_5);
            Func_080e3908(var_r5_5, 0x3C, -0x4000);
            M2C_FIELD(var_r5_5, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0x18) - 1);
        }
        temp_r7_3 = sp80 + 1;
        var_r5_5 += 0x1C;
        sp80 = temp_r7_3;
    } while (temp_r7_3 != 0x400);
    Func_080e155c(4, 4U);
    Func_080cd52c();
    M2C_FIELD(temp_r0, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r1_6 = sp78 + 1;
    sp78 = temp_r1_6;
    if (temp_r1_6 != sp60) {
        goto frame_loop;
    }
cleanup:
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}
