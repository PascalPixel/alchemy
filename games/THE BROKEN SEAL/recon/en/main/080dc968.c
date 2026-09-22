#include "shared-aggregates.h"

typedef void (*WordCopyFn)(void *dst, const void *src, s32 size);

/* Only the m2c spellings this draft actually uses. */
typedef s32 M2C_UNK;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

#define DRAW_RECTANGLE(destination, source, x, y) \
    draw_args[0]( \
        (void *)(destination), (source), (x), (y), sp0, sp4)

void Func_080dc968(void *arg0) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 *spC;
    s32 sp14;
    DrawRectangleFn draw_args[2];
    s32 local_record[3];
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    struct M2cAggregate_absolute_02010000 *sp24;
    s32 sp28;
    s32 sp2C;
    void *sp30;
    s32 sp34;
    struct M2cAggregate_deref_absolute_03001f00_0 *sp38;
    s32 actor_position[3];
    struct { s32 x; s32 y; s32 z; s32 color; } sp50;
    struct { s32 x; s32 y; s32 z; } sp60;
    s16 temp_r8_454;
    s16 temp_sl_455;
    u8 *var_r5_252;
    u8 *var_r5_723;
    u8 *var_r5_811;
    u8 *var_r6_389;
    s32 temp_r0_1078;
    s32 temp_r0_1119;
    s32 temp_r0_395;
    s32 temp_r0_786;
    s32 temp_r0_813;
    s32 temp_r0_816;
    s32 temp_r0_841;
    s32 temp_r1_282;
    s32 temp_r1_469;
    s32 temp_r2_410;
    s32 temp_r2_472;
    s32 temp_r2_915;
    s32 temp_r3_1101;
    s32 temp_r3_773;
    s32 temp_r5_1087;
    s32 temp_r6_436;
    s32 temp_r6_949;
    s32 temp_r7_439;
    s32 var_r2_516;
    s32 var_r3_517;
    s32 var_r4_387;
    s32 var_r5_1013;
    s32 var_r5_383;
    s32 var_r6_247;
    s32 var_r7_376;
    s32 var_r7_717;
    s32 var_r9_242;
    s32 var_r9_371;
    s32 var_r9_710;
    u8 *temp_r3_25;
    u8 *var_r1_515;
    u8 *var_r5_836;
    u8 *var_r7_185;
    u8 *var_r7_927;
    u32 temp_r0_885;
    u32 temp_r1_1012;
    u32 temp_r1_887;
    u32 temp_r3_1064;
    u32 temp_r3_529;
    u32 temp_r3_891;
    u32 temp_r3_938;
    u32 var_r6_943;
    u8 temp_r5_956;
    void *temp_r2_1081;
    void *temp_r2_844;
    void *temp_r3_447;
    void *temp_r5_449;
    u8 *battle_globals;

    battle_globals = (u8 *)0x03001F00;
    sp38 = *(struct M2cAggregate_deref_absolute_03001f00_0 **)battle_globals;
    sp34 = *(s32 *)(battle_globals - 0x10);
    temp_r3_25 = *(u8 **)(battle_globals - 0x14);
    sp24 = *(struct M2cAggregate_absolute_02010000 **)(battle_globals - 0x0C);
    sp20 = (s32) absolute_03001ad0.field_0004;
    M2C_FIELD(temp_r3_25, void **, 0x7828) = arg0;
    Func_080cd594(0x2000);
    absolute_04000020.field_0000 = 0x100;
    Func_080c9048();
    *(s16 *)0x05000000 = 0;
    *(s16 *)0x05000002 = 0;
    M2C_FIELD(temp_r3_25, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(0, 0);
    Func_080d6750(M2C_FIELD(temp_r3_25, void **, 0x7828));
    Func_080dbb24(9, 0x172, 1);
    Func_080e0524(0x6A, temp_r3_25, 1, 1);
    ((WordCopyFn)0x03001388)((void *)0x05000000, Func_08002f40(0xA0), 0x80);
    Func_080e0524(0x73, sp24, 0, 0);
    sp30 = Func_08002f40(0xD2);
    sp0 = 2;
    Func_080ed408(0x2E, 7, 7, 3, 2);
    sp0 = 3;
    Func_080ed408(0x2F, 7, 7, 3, 3);
    draw_args[0] = (DrawRectangleFn)*(void **)(battle_globals + 8);
    draw_args[1] = (DrawRectangleFn)*(void **)(battle_globals + 0x0C);
    absolute_03001ce0.field_0010 = 0xF0;
    Func_080030f8(1);
    Func_080b5040(1, 0x3B, 0);
    M2C_FIELD(temp_r3_25, s32 *, 0x7790) = 0;
    M2C_FIELD(temp_r3_25, s32 *, 0x7794) = 4;
    M2C_FIELD(temp_r3_25, s32 *, 0x7798) = -1;
    M2C_FIELD(temp_r3_25, s32 *, 0x779C) = 0;
    Func_080041d8(0x080C90E5, 0x480);
    sp38->field_0010 = 1;
    Func_080cd104(0, 1);
    M2C_FIELD((void *)0x04000000, s16 *, 0) = 0x7741;
    absolute_04000020.field_0000 = 0x80;
    M2C_FIELD((void *)0x04000000, s16 *, 0x52) = 0x1010;
    *(s16 *)0x04000050 = 0x3F44;
    sp1C = 0;
    sp18 = 0;
    sp2C = 0;
    var_r7_185 = temp_r3_25 + 0x7080;
    do {
        M2C_FIELD(var_r7_185, s32 *, 0) = (s32) ((Func_08002304(Func_08004458(), 0x60) + 0xC) << 0x10);
        M2C_FIELD(var_r7_185, s32 *, 4) = ((0x3F & Func_08004458()) + 0x20) << 0x10;
        M2C_FIELD(var_r7_185, s32 *, 0xC) = 0;
        M2C_FIELD(var_r7_185, s32 *, 0x10) = 0;
        M2C_FIELD(var_r7_185, s32 *, 0x18) = 0;
        var_r9_242 = 0;
        var_r6_247 = (sp2C * 0x480) + 0x02013800;
        var_r5_252 = (u8 *)&absolute_02010000 + (sp2C * 0x2A0);
loop_4:
        *(s32 *)var_r5_252 = (0xF & Func_08004458()) + 0x30;
        Func_080049ac();
        Func_08004c6c(Func_08004458());
        Func_08004bd4(Func_08004458());
        Func_08004c1c(Func_08004458());
        Func_08004a28(var_r6_247);
        var_r9_242 += 1;
        var_r5_252 += 0x1C;
        var_r6_247 += 0x30;
        if (var_r9_242 != 0x18) {
            goto loop_4;
        }
        temp_r1_282 = sp2C + 1;
        var_r7_185 += 0x1C;
        sp2C = temp_r1_282;
    } while (temp_r1_282 != 0x10);
    M2C_FIELD(temp_r3_25, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r3_25, s32 *, 0x7784) = 0x32;
    absolute_0400000c.field_0000 = 0x784;
    sp28 = 0;
    if (*(s32 *)0x03001B04 & 3) {

    } else {
        spC = local_record;
loop_9:
        if (sp28 <= 0xD1) {
            if (sp28 == 0) {
                sp1C = (M2C_FIELD(sp30, s8 *, 0) << 8) + M2C_FIELD(sp30, u8 *, 1);
                sp18 = (M2C_FIELD(sp30, s8 *, 2) << 8) + M2C_FIELD(sp30, u8 *, 3);
                sp30 += 4;
            } else {
                sp1C += M2C_FIELD(sp30, s8 *, 0);
                sp18 += (s8) M2C_FIELD(sp30, u8 *, 1);
                sp30 += 2;
            }
            M2C_FIELD(&sp50, s32 *, 0xC) = 0;
            M2C_FIELD(&sp50, s32 *, 4) = 0xFF0000;
            sp14 = 0;
            var_r9_371 = 0;
            var_r7_376 = 0x400000 - (sp18 << 0x10);
            do {
                var_r5_383 = (sp1C << 0x10) + 0x500000;
                var_r4_387 = 0;
                var_r6_389 = (var_r9_371 * 4) + temp_r3_25 + 0x77D8;
loop_15:
                M2C_FIELD(&sp50, s32 *, 0) = var_r5_383;
                M2C_FIELD(&sp50, s32 *, 8) = var_r7_376;
                temp_r0_395 = *(s32 *)var_r6_389;
                var_r6_389 += 4;
                sp8 = var_r4_387;
                Func_08009008(temp_r0_395, &sp50, 0x080EEB40, 0);
                var_r4_387 = sp8 + 1;
                var_r5_383 += 0x200000;
                if (var_r4_387 != 3) {
                    goto loop_15;
                }
                temp_r2_410 = sp14 + 1;
                var_r9_371 += 3;
                var_r7_376 += 0x200000;
                sp14 = temp_r2_410;
            } while (temp_r2_410 != 3);
        }
        M2C_FIELD(spC, s32 *, 4) = 0;
        M2C_FIELD(spC, s32 *, 8) = 0;
        if (sp28 == 0x30) {
            M2C_FIELD(temp_r3_25, s32 *, 0x77B4) = 0x18;
            M2C_FIELD(temp_r3_25, s32 *, 0x77B8) = 0;
        }
        sp2C = 0;
loop_20:
        temp_r6_436 = sp2C * 8;
        temp_r7_439 = temp_r6_436 + 0x40;
        if (sp28 < temp_r7_439) {

        } else {
            temp_r3_447 = (sp2C * 0x1C) + temp_r3_25;
            temp_r5_449 = temp_r3_447 + 0x7080;
            temp_r8_454 = M2C_FIELD(temp_r5_449, s16 *, 2);
            temp_sl_455 = M2C_FIELD(temp_r5_449, s16 *, 6);
            if (sp28 == (temp_r6_436 + 0x54)) {
                Func_080f9010(0xD4);
            }
            if (sp28 >= (s32) (temp_r6_436 + 0x55)) {
                temp_r1_469 = M2C_FIELD(temp_r5_449, s32 *, 0xC);
                temp_r2_472 = M2C_FIELD(temp_r5_449, s32 *, 0x10);
                M2C_FIELD(temp_r3_447, s32 *, 0x7080) = (s32) (M2C_FIELD(temp_r3_447, s32 *, 0x7080) + temp_r1_469);
                M2C_FIELD(temp_r5_449, s32 *, 4) = (s32) (M2C_FIELD(temp_r5_449, s32 *, 4) + temp_r2_472);
                M2C_FIELD(temp_r5_449, s32 *, 0xC) = (s32) (temp_r1_469 + 0xFFFF0000);
                M2C_FIELD(temp_r5_449, s32 *, 0x10) = (s32) (temp_r2_472 + 0x20000);
                sp0 = 0x10;
                sp4 = 0x15;
                DRAW_RECTANGLE(sp34, temp_r3_25 + 0x16AC, temp_r8_454 + 4, temp_sl_455 - 0x28);
                sp0 = 0x1D;
                sp4 = 0x23;
                DRAW_RECTANGLE(sp34, temp_r3_25 + 0x17FC, temp_r8_454 - 0x10, temp_sl_455 - 0x13);
                sp4 = 0x18;
                var_r1_515 = temp_r3_25 + 0x1BF3;
                var_r2_516 = temp_r8_454 - 0x14;
                var_r3_517 = temp_sl_455 + 0x10;
                sp0 = 0x15;
                goto block_36;
            }
            if (sp28 < (s32) (temp_r6_436 + 0x50)) {
                var_r9_710 = 0;
                var_r7_717 = (sp2C * 0x480) + 0x02013800;
                var_r5_723 = (u8 *)&absolute_02010000 + (sp2C * 0x2A0);
                do {
                    if (*(s32 *)var_r5_723 > 0) {
                        Func_08004a44(var_r7_717);
                        M2C_FIELD(spC, s32 *, 0) = *(s32 *)var_r5_723;
                        Func_080e3944(spC, &sp60);
                        M2C_FIELD(&sp60, s32 *, 0) = (s32) (((s32) M2C_FIELD(&sp60, s32 *, 0) >> 1) + temp_r8_454);
                        M2C_FIELD(&sp60, s32 *, 4) = (s32) (M2C_FIELD(&sp60, s32 *, 4) + temp_sl_455 + 0x10);
                        *(s32 *)var_r5_723 -= 4;
                        sp0 = 5;
                        sp4 = 0xA;
                        draw_args[1]((void *)sp34, (u8 *)sp24 + M2C_FIELD((void *)0x080EDE48, u16 *, 8), M2C_FIELD(&sp60, s32 *, 0) - 2, M2C_FIELD(&sp60, s32 *, 4) - 5, 5, 10);
                    }
                    var_r9_710 += 1;
                    var_r7_717 += 0x30;
                    var_r5_723 += 0x1C;
                } while (var_r9_710 != 0x18);
            } else {
                temp_r3_529 = (sp28 - temp_r7_439) - 0x10;
                switch (temp_r3_529) {              /* irregular */
                case 0:
                    sp0 = 0xE;
                    sp4 = 0x1C;
                    DRAW_RECTANGLE(sp34, temp_r3_25, temp_r8_454 - 7, temp_sl_455 - 0xE);
                    break;
                case 1:
                    sp0 = 0x17;
                    sp4 = 0x2C;
                    var_r1_515 = temp_r3_25 + 0x188;
                    var_r2_516 = temp_r8_454 - 0xB;
                    var_r3_517 = temp_sl_455 - 0x16;
block_36:
                    DRAW_RECTANGLE(sp34, var_r1_515, var_r2_516, var_r3_517);
                    break;
                case 2:
                    sp0 = 0x14;
                    sp4 = 0x1E;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0x57C, temp_r8_454 - 4, temp_sl_455 - 0x1F);
                    sp0 = 0x16;
                    sp4 = 0x21;
                    var_r1_515 = temp_r3_25 + 0x7D4;
                    var_r2_516 = temp_r8_454 - 0x10;
                    var_r3_517 = temp_sl_455 - 1;
                    goto block_36;
                case 3:
                    sp0 = 0x12;
                    sp4 = 0x1B;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0xAAA, temp_r8_454 + 1, temp_sl_455 - 0x26);
                    sp0 = 0x16;
                    sp4 = 0x16;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0xC90, temp_r8_454 - 0xB, temp_sl_455 - 0xB);
                    sp0 = 0x13;
                    sp4 = 0x1C;
                    var_r1_515 = temp_r3_25 + 0xE74;
                    var_r2_516 = temp_r8_454 - 0x13;
                    var_r3_517 = temp_sl_455 + 0xB;
                    goto block_36;
                case 4:
                    sp0 = 0x10;
                    sp4 = 0x17;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0x1088, temp_r8_454 + 4, temp_sl_455 - 0x28);
                    sp0 = 0x17;
                    sp4 = 0x1C;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0x11F8, temp_r8_454 - 0xA, temp_sl_455 - 0x11);
                    sp0 = 0x14;
                    var_r1_515 = temp_r3_25 + 0x147C;
                    var_r2_516 = temp_r8_454 - 0x14;
                    var_r3_517 = temp_sl_455 + 0xB;
                    sp4 = 0x1C;
                    goto block_36;
                }
            }
        }
        temp_r3_773 = sp2C + 1;
        sp2C = temp_r3_773;
        if (temp_r3_773 != 0x10) {
            goto loop_20;
        }
        M2C_FIELD(temp_r3_25, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        temp_r0_786 = sp28 + 1;
        sp28 = temp_r0_786;
        if ((temp_r0_786 != 0xDC) && !(*(u32 *)0x03001B04 & 3)) {
            goto loop_9;
        }
    }
    Func_08004278(0x080C90E5);
    sp38->field_0010 = 0;
    absolute_03001ad0.field_0004 = sp20;
    Func_080d67dc();
    sp2C = 0;
    var_r5_811 = temp_r3_25 + 0x77D8;
    do {
        temp_r0_813 = *(s32 *)var_r5_811;
        var_r5_811 += 4;
        Func_08009038(temp_r0_813);
        temp_r0_816 = sp2C + 1;
        sp2C = temp_r0_816;
    } while (temp_r0_816 != 9);
    absolute_04000020.field_0000 = 0x80;
    *(u16 *)0x04000000 = 0x7741;
    Func_080e0524(0xB4, &absolute_02010000, 1, 0);
    sp2C = 0;
    var_r5_836 = temp_r3_25 + 0x7080;
    do {
        temp_r0_841 = Func_080022fc(sp2C, 6);
        temp_r2_844 = M2C_FIELD(temp_r3_25, void **, 0x7828);
        if (temp_r0_841 < (s32) M2C_FIELD(temp_r2_844, s32 *, 0x14)) {
            Func_080e396c(M2C_FIELD(temp_r2_844, s16 *,
                (temp_r0_841 * 2) + 0x24), actor_position);
            temp_r0_885 = 0 - ((Func_08004458() & 0x1F) + 0x28);
            M2C_FIELD(var_r5_836, s32 *, 4) = temp_r0_885;
            temp_r1_887 = actor_position[0];
            temp_r3_891 = 0x50 - temp_r0_885;
            M2C_FIELD(var_r5_836, s32 *, 0) = (s32) (((s32) (temp_r1_887 + (temp_r1_887 >> 0x1F)) >> 1) + ((s32) (temp_r3_891 + (temp_r3_891 >> 0x1F)) >> 1));
        } else {
            M2C_FIELD(var_r5_836, s32 *, 0) = (s32) ((0x3F & Func_08004458()) + 0x50);
            M2C_FIELD(var_r5_836, s32 *, 4) = 0 - ((Func_08004458() & 0x1F) + 0x28);
        }
        M2C_FIELD(var_r5_836, s32 *, 0x18) = -1U;
        temp_r2_915 = sp2C + 1;
        var_r5_836 += 0x1C;
        sp2C = temp_r2_915;
    } while (temp_r2_915 != 0x20);
    sp28 = 0;
loop_57:
    sp2C = 0;
    var_r7_927 = temp_r3_25 + 0x7080;
loop_58:
    if ((sp28 < (s32) (sp2C * 2)) && (sp28 <= 0x28)) {

    } else {
        temp_r3_938 = M2C_FIELD(var_r7_927, s32 *, 0x18);
        if ((s32) temp_r3_938 >= 0) {
            if ((s32) temp_r3_938 <= 0x17) {
                var_r6_943 = temp_r3_938;
                if ((s32) temp_r3_938 < 0) {
                    var_r6_943 = temp_r3_938 + 3;
                }
                temp_r6_949 = (s32) var_r6_943 >> 2;
                temp_r5_956 = *(u8 *)(0x080EDE9F + temp_r6_949);
                sp0 = (s32) temp_r5_956;
                sp4 = *(u8 *)(0x080EDEA5 + temp_r6_949);
                draw_args[sp2C & 1]((void *)sp34,
                    (struct M2cAggregate_absolute_02010000 *)
                        &absolute_02010000.unknown_0000[
                            *(u16 *)(0x080EDEB2 + (temp_r6_949 * 2))],
                    (M2C_FIELD(var_r7_927, s32 *, 0) -
                        (temp_r5_956 >> 1)) - 8,
                    (M2C_FIELD(var_r7_927, s32 *, 4) +
                        *(u8 *)(0x080EDEAB + temp_r6_949)) - 0x28, sp0, sp4);
                if ((s32) M2C_FIELD(var_r7_927, s32 *, 0x18) <= 0xB) {
                    sp0 = 0x10;
                    sp4 = 0x15;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0x16AC, M2C_FIELD(var_r7_927, s32 *, 0) + 4, M2C_FIELD(var_r7_927, s32 *, 4) - 0x28);
                    sp0 = 0x1D;
                    sp4 = 0x23;
                    DRAW_RECTANGLE(sp34, temp_r3_25 + 0x17FC, M2C_FIELD(var_r7_927, s32 *, 0) - 0x10, M2C_FIELD(var_r7_927, s32 *, 4) - 0x13);
                }
            }
            M2C_FIELD(var_r7_927, s32 *, 0x18) += 1;
        } else {
            temp_r1_1012 = M2C_FIELD(var_r7_927, s32 *, 4);
            var_r5_1013 = 0x18;
            if ((s32) temp_r1_1012 > 0x38) {
                var_r5_1013 = (0x18 - temp_r1_1012) + 0x38;
            }
            sp0 = 0x10;
            sp4 = 0x15;
            DRAW_RECTANGLE(sp34, temp_r3_25 + 0x16AC, M2C_FIELD(var_r7_927, s32 *, 0) + 4, temp_r1_1012 - 0x28);
            sp0 = 0x1D;
            sp4 = 0x23;
            DRAW_RECTANGLE(sp34, temp_r3_25 + 0x17FC, M2C_FIELD(var_r7_927, s32 *, 0) - 0x10, M2C_FIELD(var_r7_927, s32 *, 4) - 0x13);
            if (var_r5_1013 > 0) {
                sp0 = 0x15;
                sp4 = var_r5_1013;
                DRAW_RECTANGLE(sp34, temp_r3_25 + 0x1BF3, M2C_FIELD(var_r7_927, s32 *, 0) - 0x14, M2C_FIELD(var_r7_927, s32 *, 4) + 0x10);
            }
            M2C_FIELD(var_r7_927, s32 *, 0) = (s32) (M2C_FIELD(var_r7_927, s32 *, 0) - 6);
            temp_r3_1064 = M2C_FIELD(var_r7_927, s32 *, 4) + 0xC;
            M2C_FIELD(var_r7_927, s32 *, 4) = temp_r3_1064;
            if ((s32) temp_r3_1064 > 0x4F) {
                M2C_FIELD(var_r7_927, s32 *, 0x18) = 0;
                M2C_FIELD(temp_r3_25, s32 *, 0x77A8) = 2;
                Func_080f9010(0x86);
                temp_r0_1078 = Func_080022fc(sp2C, 6);
                temp_r2_1081 = M2C_FIELD(temp_r3_25, void **, 0x7828);
                if (temp_r0_1078 < (s32) M2C_FIELD(temp_r2_1081, s32 *, 0x14)) {
                    temp_r5_1087 = (temp_r0_1078 * 2) + 0x24;
                    sp0 = 8;
                    Func_080d6888(M2C_FIELD(temp_r2_1081, s16 *,
                        temp_r5_1087), 7, 5, temp_r0_1078, 8);
                    Func_080b5088(M2C_FIELD(
                        M2C_FIELD(temp_r3_25, void **, 0x7828), s16 *,
                        temp_r5_1087), 1);
                }
            }
        }
    }
    temp_r3_1101 = sp2C + 1;
    var_r7_927 += 0x1C;
    sp2C = temp_r3_1101;
    if (temp_r3_1101 != 0x18) {
        goto loop_58;
    }
    Func_080e155c(4, 8);
    Func_080cd52c();
    M2C_FIELD(temp_r3_25, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    temp_r0_1119 = sp28 + 1;
    sp28 = temp_r0_1119;
    if (temp_r0_1119 != 0x58) {
        goto loop_57;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
