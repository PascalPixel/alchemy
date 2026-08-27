#include "types.h"

/*
 * Full source-shape draft for the effect sequence at 0x080e7404.
 *
 * Aggregate names remain provisional.  Byte-offset accesses are retained
 * where the repository does not yet provide an evidence-backed structure.
 *
 * Two reference shapes in this owner are outside what the approved
 * compiler can emit from C, so this draft is a near reconstruction and
 * not a candidate for byte identity:
 *
 *   1. Both DMA descriptor writes to 0x040000d4 are one standalone
 *      three-register Thumb store multiple (`stmia r3!, {r0, r1, r2}`
 *      followed by a dead `subs r3, #12`).  gcc 2.96's Thumb backend
 *      emits multi-register transfers only from movmem8b/movmem12b, and
 *      those always print an immediately adjacent `ldmia` before the
 *      `stmia` (arm.c thumb_output_move_mem_multiple, arm.md movmem12b);
 *      the generic store_multiple patterns are TARGET_ARM only.  This
 *      draft writes the three descriptor words separately instead.
 *
 *   2. Before each `Func_080e7338` call the reference loads r9 with the
 *      frame top (`add r2, sp, #284; mov r9, r2`) and never reads r9
 *      again in this owner.  Func_080e7338 reads r9 on entry and
 *      dereferences r9 - 136, which is this frame's work-pointer slot,
 *      so r9 is an implicit register argument.  Func_080e7338 is already
 *      classified as a hidden_register_context_module; the caller side of
 *      that contract is equally inexpressible without a register pin.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopy)(void *, const void *, s32);
typedef void (*DrawRectangle)(
    void *, const void *, s32, s32, s32, s32);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40 (the draw destination)
   and 46 (the rectangle entry Func_080ed408 loads), plus kind 44. */
extern void *Data_03001e50[];

/* Two IWRAM cells the reference addresses through one base register
   plus a field offset, so each is named as an aggregate rather than
   as a folded absolute address. */
struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 Data_03001ad0;

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 Data_03001ce0;

/* Sixteen halfword cell offsets; the reference keeps the table base in
   a register and indexes it, so it is named rather than folded into a
   biased literal. */
extern u16 Data_080ede48[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_000000c0;
extern u8 Value_000000c1;
extern u8 Value_000000c4;

void *Func_08009030(s32);
void **Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_080c9048(void);
void Func_080041d8(s32, s32);
void Func_080cd104(s32, s32);
void Func_08004278(s32);
void Func_08009020(void *, s32);
void Func_080dbb24(s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
s32 Func_08004458(void);
void Func_080d6750(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_08009008(s32, void *, void *, s32);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c6c(s32);
void Func_08004c1c(s32);
void Func_080e3944(const void *, void *);
void Func_080e7338(s32, s32, s32);
void Func_08002dd8(s32);
void Func_080d67dc(void);
void Func_080e727c(s32, s32, s32);
void Func_08009038(s32);
void Func_080b5118(void);
void Func_080b50e8(s32);
void Func_080051d8(s32, s32);
void Func_080e38b8(void *, s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080e155c(s32, s32);
s32 Func_080cdbc0(void);

s32 Func_080e7404(s32 arg0, s32 arg1)
{
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    void *sp18;
    void *sp1C;
    void **sp20;
    s32 sp24;
    void **sp28;
    void *sp2C;
    void *sp30;
    s32 sp34;
    s32 sp38;
    void **sp3C;
    s32 sp40;
    DrawRectangle draw_rectangle;
    void *draw_destination;
    s32 sp4C;
    u8 *var_r7_1383;
    u8 *var_r7_1551;
    s16 *var_r5_615;
    s16 *var_sl_235;
    s16 temp_r2_941;
    s16 temp_r3_946;
    s32 *var_r3_572;
    s32 temp_r0_1683;
    s32 temp_r1_1068;
    s32 temp_r1_1333;
    s32 temp_r1_434;
    s32 temp_r1_660;
    s32 temp_r1_848;
    s32 temp_r2_1687;
    s32 temp_r2_668;
    s32 temp_r3_1001;
    s32 temp_r3_1093;
    s32 temp_r3_1534;
    s32 temp_r3_1580;
    s32 temp_r3_748;
    s32 temp_r4_611;
    s32 temp_r5_1334;
    s32 temp_r5_1422;
    s32 temp_r5_1497;
    s32 temp_r5_314;
    s32 temp_r6_1503;
    s32 temp_r6_1570;
    s32 temp_r6_310;
    s32 var_fp_590;
    s32 var_r0_428;
    s32 var_r1_1643;
    s32 var_r1_672;
    s32 var_r2_1397;
    s32 var_r2_326;
    s32 var_r2_439;
    s32 var_r2_737;
    s32 var_r2_830;
    s32 var_r2_982;
    s32 var_r3_1413;
    s32 var_r3_320;
    s32 var_r3_445;
    s32 var_r3_606;
    s32 var_r3_661;
    s32 var_r3_671;
    s32 var_r3_713;
    s32 var_r3_859;
    s32 var_r3_875;
    s32 var_r3_972;
    s32 var_r5_135;
    s32 var_r5_410;
    s32 var_r6_1060;
    s32 var_r6_1223;
    s32 var_r6_136;
    s32 var_r6_408;
    s32 var_r6_763;
    s32 var_r7_718;
    s32 var_r7_979;
    s32 var_r8_1058;
    s32 var_r8_1221;
    s32 var_r8_1246;
    s32 var_r8_1276;
    s32 var_r8_137;
    s32 var_r8_1386;
    s32 var_r8_1490;
    s32 var_r8_1552;
    s32 var_r8_233;
    s32 var_r8_303;
    s32 var_r8_570;
    s32 var_r8_649;
    s32 var_r8_826;
    s32 var_r8_912;
    s32 var_r8_997;
    s32 var_sl_1491;
    s32 temp_r1_319;
    s32 temp_r3_916;
    s32 temp_r3_926;
    u16 temp_r4_1585;
    u32 temp_r0_1420;
    u32 temp_r0_1587;
    u32 temp_r2_1572;
    u32 temp_r3_1597;
    u32 temp_r4_1501;
    u32 var_fp_1329;
    u32 var_r8_409;
    u8 *var_r5_398;
    s32 temp_r5_271;
    s32 temp_r6_269;
    void *temp_r0_140;
    void *temp_r0_603;
    void *temp_r1_1495;
    void *temp_r1_154;
    void *temp_r2_1063;
    void *temp_r2_1122;
    void *temp_r2_1307;
    void *temp_r2_365;
    void *temp_r2_46;
    void *temp_r2_553;
    void *temp_r3_1065;
    void *temp_r3_1499;
    void *temp_r3_24;
    void *temp_r3_835;
    void *temp_r5_1067;
    void *temp_r5_1230;
    void *temp_r5_837;
    void *temp_r7_1505;
    void *var_r5_1245;
    void *var_r5_1275;
    void *var_r5_995;
    void *var_r7_911;
    void **cursor;
    void **display;
    u8 sp98[132];
    s32 sp84[5];
    s32 sp78[3];
    s32 sp6C[3];
    s32 sp60[3];
    s32 sp58[2];
    s32 sp50[2];

    cursor = &Data_03001e50[40];
    sp4C = arg1;
    sp3C = (void **)&sp84[4];
    draw_destination = *cursor;
    temp_r3_24 = cursor[-1];
    *sp3C = temp_r3_24;
    M2C_FIELD(temp_r3_24, s32 *, 0x7828) = arg0;
    Func_080cd594(0x2000);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    if (sp4C == 1) {
        temp_r2_46 = *Func_080b5098(M2C_FIELD(M2C_FIELD(*sp3C, void **, 0x7828), s32 *, 8));
        M2C_FIELD(temp_r2_46, s32 *, 0x28) = 0xA0000;
        M2C_FIELD(temp_r2_46, s32 *, 0x48) = 0x91EB;
        Func_080d6888(M2C_FIELD(M2C_FIELD(*sp3C, void **, 0x7828), s32 *, 8), -1, 2, -1, 0);
        Func_080f9010(0x91);
        sp40 = sp4C;
        if (M2C_FIELD(M2C_FIELD(*sp3C, void **, 0x7828), s32 *, 4) != 1) {
            sp40 = -1;
        }
    } else {
        sp40 = -1;
    }
    Func_080c9048();
    M2C_FIELD((void *)0x05000000, s16 *, 0) = 0;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = 0;
    M2C_FIELD(*sp3C, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(0, 0);
    Func_08004278(0x080CD261);
    if (sp4C == 1) {
        var_r5_135 = 0x77D8;
        var_r6_136 = 0x1E3;
        var_r8_137 = 0;
        do {
            temp_r0_140 = Func_08009030(var_r6_136);
            *(void **)((u8 *)*sp3C + var_r5_135) = temp_r0_140;
            if (temp_r0_140 != NULL) {
                M2C_FIELD(temp_r0_140, s8 *, 0x26) = 0;
                Func_08009020(temp_r0_140, 2);
                temp_r1_154 = *(void **)((u8 *)*sp3C + var_r5_135);
                M2C_FIELD(temp_r1_154, u8 *, 9) = (u8) (M2C_FIELD(temp_r1_154, u8 *, 9) | 0xC);
            }
            var_r8_137 += 1;
            var_r5_135 += 4;
            var_r6_136 += 0x2001;
        } while (var_r8_137 != 2);
    } else {
        Func_080dbb24(1, 0x17D, 3);
    }
    Func_080e0524((s32)&Value_000000c1, *sp3C, 1, 1);
    if (sp4C == 1) {
        ((WordCopy)0x03001388)(
            (void *)0x05000000, (void *)Func_08002f40((s32)&Value_000000c4), 0x80);
    }
    *(s32 *)(sp98 + 128) = 0x01010101;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = (s32 *)(sp98 + 128);
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x02010000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85002000;
    ((WordCopy)0x03001388)(
        (void *)0x06008000, (void *)0x02010000, 0x7800);
    Func_080030f8(1);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x0400000A, s16 *, 0) = 0x1F80;
    M2C_FIELD((void *)0x0400000A, s16 *, 2) = 0x2787;
    var_r8_233 = 0;
    var_sl_235 = (s16 *)0x05000100;
    do {
        temp_r6_269 = Func_08004458();
        temp_r5_271 = Func_08004458();
        var_r8_233 += 1;
        *var_sl_235 = (((Func_08004458() & 0xF) + 0x10) << 0xA) | (((temp_r5_271 & 0xF) + 0x10) << 5) | ((temp_r6_269 & 0xF) + 0x10);
        var_sl_235 += 1;
    } while (var_r8_233 != 0x3F);
    *(s32 *)(sp98 + 128) = 0;
    M2C_FIELD((void *)0x040000D4, s32 **, 0) = (s32 *)(sp98 + 128);
    M2C_FIELD((void *)0x040000D4, void **, 4) = draw_destination;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x85001000;
    var_r8_303 = 0;
    do {
        temp_r6_310 = Func_08004458() & 0x7F;
        temp_r5_314 = Func_08004458() & 0x7F;
        temp_r1_319 = (0x3F & Func_08004458()) + 0x40;
        var_r3_320 = temp_r5_314;
        if (temp_r5_314 < 0) {
            var_r3_320 = temp_r5_314 + 7;
        }
        var_r2_326 = temp_r6_310;
        if (temp_r6_310 < 0) {
            var_r2_326 = temp_r6_310 + 7;
        }
        *((u8 *)draw_destination + (((((((var_r3_320 >> 3) * 0x10)
            + (var_r2_326 >> 3)) * 8) + (temp_r5_314 & 7)) * 8)
            + (temp_r6_310 & 7))) = temp_r1_319;
        var_r8_303 += 1;
    } while (var_r8_303 != 0x100);
    ((WordCopy)0x03001388)(
        (void *)0x06004000, draw_destination, 0x4000);
    Data_03001ce0.unk10 = 0xF0;
    Func_080d6750(M2C_FIELD(*sp3C, s32 *, 0x7828));
    temp_r2_365 = *sp3C;
    M2C_FIELD(temp_r2_365, s32 *, 0x77D0) = 0;
    M2C_FIELD(temp_r2_365, s32 *, 0x77D4) = 0;
    M2C_FIELD(temp_r2_365, s32 *, 0x7790) = 0;
    M2C_FIELD(temp_r2_365, s32 *, 0x7794) = 2;
    M2C_FIELD(temp_r2_365, s32 *, 0x7798) = (s32) (sp40 << 7);
    M2C_FIELD(temp_r2_365, s32 *, 0x779C) = var_r8_303;
    Func_080041d8(0x080C9139, 0x4FF);
    Func_080041d8(0x080CD359, 0x480);
    var_r5_398 = sp98;
    do {
        *var_r5_398 = Func_08004458() & 0x3F;
        var_r5_398 += 1;
    } while (var_r5_398 != sp98 + 128);
    var_r6_408 = 0;
    var_r8_409 = 1;
    var_r5_410 = 0;
    do {
        var_r6_408 += (s32) ((var_r8_409 >> 0x1F) + var_r8_409) >> 1;
        var_r8_409 += 4;
        if (var_r5_410 != var_r6_408) {
            do {
                var_r0_428 = 0;
loop_33:
                temp_r1_434 = var_r5_410 - sp98[var_r0_428 & 0x7F];
                /* Two ordered signed bounds, not one folded unsigned
                   range test: the reference compares against 0 and 127
                   separately. */
                if (temp_r1_434 >= 0) {
                if (temp_r1_434 <= 0x7F) {
                    var_r2_439 = temp_r1_434;
                    if (temp_r1_434 < 0) {
                        var_r2_439 = temp_r1_434 + 7;
                    }
                    var_r3_445 = var_r0_428;
                    if (var_r0_428 < 0) {
                        var_r3_445 = var_r0_428 + 7;
                    }
                    M2C_FIELD((((((((var_r2_439 >> 3) << 5) + (var_r3_445 >> 3)) * 8) + (temp_r1_434 & 7)) * 8) + (var_r0_428 & 7)), s8 *, 0x02010000) = 0;
                }
                }
                var_r0_428 += 1;
                if (var_r0_428 != 0x100) {
                    goto loop_33;
                }
                var_r5_410 += 1;
            } while (var_r5_410 != var_r6_408);
        }
        M2C_FIELD(*sp3C, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    } while (var_r6_408 <= 0xBF);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F42;
    M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x1010;
    sp38 = (s32) Data_03001ad0.unk04;
    sp34 = (s32) Data_03001ad0.unk06;
    display = &Data_03001e50[44];
    sp30 = display[0];
    Data_03001ad0.unk04 = 0U;
    Data_03001ad0.unk06 = 0x20U;
    Func_080ed408(0x2E, 8, 7, 3, 2);
    draw_rectangle = (DrawRectangle) display[2];
    temp_r2_553 = *sp3C;
    M2C_FIELD(temp_r2_553, s32 *, 0x7780) = 3;
    M2C_FIELD(temp_r2_553, s32 *, 0x7784) = 0x02020202;
    Func_080041d8(0x080E72E1, 0x4FE);
    var_r8_570 = 0;
    var_r3_572 = *sp3C + 0x7098;
    do {
        var_r8_570 += 1;
        *var_r3_572 = -1;
        var_r3_572 += 7;
    } while (var_r8_570 != 0x40);
    M2C_FIELD(sp30, s32 *, 0x10) = 1;
    M2C_FIELD(*sp3C, s32 *, 0x778C) = 0;
    var_fp_590 = 0;
    sp18 = sp84;
    sp2C = sp58;
    sp28 = sp3C;
    sp10 = 0;
loop_47:
    temp_r0_603 = *sp3C;
    var_r3_606 = M2C_FIELD(temp_r0_603, s32 *, 0x778C);
    if (var_r3_606 < 0) {
        var_r3_606 += 3;
    }
    temp_r4_611 = var_r3_606 >> 2;
    var_r5_615 = temp_r0_603 + 0x1F80;
    if (sp4C == 1) {
        if ((*(s32 *)0x03001B04 & 3) && (var_fp_590 > 0x10)) {

        } else {
            goto block_56;
        }
    } else if ((*(s32 *)0x03001B04 & 3) && (var_fp_590 > 4)) {

    } else {
block_56:
        if (var_fp_590 == 0) {
            sp8 = temp_r4_611;
            Func_080f9010(0x8D);
        }
        var_r8_649 = 0;
        do {
            var_r8_649 += 1;
            *var_r5_615 = 0;
            var_r5_615 += 1;
        } while (var_r8_649 != 0xF);
        do {
            temp_r1_660 = var_r8_649 - 0x10;
            var_r3_661 = temp_r1_660;
            if (temp_r1_660 < 0) {
                var_r3_661 = var_r8_649 - 0xD;
            }
            temp_r2_668 = (var_r3_661 >> 2) + temp_r4_611;
            var_r3_671 = temp_r2_668 - 0x20;
            var_r1_672 = temp_r2_668 - 0x50;
            if (var_r3_671 < 0) {
                var_r3_671 = 0;
            }
            if (var_r3_671 > 0x1F) {
                var_r3_671 = 0x1F;
            }
            if (var_r1_672 < 0) {
                var_r1_672 = 0;
            }
            if (var_r1_672 > 0x1F) {
                var_r1_672 = 0x1F;
            }
            var_r8_649 += 1;
            *var_r5_615 = (var_r3_671 << 0xA) | (var_r1_672 << 5) | (var_r1_672 >> 1);
            var_r5_615 += 1;
        } while (var_r8_649 != 0x87);
        do {
            var_r8_649 += 1;
            *var_r5_615 = 0;
            var_r5_615 += 1;
        } while (var_r8_649 != 0xA0);
        if (sp40 == 1) {
            var_r3_713 = var_fp_590;
            if (var_r3_713 < 0) {
                var_r3_713 += 3;
            }
            var_r7_718 = var_r3_713 >> 2;
        } else {
            var_r2_737 = var_fp_590;
            if (var_r2_737 < 0) {
                var_r2_737 += 3;
            }
            var_r7_718 = 0x40 - (var_r2_737 >> 2);
        }
        temp_r3_748 = 0x60 - var_fp_590;
        M2C_FIELD(sp18, s32 *, 0xC) = 0;
        M2C_FIELD(sp18, s32 *, 4) = 0xFF0000;
        if (sp4C == 1) {
            var_r6_763 = sp10 + 0xA000;
            sp58[0] = var_r6_763;
            M2C_FIELD(sp2C, s32 *, 4) = var_r6_763;
            M2C_FIELD(sp18, s32 *, 0) = (s32) ((var_r7_718 << 0x10) + 0x500000);
            M2C_FIELD(sp18, s32 *, 8) = (s32) ((0x40 - temp_r3_748) << 0x10);
            Func_08009008(M2C_FIELD(*sp28, s32 *, 0x77D8), sp18, sp2C, 0);
            Func_08009008(M2C_FIELD(*sp28, s32 *, 0x77DC), sp18, sp2C, 0);
        } else {
            var_r6_763 = sp10 + 0x10000;
            sp58[0] = var_r6_763;
            M2C_FIELD(sp2C, s32 *, 4) = var_r6_763;
            M2C_FIELD(sp18, s32 *, 0) = (s32) ((var_r7_718 << 0x10) + 0x600000);
            M2C_FIELD(sp18, s32 *, 8) = (s32) ((0x60 - temp_r3_748) << 0x10);
            Func_08009008(M2C_FIELD(*sp3C, s32 *, 0x77D8), sp18, sp2C, 0);
        }
        var_r8_826 = 0;
        var_r2_830 = 0;
loop_84:
        temp_r3_835 = *sp3C + var_r2_830;
        temp_r5_837 = temp_r3_835 + 0x7080;
        if (M2C_FIELD(temp_r5_837, s32 *, 0x18) == -1) {
            temp_r1_848 = (0x7FFF & Func_08004458()) + 0x4000;
            M2C_FIELD(temp_r5_837, s32 *, 0x18) = 0;
            spC = temp_r1_848;
            var_r3_859 = Func_08002322(temp_r1_848) * 0x1E;
            if (var_r3_859 < 0) {
                var_r3_859 += 0xFFFF;
            }
            M2C_FIELD(temp_r3_835, s32 *, 0x7080) = (s32) (((var_r7_718 + 0x60) << 0x10) + ((var_r3_859 >> 0x10) * var_r6_763));
            var_r3_875 = Func_0800231c(temp_r1_848) * 0x1E;
            if (var_r3_875 < 0) {
                var_r3_875 += 0xFFFF;
            }
            M2C_FIELD(temp_r5_837, s32 *, 4) = (s32) (((0x20 - temp_r3_748) << 0x10) - ((var_r3_875 >> 0x10) * var_r6_763));
        } else {
            var_r8_826 += 1;
            var_r2_830 += 0x1C;
            if (var_r8_826 != 0x20) {
                goto loop_84;
            }
        }
        sp60[0] = 0;
        sp60[1] = 0;
        sp60[2] = 0x02000000;
        Func_080049ac();
        Func_08004cb4(sp60);
        Func_08004c6c(0x800);
        Func_08004c1c(sp10);
        var_r7_911 = (void *)0x080EEE76;
        var_r8_912 = 0;
        do {
            /* The reference keeps the unsigned halfword load and does the
               sign extension with the same shifted value the rounding bit
               comes from, so the halving is spelled out on the shifted
               word rather than through an s16 cast. */
            temp_r3_916 = M2C_FIELD(var_r7_911, u16 *, 0) << 0x10;
            sp78[1] = (s32) ((M2C_FIELD(var_r7_911, s16 *, 2) + var_fp_590) << 0x10);
            temp_r3_926 = M2C_FIELD(var_r7_911, u16 *, 4) << 0x10;
            sp78[0] = (s32) ((((temp_r3_916 >> 0x10)
                + (s32) ((u32) temp_r3_916 >> 0x1F)) >> 1) << 0x10);
            sp78[2] = (s32) ((((temp_r3_926 >> 0x10)
                + (s32) ((u32) temp_r3_926 >> 0x1F)) >> 1) << 0x10);
            Func_080e3944(sp78, sp6C);
            temp_r2_941 = ((s16 *)sp6C)[1];
            sp6C[0] = (s32) (temp_r2_941 + 0x80);
            temp_r3_946 = ((s16 *)sp6C)[3];
            sp6C[1] = (s32) (temp_r3_946 + 0x3C);
            draw_rectangle((void *)0x02010000, *sp3C + 0x1F40,
                temp_r2_941 + 0x7C, temp_r3_946 + 0x38, 8U, 8);
            var_r8_912 += 1;
            var_r7_911 += 6;
        } while (var_r8_912 != 7);
        if (sp40 == 1) {
            var_r3_972 = var_fp_590;
            if (var_r3_972 < 0) {
                var_r3_972 += 3;
            }
            var_r7_979 = (var_r3_972 >> 2) - 0x10;
        } else {
            var_r2_982 = var_fp_590;
            if (var_r2_982 < 0) {
                var_r2_982 += 3;
            }
            var_r7_979 = 0x10 - (var_r2_982 >> 2);
        }
        var_r5_995 = (void *)0x080EEEA0;
        var_r8_997 = 0;
        do {
            temp_r3_1001 = M2C_FIELD(var_r5_995, s16 *, 2) + (var_fp_590 - 0x60);
            if (temp_r3_1001 <= 0x5D) {
                draw_rectangle((void *)0x02010000, *sp3C + 0x1C80,
                    (M2C_FIELD(var_r5_995, s16 *, 0) + var_r7_979) - 0xC,
                    temp_r3_1001 - 0xC, 0x18U, 0x18);
            } else if (temp_r3_1001 <= 0x5F) {
                Func_080e7338((M2C_FIELD(var_r5_995, s16 *, 0) + var_r7_979) << 0x10, temp_r3_1001 << 0x10, 1);
            }
            var_r8_997 += 1;
            var_r5_995 += 4;
        } while (var_r8_997 != 7);
        var_r8_1058 = 0;
        var_r6_1060 = 0;
        do {
            temp_r2_1063 = *sp3C;
            temp_r3_1065 = temp_r2_1063 + var_r6_1060;
            temp_r5_1067 = temp_r3_1065 + 0x7080;
            temp_r1_1068 = M2C_FIELD(temp_r5_1067, s32 *, 0x18);
            if (temp_r1_1068 >= 0) {
                draw_rectangle((void *)0x02010000,
                    temp_r2_1063 + (temp_r1_1068 << 0xA),
                    M2C_FIELD(temp_r5_1067, s16 *, 2) - 0x10,
                    M2C_FIELD(temp_r5_1067, s16 *, 6) - 0x10,
                    0x20U, 0x20);
                M2C_FIELD(temp_r3_1065, s32 *, 0x7080) = (s32) (M2C_FIELD(temp_r3_1065, s32 *, 0x7080) - (sp40 * 0x14000));
                M2C_FIELD(temp_r5_1067, s32 *, 4) = (s32) (M2C_FIELD(temp_r5_1067, s32 *, 4) + 0xFFFB0000);
                temp_r3_1093 = M2C_FIELD(temp_r5_1067, s32 *, 0x18) + 1;
                M2C_FIELD(temp_r5_1067, s32 *, 0x18) = temp_r3_1093;
                if (temp_r3_1093 == 6) {
                    M2C_FIELD(temp_r5_1067, s32 *, 0x18) = -1;
                }
            }
            var_r8_1058 += 1;
            var_r6_1060 += 0x1C;
        } while (var_r8_1058 != 0x20);
        M2C_FIELD(*sp28, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        sp10 += 0x100;
        temp_r2_1122 = *sp28;
        var_fp_590 += 1;
        M2C_FIELD(temp_r2_1122, s32 *, 0x778C) = (s32) (M2C_FIELD(temp_r2_1122, s32 *, 0x778C) + 1);
        if (var_fp_590 != 0xC0) {
            goto loop_47;
        }
    }
    Func_080030f8(1);
    M2C_FIELD(sp30, s32 *, 0x10) = 0;
    Func_08004278(0x080C9139);
    Func_08004278(0x080E72E1);
    Func_08004278(0x080CD359);
    Data_03001ad0.unk04 = (u16) sp38;
    Data_03001ad0.unk06 = (u16) sp34;
    Func_08002dd8(0x2E);
    Func_080d67dc();
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
    *(s32 *)0x04000028 = 0;
    M2C_FIELD((void *)0x04000020, s32 *, 0xC) = 0xFFFFF000;
    M2C_FIELD((void *)0x04000020, s16 *, 0x32) = 0x1010;
    M2C_FIELD((void *)0x04000020, s16 *, -0x14) = 0x2784;
    Func_080ed408(0x2E, 7, 7, 3, 2);
    draw_rectangle = (DrawRectangle) Data_03001e50[46];
    Func_080e0524((s32)&Value_000000c0, *sp3C, 1, 0);
    var_r8_1221 = 0;
    var_r6_1223 = 0;
    do {
        temp_r5_1230 = *sp3C + var_r6_1223 + 0x7080;
        M2C_FIELD(temp_r5_1230, s32 *, 0) = (s32) (Func_08004458() & 0x7F);
        var_r8_1221 += 1;
        M2C_FIELD(temp_r5_1230, s32 *, 4) = (s32) ((Func_08004458() & 0x7F) + 0x7F);
        var_r6_1223 += 0x1C;
    } while (var_r8_1221 != 0x20);
    var_r5_1245 = (void *)0x02010000;
    var_r8_1246 = 0;
    do {
        M2C_FIELD(var_r5_1245, s32 *, 0) = 0;
        M2C_FIELD(var_r5_1245, s32 *, 4) = 0;
        M2C_FIELD(var_r5_1245, s32 *, 8) = 0;
        M2C_FIELD(var_r5_1245, s32 *, 0xC) = (s32) (((Func_08004458() & 0xFF) - 0x7F) << 0xC);
        M2C_FIELD(var_r5_1245, s32 *, 0x10) = (s32) ((Func_08004458() & 0xFF) << 0xB);
        var_r8_1246 += 1;
        M2C_FIELD(var_r5_1245, s32 *, 0x14) = (s32) (((Func_08004458() & 0xFF) - 0x7F) << 0xC);
        M2C_FIELD(var_r5_1245, s32 *, 0x18) = 0;
        var_r5_1245 += 0x1C;
    } while (var_r8_1246 != 0x80);
    var_r5_1275 = (void *)0x02010E00;
    var_r8_1276 = 0;
    do {
        M2C_FIELD(var_r5_1275, s32 *, 0) = 0;
        M2C_FIELD(var_r5_1275, s32 *, 4) = 0;
        M2C_FIELD(var_r5_1275, s32 *, 8) = 0;
        M2C_FIELD(var_r5_1275, s32 *, 0xC) = (s32) (((Func_08004458() & 0xFF) - 0x80) << 0xD);
        M2C_FIELD(var_r5_1275, s32 *, 0x10) = (s32) ((Func_08004458() & 0xFF) << 0xB);
        var_r8_1276 += 1;
        M2C_FIELD(var_r5_1275, s32 *, 0x14) = (s32) (((Func_08004458() & 0xFF) - 0x80) << 0xD);
        M2C_FIELD(var_r5_1275, s32 *, 0x18) = 0;
        var_r5_1275 += 0x1C;
    } while (var_r8_1276 != 0x200);
    temp_r2_1307 = *sp3C;
    M2C_FIELD(temp_r2_1307, s32 *, 0x7780) = 1;
    M2C_FIELD(temp_r2_1307, s32 *, 0x7784) = 0x10101010;
    Func_080041d8(0x080CD261, 0x480);
    sp20 = sp3C;
    sp1C = sp50;
    sp14 = 0x1D000;
    var_fp_1329 = 0;
loop_121:
    temp_r1_1333 = var_fp_1329 - 0x10;
    temp_r5_1334 = *(s32 *)0x03001E80;
    sp24 = temp_r1_1333;
    if (temp_r1_1333 > 0x13) {
        Func_080e727c(2, 2, 2);
    }
    if (var_fp_1329 == 0) {
        Func_080f9010(0x9C);
    }
    if (var_fp_1329 == 0x28) {
        Func_080f9010(0x91);
    }
    if (var_fp_1329 == 0x30) {
        if (sp4C == 1) {
            Func_08009038(M2C_FIELD(*sp20, s32 *, 0x77D8));
            Func_08009038(M2C_FIELD(*sp20, s32 *, 0x77DC));
            Func_080b5118();
        }
        Func_080b50e8(0x86);
    }
    Func_080049ac();
    Func_080051d8(temp_r5_1334, temp_r5_1334 + 0xC);
    var_r7_1383 = (u8 *)0x02010E00;
    var_r8_1386 = 0;
    do {
        if ((s32) M2C_FIELD(var_r7_1383, s32 *, 4) >= 0) {
            Func_080e3944(var_r7_1383, sp60);
            var_r2_1397 = sp60[2];
            sp60[0] = (s32) ((s32) sp60[0] >> 1);
            if (var_r2_1397 <= 0x9F) {
                sp60[2] = 0xA0;
                var_r2_1397 = 0xA0;
            }
            if (var_r2_1397 > 0x31F) {
                sp60[2] = 0x31F;
                var_r2_1397 = 0x31F;
            }
            var_r3_1413 = var_r2_1397 - 0xA0;
            if (var_r3_1413 < 0) {
                var_r3_1413 += 0x3F;
            }
            temp_r0_1420 = 9 - (var_r3_1413 >> 6);
            temp_r5_1422 = temp_r0_1420 * 2;
            draw_rectangle(draw_destination,
                *sp3C + (Data_080ede48[temp_r0_1420 - 1]
                    + ((var_r8_1386 & 1) * 0x302)) + 0x3200,
                sp60[0]
                    - ((s32)(temp_r0_1420 + (temp_r0_1420 >> 0x1F)) >> 1),
                sp60[1] - temp_r0_1420,
                temp_r0_1420, temp_r5_1422);
            Func_080e38b8(var_r7_1383, 0x40, 0xFFFFE000);
            if ((s32) M2C_FIELD(var_r7_1383, s32 *, 4) <= 0x140000) {
                M2C_FIELD(var_r7_1383, s32 *, 0) = 0;
                M2C_FIELD(var_r7_1383, s32 *, 8) = 0;
                M2C_FIELD(var_r7_1383, s32 *, 4) = 0x140000;
                M2C_FIELD(var_r7_1383, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xF);
                M2C_FIELD(var_r7_1383, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xD);
                M2C_FIELD(var_r7_1383, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xF);
            }
        }
        var_r8_1386 += 1;
        var_r7_1383 += 0x1C;
    } while (var_r8_1386 != 0x40);
    var_r8_1490 = 0;
    var_sl_1491 = 0;
    do {
        temp_r1_1495 = *sp3C;
        temp_r5_1497 = 7 & var_r8_1490;
        temp_r3_1499 = temp_r1_1495 + var_sl_1491;
        temp_r4_1501 = temp_r5_1497 + 3;
        temp_r6_1503 = temp_r4_1501 * 2;
        temp_r7_1505 = temp_r3_1499 + 0x7080;
        draw_rectangle(draw_destination,
            temp_r1_1495 + (Data_080ede48[temp_r4_1501 - 1]
                + ((var_r8_1490 & 1) * 0x302)) + 0x3200,
            M2C_FIELD(temp_r3_1499, s32 *, 0x7080)
                - (temp_r4_1501 >> 1),
            M2C_FIELD(temp_r7_1505, s32 *, 4) - temp_r4_1501,
            temp_r4_1501, temp_r6_1503);
        temp_r3_1534 = (M2C_FIELD(temp_r7_1505, s32 *, 4) - temp_r5_1497) - 8;
        M2C_FIELD(temp_r7_1505, s32 *, 4) = temp_r3_1534;
        if (temp_r3_1534 < -0xA) {
            M2C_FIELD(temp_r7_1505, s32 *, 4) = 0x80;
        }
        var_r8_1490 += 1;
        var_sl_1491 += 0x1C;
    } while (var_r8_1490 != 0x40);
    var_r7_1551 = (u8 *)0x02010000;
    var_r8_1552 = 0;
    do {
        if ((Func_080022ec(var_r8_1552, 3) < sp24) && ((s32) M2C_FIELD(var_r7_1551, s32 *, 4) >= 0)) {
            Func_080e3944(var_r7_1551, sp60);
            temp_r6_1570 = (s32) sp60[0] >> 1;
            sp60[0] = temp_r6_1570;
            temp_r2_1572 = M2C_FIELD(var_r7_1551, u32 *, 0x18);
            if (temp_r2_1572 <= 0xDU) {
                temp_r3_1580 = ((s32) (temp_r2_1572 + (temp_r2_1572 >> 0x1F)) >> 1) * 2;
                temp_r4_1585 = M2C_FIELD(temp_r3_1580, u16 *, 0x080EEECA);
                temp_r0_1587 = temp_r4_1585 >> 1;
                draw_rectangle(draw_destination,
                    *sp3C + M2C_FIELD(temp_r3_1580, u16 *, 0x080EEEBC),
                    temp_r6_1570 - temp_r0_1587,
                    sp60[1] - temp_r0_1587,
                    (u32)temp_r4_1585, (s32)temp_r4_1585);
            }
            temp_r3_1597 = M2C_FIELD(var_r7_1551, u32 *, 0x18) + 1;
            M2C_FIELD(var_r7_1551, u32 *, 0x18) = temp_r3_1597;
            if (temp_r3_1597 == 0xE) {
                M2C_FIELD(var_r7_1551, s32 *, 4) = 0x140000;
                M2C_FIELD(var_r7_1551, s32 *, 0) = 0;
                M2C_FIELD(var_r7_1551, s32 *, 8) = (s32) (((Func_08004458() & 0xFF) - 0x7F) << 0x10);
                M2C_FIELD(var_r7_1551, s32 *, 0xC) = 0;
                M2C_FIELD(var_r7_1551, s32 *, 0x10) = (s32) ((Func_08004458() & 0xFF) << 0xB);
                M2C_FIELD(var_r7_1551, s32 *, 0x14) = 0;
                M2C_FIELD(var_r7_1551, u32 *, 0x18) = 0U;
            } else {
                Func_080e38b8(var_r7_1551, 0x40, 1);
            }
        }
        var_r8_1552 += 1;
        var_r7_1551 += 0x1C;
    } while (var_r8_1552 != 0x40);
    if (sp40 == 1) {
        var_r1_1643 = ((s32) ((var_fp_1329 >> 0x1F) + var_fp_1329) >> 1) + 0x18;
    } else {
        var_r1_1643 = 0x38 - ((s32) ((var_fp_1329 >> 0x1F) + var_fp_1329) >> 1);
    }
    temp_r0_1683 = 0x40 - (var_fp_1329 * 2);
    temp_r2_1687 = (var_fp_1329 << 8) + 0x20000;
    M2C_FIELD(sp18, s32 *, 0xC) = 0;
    M2C_FIELD(sp18, s32 *, 4) = 0xFF0000;
    if (sp4C == 1) {
        sp50[0] = sp14;
        M2C_FIELD(sp1C, s32 *, 4) = sp14;
        M2C_FIELD(sp18, s32 *, 0) = (s32) ((var_r1_1643 << 0x10) + 0x600000);
        M2C_FIELD(sp18, s32 *, 8) = (s32) ((0x60 - temp_r0_1683) << 0x10);
        Func_08009008(M2C_FIELD(*sp20, s32 *, 0x77D8), sp18, sp1C, 0);
        Func_08009008(M2C_FIELD(*sp20, s32 *, 0x77DC), sp18, sp1C, 0);
    } else {
        sp50[0] = temp_r2_1687;
        M2C_FIELD(sp1C, s32 *, 4) = temp_r2_1687;
        M2C_FIELD(sp18, s32 *, 0) = (s32) ((var_r1_1643 << 0x10) + 0x600000);
        M2C_FIELD(sp18, s32 *, 8) = (s32) ((0x60 - temp_r0_1683) << 0x10);
        Func_08009008(M2C_FIELD(*sp3C, s32 *, 0x77D8), sp18, sp1C, 0);
    }
    M2C_FIELD(*sp20, s32 *, 0x77A8) = 1;
    Func_080e155c(8, 8);
    M2C_FIELD(*sp20, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    var_fp_1329 += 1;
    sp14 += 0x100;
    if (var_fp_1329 != 0x36) {
        goto loop_121;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2E);
    if (sp4C == 0) {
        Func_08009038(M2C_FIELD(*sp3C, s32 *, 0x77D8));
    }
    return Func_080cdbc0();
}
