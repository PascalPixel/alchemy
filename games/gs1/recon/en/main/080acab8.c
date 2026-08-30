#include "shared-aggregates.h"

/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

/* Sh2 control register loads/stores */
#define M2C_LOAD_SR() (0)
#define M2C_LOAD_GBR() (0)
#define M2C_LOAD_VBR() (0)
#define M2C_STORE_SR(a)
#define M2C_STORE_GBR(a)
#define M2C_STORE_VBR(a)

#define M2C_CMP_STR(a, b) (0)

#endif

s32 Func_080acab8(M2C_UNK arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    void *sp30;
    void *sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    u16 sp4C[48];
    s16 temp_r0_483;
    s16 temp_r0_524;
    s32 temp_r0_29;
    s32 temp_r0_303;
    s32 temp_r0_719;
    s32 temp_r0_740;
    s32 temp_r1_447;
    s32 temp_r1_49;
    s32 temp_r1_90;
    s32 temp_r1_99;
    s32 temp_r2_399;
    s32 temp_r2_712;
    s32 temp_r3_30;
    s32 temp_r3_485;
    s32 temp_r4_27;
    s32 temp_r5_109;
    s32 temp_r5_152;
    s32 temp_r5_173;
    s32 temp_r5_230;
    s32 temp_r5_264;
    s32 temp_r5_280;
    s32 temp_r5_310;
    s32 temp_r5_498;
    s32 temp_r5_537;
    s32 temp_r5_574;
    s32 temp_r5_610;
    s32 temp_r5_684;
    s32 temp_r5_807;
    s32 temp_r5_89;
    s32 temp_r6_129;
    s32 temp_r6_307;
    s32 temp_r6_648;
    s32 temp_r6_852;
    s32 temp_r7_305;
    s32 var_fp_402;
    s32 var_fp_743;
    s32 var_fp_85;
    s32 var_r2_438;
    s32 var_r4_446;
    s32 var_r5_453;
    s32 var_r5_862;
    s32 var_r7_451;
    s32 var_r7_86;
    s32 var_r8_726;
    s8 temp_r0_735;
    s8 temp_r6_798;
    s8 var_r2_741;
    u16 *var_r7_753;
    u16 temp_r2_563;
    u16 temp_r2_599;
    u16 temp_r2_637;
    u16 temp_r2_767;
    u32 temp_r1_828;
    u8 *var_r6_452;
    u8 temp_r2_673;
    void *temp_r0_65;

    sp40 = arg1;
    sp3C = arg2;
    sp38 = arg3;
    sp34 = (void *)Func_08077008(arg3);
    temp_r4_27 = *(s32 *)0x03001F2C;
    temp_r0_29 = temp_r4_27 + 2;
    temp_r3_30 = arg4 + 0x254;
    sp2C = (s32)*(u8 *)(temp_r0_29 + temp_r3_30);
    sp28 = (s32)*(u8 *)(temp_r4_27 + temp_r3_30);
    sp24 = (s32)(u16)(0x8000 & *(u16 *)(temp_r4_27 + ((arg4 * 2) + 0x178)));
    temp_r1_49 = arg5 + 0x254;
    sp20 = (s32)*(u8 *)(temp_r0_29 + temp_r1_49);
    sp1C = (s32)*(u8 *)(temp_r4_27 + temp_r1_49);
    sp18 = (s32)(u16)(0x8000 & *(u16 *)(temp_r4_27 + ((arg5 * 2) + 0x178)));
    temp_r0_65 = Func_08004938(0x14C);
    sp30 = temp_r0_65;
    Func_080072f0(temp_r0_65, sp34, 0x14C, 0x03001388);
    if (arg7 != 0) {

    } else {
        if (arg6 == 3) {
            var_fp_85 = sp40 * 8;
            var_r7_86 = sp3C * 8;
            temp_r5_89 = var_fp_85 + 0x50;
            temp_r1_90 = var_r7_86 + 0x38;
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x34), 3, arg0, temp_r5_89, temp_r1_90);
            temp_r1_99 = var_r7_86 + 0x40;
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x36), 3, arg0, temp_r5_89, temp_r1_99);
            temp_r5_109 = temp_r5_89 - 0x20;
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x38), 3, arg0, temp_r5_109, temp_r1_90);
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x3A), 3, arg0, temp_r5_109, temp_r1_99);
            temp_r6_129 = var_fp_85 + 0x48;
            Func_08015090((void *)0x080AF290, arg0, temp_r6_129, temp_r1_90);
            Func_08015090((void *)0x080AF290, arg0, temp_r6_129, temp_r1_99);
        } else {
            var_r7_86 = sp3C * 8;
            var_fp_85 = sp40 * 8;
            temp_r5_152 = var_fp_85 + 0x30;
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x38), 3, arg0, temp_r5_152, var_r7_86 + 0x38);
            Func_080150b0(M2C_FIELD(sp34, s16 *, 0x3A), 3, arg0, temp_r5_152, var_r7_86 + 0x40);
        }
        temp_r5_173 = var_fp_85 + 0x30;
        Func_080150b0((s16) M2C_FIELD(sp34, u16 *, 0x3C), 3, arg0, temp_r5_173, var_r7_86 + 0x48);
        Func_080150b0((s16) M2C_FIELD(sp34, u16 *, 0x3E), 3, arg0, temp_r5_173, var_r7_86 + 0x50);
        Func_080150b0((s16) M2C_FIELD(sp34, u16 *, 0x40), 3, arg0, temp_r5_173, var_r7_86 + 0x58);
        Func_080150b0((s16) M2C_FIELD(sp34, u8 *, 0x42), 2, arg0, var_fp_85 + 0x38, var_r7_86 + 0x60);
    }
    switch (arg6) {                                 /* irregular */
    case 0:
        temp_r5_230 = 0x1F & sp1C;
        Func_080771a8(sp38, sp20, temp_r5_230);
        Func_080771b0(sp38, sp20, temp_r5_230);
        break;
    case 1:
        sp28 &= 0x1F;
        Func_080771b8(sp38, sp2C, sp28);
        break;
    case 2:
        if (sp24 != 0) {
            sp28 &= 0x1F;
            Func_080771b8(sp38, sp2C);
        }
        temp_r5_264 = 0x1F & sp1C;
        Func_080771a8(sp38, sp20, temp_r5_264);
        if (sp18 != 0) {
            Func_080771b0(sp38, sp20, temp_r5_264);
        }
        break;
    case 4:
        temp_r5_280 = 0x1F & sp1C;
        Func_080771a8(sp38, sp20, temp_r5_280);
        if (sp18 != 0) {
            Func_080771b0(sp38, sp20, temp_r5_280);
        }
        break;
    }
    Func_08077010(sp38);
    sp34 = Func_08077008(sp38);
    if (arg7 == 0) {
        temp_r0_303 = sp40 * 8;
        temp_r7_305 = sp3C * 8;
        temp_r6_307 = temp_r0_303 + 0x28;
        temp_r5_310 = temp_r7_305 + 0x10;
        Func_08015090(sp34, arg0, temp_r6_307, temp_r7_305);
        Func_08015090((void *)0x080AF28C, arg0, temp_r6_307, temp_r5_310);
        Func_080150b0((s16) M2C_FIELD(sp34, u8 *, 0xF), 2, arg0, temp_r0_303 + 0x58, temp_r5_310);
        Func_08015080(0x8AE, arg0, temp_r0_303, temp_r7_305 + 0x38);
        Func_08015080(0x8AF, arg0, temp_r0_303, temp_r7_305 + 0x40);
        Func_08015080(0x8B0, arg0, temp_r0_303, temp_r7_305 + 0x48);
        Func_08015080(0x8B1, arg0, temp_r0_303, temp_r7_305 + 0x50);
        Func_08015080(0x8B2, arg0, temp_r0_303, temp_r7_305 + 0x58);
        Func_08015080(0x8B3, arg0, temp_r0_303, temp_r7_305 + 0x60);
        Func_08015080(M2C_FIELD(sp30, u8 *, 0x129) + 0x741, arg0, temp_r0_303, temp_r7_305 + 0x20);
    }
    if (arg7 != 0) {

    } else {
        if (M2C_FIELD(sp30, u8 *, 0x129) != M2C_FIELD(sp34, u8 *, 0x129)) {
            temp_r2_399 = sp40 * 8;
            var_fp_402 = temp_r2_399;
            Func_08015080(M2C_FIELD(sp34, u8 *, 0x129) + 0x741, arg0, temp_r2_399, (sp3C * 8) + 0x30);
            Func_08015280(arg0, 0xF296, sp40 + 2, 5, arg7);
        } else {
            var_fp_402 = sp40 * 8;
        }
        var_r2_438 = sp40;
        if (M2C_FIELD(sp30, u8 *, 0x129) != M2C_FIELD(sp34, u8 *, 0x129)) {
            var_r2_438 += 5;
        }
        var_r4_446 = 0;
        temp_r1_447 = sp3C + 5;
        var_r7_451 = var_r2_438 + 1;
        var_r6_452 = sp34 + 0x11C;
        var_r5_453 = var_r2_438;
        do {
            sp8 = var_r4_446;
            Func_08015280(arg0, var_r4_446 + 0x5001, var_r5_453, temp_r1_447, 0);
            Func_08015280(arg0, *var_r6_452 + 0xF030, var_r7_451, temp_r1_447, 0);
            var_r4_446 += 1;
            var_r6_452 += 1;
            var_r7_451 += 2;
            var_r5_453 += 2;
        } while (var_r4_446 <= 3);
        temp_r0_483 = M2C_FIELD(sp34, s16 *, 0x38);
        temp_r3_485 = var_fp_402 + 0x46;
        if (temp_r0_483 != M2C_FIELD(sp30, s16 *, 0x38)) {
            temp_r5_498 = (sp3C * 8) + 0x38;
            Func_080150b0(temp_r0_483, 4, arg0, var_fp_402 + 0x48, temp_r5_498);
            if ((s32) M2C_FIELD(sp34, s16 *, 0x38) > (s32) M2C_FIELD(sp30, s16 *, 0x38)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_498, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_498, 1);
            }
        }
        temp_r0_524 = M2C_FIELD(sp34, s16 *, 0x3A);
        if (temp_r0_524 != M2C_FIELD(sp30, s16 *, 0x3A)) {
            temp_r5_537 = (sp3C * 8) + 0x40;
            Func_080150b0(temp_r0_524, 4, arg0, var_fp_402 + 0x48, temp_r5_537);
            if ((s32) M2C_FIELD(sp34, s16 *, 0x3A) > (s32) M2C_FIELD(sp30, s16 *, 0x3A)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_537, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_537, 1);
            }
        }
        temp_r2_563 = M2C_FIELD(sp34, u16 *, 0x3C);
        if (temp_r2_563 != M2C_FIELD(sp30, u16 *, 0x3C)) {
            temp_r5_574 = (sp3C * 8) + 0x48;
            Func_080150b0((s16) temp_r2_563, 4, arg0, var_fp_402 + 0x48, temp_r5_574);
            if ((u32) M2C_FIELD(sp34, u16 *, 0x3C) > (u32) M2C_FIELD(sp30, u16 *, 0x3C)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_574, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_574, 1);
            }
        }
        temp_r2_599 = M2C_FIELD(sp34, u16 *, 0x3E);
        if (temp_r2_599 != M2C_FIELD(sp30, u16 *, 0x3E)) {
            temp_r5_610 = (sp3C * 8) + 0x50;
            Func_080150b0((s16) temp_r2_599, 4, arg0, var_fp_402 + 0x48, temp_r5_610);
            if ((u32) M2C_FIELD(sp34, u16 *, 0x3E) > (u32) M2C_FIELD(sp30, u16 *, 0x3E)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_610, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_610, 1);
            }
        }
        temp_r2_637 = M2C_FIELD(sp34, u16 *, 0x40);
        if (temp_r2_637 != M2C_FIELD(sp30, u16 *, 0x40)) {
            temp_r6_648 = (sp3C * 8) + 0x58;
            Func_080150b0((s16) temp_r2_637, 4, arg0, var_fp_402 + 0x48, temp_r6_648);
            if ((u32) M2C_FIELD(sp34, u16 *, 0x40) > (u32) M2C_FIELD(sp30, u16 *, 0x40)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r6_648, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r6_648, 1);
            }
        }
        temp_r2_673 = M2C_FIELD(sp34, u8 *, 0x42);
        if (temp_r2_673 != M2C_FIELD(sp30, u8 *, 0x42)) {
            temp_r5_684 = (sp3C * 8) + 0x60;
            Func_080150b0((s16) temp_r2_673, 2, arg0, var_fp_402 + 0x58, temp_r5_684);
            if ((u32) M2C_FIELD(sp34, u8 *, 0x42) > (u32) M2C_FIELD(sp30, u8 *, 0x42)) {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_684, 0);
            } else {
                Func_080ae9f0(arg0, temp_r3_485, temp_r5_684, 1);
            }
        }
    }
    if (arg7 <= 0) {

    } else {
        temp_r2_712 = 3 ^ arg6;
        temp_r0_719 = 6 - ((u32) ((0 - temp_r2_712) | temp_r2_712) >> 0x1F);
        var_r8_726 = temp_r0_719 * (arg7 - 1);
        temp_r0_735 = Func_080aae14(sp30 + 0x58, sp34 + 0x58, (u16 (*)[]) &sp4C[0], &sp48, &sp44);
        sp10 = temp_r0_735 << 0x18;
        temp_r0_740 = sp40 * 8;
        var_r2_741 = 0;
        var_fp_743 = temp_r0_740;
        if (var_r8_726 < (s32) temp_r0_735) {
            if (temp_r0_719 > 0) {
                sp14 = temp_r0_740;
                spC = 0;
                var_r7_753 = &(&sp4C[0])[var_r8_726];
loop_62:
                Func_080ae958(arg0, var_fp_743, ((sp3C + ((s32) (var_r2_741 << 0x18) >> 0x17)) * 8) + 4, *var_r7_753 & 0x3FFF);
                temp_r2_767 = *var_r7_753;
                if (0x8000 & temp_r2_767) {
                    Func_080150b8(4);
                } else if (0x4000 & temp_r2_767) {
                    Func_080150b8(2);
                } else {
                    Func_080150b8(0xF);
                }
                temp_r6_798 = var_r2_741;
                temp_r5_807 = ((sp3C + (temp_r6_798 * 2)) * 8) + 8;
                Func_08015080((0x3FFF & *var_r7_753) + 0x333, arg0, sp14 + 0x10, temp_r5_807);
                Func_080150a8(M2C_FIELD(Func_08077080(*var_r7_753), u8 *, 9), 2, arg0, sp14 + 0x58, temp_r5_807);
                temp_r1_828 = (u32) (spC + 0x01000000) >> 0x18;
                var_r8_726 += 1;
                var_r2_741 = (s8) (u8) (temp_r6_798 + 1);
                var_r7_753 += 2;
                if (var_r8_726 < (s32) (sp10 >> 0x18)) {
                    spC = temp_r1_828 << 0x18;
                    if ((s32) (s8) temp_r1_828 >= temp_r0_719) {

                    } else {
                        goto loop_62;
                    }
                }
            } else {
                var_fp_743 = sp40 * 8;
            }
        }
        Func_080150b8(0xF);
        temp_r6_852 = sp3C * 8;
        Func_08015080(0xAED, arg0, var_fp_743 + 0x58, temp_r6_852);
        if (arg6 != 3) {
            var_r5_862 = 0;
            if (sp48 != 0) {
                Func_080150b8(4);
                Func_08015080(0xBA2, arg0, var_fp_743, temp_r6_852 + 0x58);
                var_r5_862 = 1;
            }
            if (sp44 != 0) {
                Func_080150b8(2);
                Func_08015080(0xBA3, arg0, var_fp_743, ((sp3C + var_r5_862) * 8) + 0x58);
                var_r5_862 += 1;
            }
            if (var_r5_862 == 0) {
                Func_08015080(0xBA8, arg0, var_fp_743, temp_r6_852 + 0x58);
            }
            Func_080150b8(0xF);
            Func_08015070(arg0, 0, 0xB, 0xD, 0xB);
        }
        M2C_FIELD(absolute_03001e8c.field_0000, s8 *, 0xEA3) = 1;
    }
    if (arg7 == 0) {
        Func_080150d8(sp38, 0, arg8, arg0, arg7, arg7);
    }
    Func_080072f0(sp34, sp30, 0x14C, 0x03001388);
    Func_08002df0(sp30);
    return 1;
}
