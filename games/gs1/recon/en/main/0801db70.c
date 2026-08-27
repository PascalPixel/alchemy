#include "types.h"

/* Owner-local field access until this runtime workspace layout is recovered
 * elsewhere (shared with field_owner_54.c / runtime_owner_123.c, which both
 * reach the same 0x03001EA0 pointer). */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern void *Data_03001ea0;
extern u8 Data_03001f54;
extern s8 Data_080367dc[];

s32 Func_080770c0(s32);
void Func_0801d980(void);
void *Func_0801d9d4(void);
void Func_0801d9bc(void);
void *Func_08021620(s32, void *, s32, s32);
void Func_080216e8(s32, s32, s32);
void Func_080216b4(void *);
void Func_08016418(void *, s32);
s32 Func_080022fc(s32, s32);
void Func_080a1038(void *);
void Func_080b0030(void *, s32, s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);

s32 Func_0801db70(void) {
    s32 sp0;
    void *sp4;
    s32 sp8;
    s32 spC;
    s32 var_fp_21;
    void *sp10;
    s32 temp_r0_22;
    s32 var_r2_101;
    s32 var_r4_35;
    s32 var_r4_80;
    u16 var_r5_144;
    s32 var_r8_51;
    void **var_r7_88;
    void *temp_r5_90;
    void *temp_sl_29;
    s32 var_r6_86;

    spC = 1;
    var_fp_21 = 3;
    temp_r0_22 = Func_080770c0(0x17E);
    sp4 = (void *)0;
    Func_0801d980();
    temp_sl_29 = Data_03001ea0;
    sp10 = Func_0801d9d4();
    var_r4_35 = -0x18;
    if (Data_03001f54 != 0) {
        var_r4_35 = -0x10;
    }
    sp8 = (s32)Func_08021620(6, sp10, 0x28, var_r4_35);
    Func_080030f8(1);
    var_r8_51 = M2C_FIELD(temp_sl_29, u16 *, 0x574);
    if (temp_r0_22 != 0) {
        sp4 = (void *)2;
        var_fp_21 = 1;
    }
    if (Data_03001f54 != 0) {
        var_fp_21 += 3;
    }
loop_6:
    if (spC != 0) {
        spC = 0;
        var_r8_51 = Func_080022fc(var_r8_51 + var_fp_21, var_fp_21);
        M2C_FIELD(temp_sl_29, u16 *, 0x574) = var_r8_51;
        var_r4_80 = 0;
        if (var_fp_21 > 0) {
            var_r6_86 = (s32)sp4;
            var_r7_88 = (void **)((u8 *)temp_sl_29 + 0x610);
            do {
                temp_r5_90 = *var_r7_88;
                var_r7_88 += 1;
                M2C_FIELD(temp_r5_90, s8 *, 0xF) = 0xFB;
                sp0 = var_r4_80;
                Func_080a1038(temp_r5_90);
                var_r2_101 = 0;
                if (var_r4_80 != M2C_FIELD(temp_sl_29, u16 *, 0x574)) {
                    var_r2_101 = 1;
                }
                sp0 = var_r4_80;
                Func_080216e8(
                    Data_080367dc[var_r6_86],
                    M2C_FIELD(temp_r5_90, u8 *, 0xE),
                    var_r2_101);
                var_r4_80 += 1;
                var_r6_86 += 1;
            } while (var_r4_80 < var_fp_21);
        }
        Func_080b0030(
            (u8 *)temp_sl_29 + 0x5A4,
            M2C_FIELD(sp10, u16 *, 0xC) * 8,
            (((var_r8_51 * 3) + M2C_FIELD(sp10, u16 *, 0xE)) * 8) + 0x10,
            3);
    }
    Func_080216b4((void *)sp8);
    Func_080030f8(1);
    if (*(s32 *)0x03001C94 & 1) {
        var_r5_144 = var_r8_51;
        Func_080f9010(0x70);
    } else if (*(s32 *)0x03001C94 & 0xA) {
        var_r5_144 = (u16)-1;
        Func_080f9010(0x71);
    } else {
        if (*(s32 *)0x03001B04 & 0x40) {
            Func_080f9010(0x6F);
            var_r8_51 -= 1;
            spC = 1;
        } else if (*(s32 *)0x03001B04 & 0x80) {
            Func_080f9010(0x6F);
            var_r8_51 += 1;
            spC = 1;
        }
        goto loop_6;
    }
    Func_08016418(sp10, 2);
    Func_0801d9bc();
    Func_080030f8(1);
    if ((s32)var_r5_144 >= 0) {
        var_r5_144 = (u16)(var_r5_144 + (s32)sp4);
    }
    return (s32)var_r5_144;
}
