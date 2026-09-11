#include "types.h"
#include "scene.h"

/* Owner-local field access until this runtime workspace layout is recovered
 * elsewhere; other menu owners reach the same 0x03001EA0 pointer. */

extern void *gIw;
extern u8 gIw2;
extern s8 gRom[];

void *Menu_Run(void);

void *Menu_unk2_4(s32, void *, s32, s32);

s32 Menu_RunWorkspaceSelectionLoop(void)
{
    void *sp10;
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 i;
    s32 j;
    s32 var_fp_21;
    s32 temp_r0_22;
    s32 var_r2_101;
    s32 var_r4_35;
    s32 var_r5_144;
    s32 var_r8_51;
    s32 temp_e;
    s32 temp_c;
    s32 temp_e2;
    s8 *tbl;
    void *temp_r5_90;
    void *temp_sl_29;

    spC = 1;
    var_fp_21 = 3;
    temp_r0_22 = Menu_Check(0x17E);
    sp4 = 0;
    Menu_unk3_3();
    temp_sl_29 = gIw;
    sp10 = Menu_Run();
    var_r4_35 = -0x18;
    if (gIw2 != 0) {
        var_r4_35 = -0x10;
    }
    sp8 = (s32)Menu_unk2_4(6, sp10, 0x28, var_r4_35);
    Menu_Do(1);
    var_r8_51 = FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574);
    if (temp_r0_22 != 0) {
        var_fp_21 = 1;
        sp4 = 2;
    }
    if (gIw2 != 0) {
        var_fp_21 += 3;
    }
loop_6:
    if (spC != 0) {
        spC = 0;
        var_r8_51 = Menu_Apply(var_r8_51 + var_fp_21, var_fp_21);
        FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574) = var_r8_51;
        i = 0;
        if (i < var_fp_21) {
            j = sp4;
            tbl = gRom;
            do {
                temp_r5_90 = ((void **)((u8 *)temp_sl_29 + 0x610))[i];
                FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xF) = 0xFB;
                Menu_unk2_2(temp_r5_90);
                temp_e = FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xE);
                var_r2_101 = 0;
                if (i != FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574)) {
                    var_r2_101 = 1;
                }
                Menu_Place(*(s8 *)(j + (s32)tbl), temp_e, var_r2_101);
                i++;
                j++;
            } while (i < var_fp_21);
        }
        temp_e2 = FIELD_AT_OFFSET(sp10, u16 *, 0xE);
        temp_c = FIELD_AT_OFFSET(sp10, u16 *, 0xC) * 8;
        var_r4_35 = (((var_r8_51 * 3) + temp_e2) * 8) + 0x10;
        Menu_SetMode(
            (u8 *)temp_sl_29 + 0x5A4,
            temp_c,
            var_r4_35,
            3);
    }
    Menu_unk3_2((void *)sp8);
    Menu_Do(1);
    if (*(volatile s32 *)0x03001C94 & 1) {
        var_r5_144 = var_r8_51;
        Menu_unk4_2(0x70);
    } else if (*(volatile s32 *)0x03001C94 & 0xA) {
        var_r5_144 = -1;
        Menu_unk4_2(0x71);
    } else {
        if (*(volatile s32 *)0x03001B04 & 0x40) {
            Menu_unk4_2(0x6F);
            var_r8_51 -= 1;
            spC = 1;
        } else if (*(volatile s32 *)0x03001B04 & 0x80) {
            Menu_unk4_2(0x6F);
            var_r8_51 += 1;
            spC = 1;
        }
        goto loop_6;
    }
    Menu_Apply2(sp10, 2);
    Menu_unk4_3();
    Menu_Do(1);
    if (var_r5_144 >= 0) {
        var_r5_144 += sp4;
    }
    return var_r5_144;
}
