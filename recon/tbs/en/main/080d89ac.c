/* Draft, not exact (2026-09-24): candidate=2024 reference=2024 differing_halfwords=947. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00007828;
extern u8 Value_00000100;
extern u8 Value_00001000;
extern u8 Value_00000200;
extern u8 Value_00007780;
extern u8 Value_00007784;
extern u8 Value_00000480;
extern u8 Value_00000139;
extern u8 Value_0000013a;
extern u8 Value_0000027a;
extern u8 Value_00000700;
extern u8 Value_00007824;
extern u8 Value_00000800;
#include "BATTLE_EFX.H"

/* A record of the effect scratch buffer at gMapCellBuffer, with the offsets
   the battle effects touch. */
struct EffectScratch {
    u8 unknown_0000[0x4];
    u32 field_0004;
    u32 field_0008;
    u32 field_000c;
    u32 field_0010;
    u32 field_0014;
    u32 field_0018;
    u8 unknown_001c[0x4];
    u32 field_0020;
    u8 unknown_0024[0x10];
    u32 field_0034;
    u8 unknown_0038[0x2];
    u8 field_003a;
};

/* The camera state gCameraWork points at. */
struct CameraState {
    s16 field_0000;
    u8 unknown_0002[0xa];
    u32 field_000c;
    u32 field_0010;
    u32 field_0014;
    u32 field_0018;
    u32 field_001c;
    u32 field_0020;
    u8 unknown_0024[0x10];
    u16 field_0034;
    u16 field_0036;
};

extern u8 gMapCellBuffer[];
extern struct CameraState *gCameraWork;
extern u8 gBattleFxWork[];

/* Only the m2c spellings this draft actually uses. */
typedef s32 M2C_UNK;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void **GetBattleObjectSlotFar(s32 actor_id);
void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);

void BattleEffectA(s32 arg0, u32 arg1) {
    void **heap_base;
    void **heap_cursor;
    u32 *sp0;
    s32 *sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    u32 sp18;
    u32 **sp1C;
    u8 *sp20;
    struct CameraState *sp24;
    DrawRectangleFn callbacks[2];
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    u32 sp38;
    s32 sp3C;
    s32 sp40;
    u32 sp44;
    void *sp48;
    s32 sp54;
    u32 sp58;
    M2C_UNK sp68;
    s32 sp74;
    s32 temp_r0_887;
    s32 temp_r1_44;
    s32 temp_r1_796;
    s32 temp_r1_951;
    s32 temp_r2_792;
    s32 temp_r3_39;
    s32 temp_r3_421;
    s32 temp_r3_656;
    s32 temp_r3_786;
    s32 temp_r3_790;
    s32 temp_r3_794;
    s32 temp_r3_90;
    s32 temp_r5_301;
    s32 temp_r5_304;
    s32 temp_r5_708;
    s32 temp_r5_913;
    s32 temp_r5_919;
    s32 temp_r5_925;
    s32 temp_r7_802;
    s32 temp_r8_489;
    s32 temp_r9_580;
    s32 var_fp_449;
    s32 var_r0_141;
    s32 var_r0_169;
    s32 var_r2_764;
    s32 var_r2_839;
    s32 var_r3_244;
    s32 var_r3_814;
    s32 var_r3_825;
    s32 var_r5_497;
    s32 var_r5_588;
    s32 var_r8_210;
    s32 var_r8_252;
    s32 var_r8_293;
    s32 var_r8_344;
    s32 var_r8_378;
    struct EffectScratch *var_r5_209;
    struct EffectScratch *var_r5_251;
    struct EffectScratch *var_r5_343;
    struct EffectScratch *var_r5_377;
    struct EffectScratch *var_r7_292;
    u16 temp_r6_298;
    u32 **temp_r3_198;
    u32 temp_r4_875;
    u32 var_r3_770;
    u32 var_r4_848;
    u32 var_r8_743;
    u8 *var_r6_753;
    u8 temp_r0_226;
    u8 temp_r0_873;
    void **temp_r5_27;
    void **var_r3_373;
    void *temp_r5_33;
    void *temp_r5_657;
    void *temp_r6_599;
    void *temp_r7_508;

    heap_base = (void **)gBattleFxWork;
    heap_cursor = heap_base;
    sp48 = *heap_cursor++;
    sp44 = *heap_cursor;
    sp38 = heap_base[2];
    temp_r5_27 = M2C_FIELD(gBattleFxWork, void **, 0) + 0x7828;
    sp30 = 0;
    *temp_r5_27 = (void *)arg0;
    BattleFx_BeginCanvasLayer(0);
    temp_r5_33 = *temp_r5_27;
    if (M2C_FIELD(temp_r5_33, s32 *, 0x1C) == 1) {
        temp_r3_39 = 6 ^ arg1;
        temp_r1_44 = 2 - ((u32) ((0 - temp_r3_39) | temp_r3_39) >> 0x1F);
        if ((arg1 == 6) || (arg1 == 0)) {
            BattleFx_PrepareCanvasEffect(arg0, temp_r1_44,
                M2C_FIELD(temp_r5_33, s32 *, 4), 0, &sp58, &sp54);
        } else {
            BattleFx_PrepareCanvasEffect(arg0, temp_r1_44,
                M2C_FIELD(temp_r5_33, s32 *, 4), 1, &sp58, &sp54);
        }
        M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18) = 0;
    }
    if (arg1 == 0) {
        EffectPosition_ApplyStepAndYOffset(M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s16 *, 0x24), &sp74);
        temp_r3_90 = 0x40 - sp74;
        sp30 = temp_r3_90;
        *(u32 *)0x04000028 = temp_r3_90 << 8;
        *(u16 *)((u8 *)0x04000020) = 0x100;
        sp2C = 0;
    } else {
        sp2C = 1;
    }
    Resource_LoadAndDecompress(0x73, sp38, 0, 0);
    Resource_LoadAndDecompress(0xBA, (u32) sp48, 0, 0);
    if ((arg1 <= 1U) || (arg1 == 3) || (arg1 == 4) || (arg1 == 5)) {
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18) == 0) {
            var_r0_141 = 0xB3;
        } else {
            var_r0_141 = 0xB9;
        }
    } else if (arg1 == 6) {
        var_r0_141 = 0x8D;
    } else {
        var_r0_141 = 0xC0;
    }
    _call_via_r3(0x05000000, Resource_GetTableEntry(var_r0_141), 0x80, 0x03001388);
    if (sp2C == 0) {
        if (arg1 == 6) {
            var_r0_169 = 0x8D;
        } else {
            var_r0_169 = 0x91;
        }
    } else if (arg1 == 6) {
        var_r0_169 = 0x8E;
    } else {
        var_r0_169 = 0x92;
    }
    Resource_DecodeType01(Resource_GetTableEntry(var_r0_169) + 0x80, sp48 + 0x1000);
    BattleFx_FetchRectangleBlitters(M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 4),
        callbacks);
    switch (arg1) {                                 /* switch 1; irregular */
    case 0:                                         /* switch 1 */
    case 6:                                         /* switch 1 */
        var_r5_209 = (struct EffectScratch *)gMapCellBuffer;
        var_r8_210 = 0;
        do {
            M2C_FIELD(var_r5_209, s32 *, 0) = (s32) ((Random16() - 0x7F) << 0xF);
            var_r5_209->field_0004 = ((0x7F & Random16()) + 0x40) << 0xF;
            temp_r0_226 = Random16();
            var_r5_209->field_0018 = 0;
            var_r8_210 += 1;
            var_r5_209->field_0008 = (temp_r0_226 - 0x7F) << 0xF;
            var_r5_209 += 0x1C;
        } while (var_r8_210 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 1:                                         /* switch 1 */
        var_r5_251 = (struct EffectScratch *)gMapCellBuffer;
        var_r8_252 = 0;
        do {
            M2C_FIELD(var_r5_251, s32 *, 0) = (s32) ((Random16() - 0x7F) << 0xF);
            var_r5_251->field_0004 = (Random16() - 0x7F) << 0xF;
            var_r5_251->field_0008 = (Random16() - 0x7F) << 0xF;
            var_r8_252 += 1;
            var_r5_251->field_0018 = 0;
            var_r5_251 += 0x1C;
        } while (var_r8_252 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 2:                                         /* switch 1 */
        var_r7_292 = (struct EffectScratch *)gMapCellBuffer;
        var_r8_293 = 0;
        do {
            temp_r6_298 = (u16) Random16();
            temp_r5_301 = 0x3F & Random16();
            temp_r5_304 = temp_r5_301 + 0x20;
            M2C_FIELD(var_r7_292, s32 *, 0) = (s32) (temp_r5_304 * Trig_Sin(temp_r6_298));
            var_r7_292->field_0004 = 0xFFCE0000;
            var_r7_292->field_0008 = temp_r5_304 * Trig_Cos(temp_r6_298);
            var_r7_292->field_0010 = ((0x1F & Random16()) + 0x20) << 0xD;
            var_r7_292->field_0018 = 0;
            var_r8_293 += 1;
            var_r7_292 += 0x1C;
        } while (var_r8_293 != 0x200);
        var_r3_244 = (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) * 8) + 0x58;
        break;
    case 3:                                         /* switch 1 */
        var_r5_343 = (struct EffectScratch *)gMapCellBuffer;
        var_r8_344 = 0;
        do {
            M2C_FIELD(var_r5_343, s32 *, 0) = (s32) ((Random16() - 0x7F) << 0xF);
            var_r5_343->field_0004 = (Random16() - 0x7F) << 0xE;
            var_r8_344 += 1;
            var_r5_343->field_0008 = (Random16() - 0x7F) << 0xF;
            var_r5_343->field_0018 = 0;
            var_r5_343 += 0x1C;
        } while (var_r8_344 != 0x200);
        var_r3_373 = sp48 + 0x7828;
block_47:
        var_r3_244 = (M2C_FIELD(*var_r3_373, s32 *, 0x14) * 8) + 0x48;
        break;
    default:                                        /* switch 1 */
        var_r5_377 = (struct EffectScratch *)gMapCellBuffer;
        var_r8_378 = 0;
        do {
            M2C_FIELD(var_r5_377, s32 *, 0) = (s32) ((Random16() - 0x7F) << 0xF);
            var_r5_377->field_0004 = (Random16() - 0x7F) << 0xF;
            var_r5_377->field_0008 = (Random16() - 0x7F) << 0xF;
            var_r5_377->field_0018 = 0;
            var_r8_378 += 1;
            var_r5_377 += 0x1C;
        } while (var_r8_378 != 0x200);
        var_r3_373 = sp48 + 0x7828;
        goto block_47;
    }
    sp40 = var_r3_244;
    sp34 = 0x40;
    temp_r3_421 = M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x18);
    if (temp_r3_421 == 0) {
        sp34 = 0x20;
    } else if (temp_r3_421 == 2) {
        sp34 = 0x80;
    }
    M2C_FIELD(sp48, s32 *, 0x7780) = 2;
    M2C_FIELD(sp48, s32 *, 0x7784) = 0x4B;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    var_fp_449 = 0;
    if (sp40 == 0) {

    } else {
        spC = 0;
loop_55:
        sp24 = gCameraWork;
        if (var_fp_449 == 0x28) {
            BattleEventRuntime_BeginPhaseFar(0);
        }
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x1C) != 1) {

        } else if (sp2C == 0) {
            temp_r8_489 = (((s32) (Trig_Sin((u16) spC) * 0x14) >> 0x10) + sp58 + sp30) - 0x14;
            var_r5_497 = (((s32) (Trig_Cos((u16) spC) * 4) >> 0x10) + sp54) - 0x18;
            if (var_fp_449 > 0x20) {
                var_r5_497 = (var_r5_497 - (var_fp_449 * 2)) + 0x40;
            }
            temp_r7_508 = sp48 + 0x1000;
            sp0 = (u32 *)0x28;
            sp4 = (s32 *)0x28;
            callbacks[0](sp44, temp_r7_508, temp_r8_489, var_r5_497,
                0x28, 0x28);
            if (var_fp_449 <= 3) {
                sp0 = (u32 *)0x28;
                sp4 = (s32 *)0x28;
                callbacks[1](sp44, temp_r7_508, temp_r8_489, var_r5_497,
                    0x28, 0x28);
            }
        } else {
            temp_r9_580 = (((s32) (Trig_Sin((u16) spC) * 0xA) >> 0x10) + ((s32) (sp58 + (sp58 >> 0x1F)) >> 1)) - 0xA;
            var_r5_588 = (((s32) (Trig_Cos((u16) spC) * 4) >> 0x10) + sp54) - 0x18;
            if (var_fp_449 > 0x20) {
                var_r5_588 = (var_r5_588 - (var_fp_449 * 2)) + 0x40;
            }
            temp_r6_599 = sp48 + 0x1000;
            sp0 = (u32 *)0x14;
            sp4 = (s32 *)0x28;
            callbacks[0](sp44, temp_r6_599, temp_r9_580, var_r5_588,
                0x14, 0x28);
            if (var_fp_449 <= 3) {
                sp0 = (u32 *)0x14;
                sp4 = (s32 *)0x28;
                callbacks[1](sp44, temp_r6_599, temp_r9_580, var_r5_588,
                    0x14, 0x28);
            }
        }
        sp3C = 0;
        if (M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14) == 0) {

        } else {
            sp20 = &sp24->unknown_0002[0xA];
            sp1C = &sp0 + 0x5C;
            sp14 = var_fp_449 << 9;
            sp10 = 0x24;
            sp8 = 0;
loop_72:
            temp_r3_656 = sp3C * 8;
            temp_r5_657 = *GetBattleObjectSlotFar(*(s16 *)((u8 *)M2C_FIELD(sp48, void **, 0x7828) + sp10));
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(sp24, sp20);
            M2C_FIELD(sp1C, u32 **, 0) = M2C_FIELD(temp_r5_657, u32 **, 8);
            M2C_FIELD(sp1C, s32 *, 4) = 0x280000;
            M2C_FIELD(sp1C, s32 *, 8) = (s32) M2C_FIELD(temp_r5_657, s32 *, 0x10);
            SceneTransform_ApplyPosition(sp1C);
            if (var_fp_449 == (temp_r3_656 + 0x14)) {
                Audio_PlayCue(0x7E);
            }
            if (var_fp_449 == (temp_r3_656 + 0x24)) {
                sp0 = (u32 *)0x1C;
                ObjectGroup_UpdateMembers(*(s16 *)((u8 *)M2C_FIELD(sp48, void **, 0x7828) + sp10), 7, -1, sp3C, 0x1C);
            }
            if (var_fp_449 <= temp_r3_656) {

            } else {
                switch (arg1) {                     /* switch 2; irregular */
                case 1:                             /* switch 2 */
                    temp_r5_708 = var_fp_449 << 9;
                    SceneTransform_ApplyPitch(temp_r5_708);
                    SceneTransform_ApplyRoll(temp_r5_708);
                    break;
                case 2:                             /* switch 2 */
                    SceneTransform_ApplyYaw((var_fp_449 - (sp3C * 0x28)) << 9);
                    break;
                case 0:                             /* switch 2 */
                case 6:                             /* switch 2 */
                case 3:                             /* switch 2 */
                    SceneTransform_ApplyYaw(sp14);
                    break;
                default:                            /* switch 2 */
                    SceneTransform_ApplyYaw(sp14);
                    SceneTransform_ApplyPitch(sp14);
                    break;
                }
                var_r8_743 = 0;
                if (sp34 == 0) {

                } else {
                    sp18 = arg1 - 3;
                    var_r6_753 = &gMapCellBuffer[sp8];
loop_90:
                    if (sp18 <= 2U) {
                        var_r2_764 = ((s32) ((var_r8_743 >> 0x1F) + var_r8_743) >> 1) + temp_r3_656 + 0x20;
                    } else {
                        var_r2_764 = 0x10000;
                    }
                    var_r3_770 = var_r8_743;
                    if ((s32) var_r3_770 < 0) {
                        var_r3_770 += 3;
                    }
                    if (var_fp_449 <= (s32) (((s32) var_r3_770 >> 2) + temp_r3_656)) {

                    } else if (var_fp_449 >= var_r2_764) {

                    } else {
                        temp_r3_786 = (s32) M2C_FIELD(var_r6_753, s32 *, 0) >> 8;
                        temp_r3_790 = (s32) M2C_FIELD(var_r6_753, s32 *, 4) >> 8;
                        temp_r2_792 = temp_r3_790 * temp_r3_790;
                        temp_r3_794 = (s32) M2C_FIELD(var_r6_753, s32 *, 8) >> 8;
                        temp_r1_796 = temp_r3_794 * temp_r3_794;
                        temp_r7_802 = _call_via_r3((temp_r3_786 * temp_r3_786) + temp_r2_792 + temp_r1_796, temp_r1_796, temp_r2_792, 0x030001D8) >> 9;
                        if (temp_r7_802 != 0) {
                            EffectPosition_ApplyBaseAndYOffset(var_r6_753, &sp68);
                            if (arg1 == 0) {
                                var_r3_814 = M2C_FIELD(&sp68, s32 *, 0) + sp30;
                            } else {
                                var_r3_814 = (s32) M2C_FIELD(&sp68, s32 *, 0) >> 1;
                            }
                            M2C_FIELD(&sp68, s32 *, 0) = var_r3_814;
                            M2C_FIELD(&sp68, s32 *, 4) = (s32) (M2C_FIELD(&sp68, s32 *, 4) + 0x10);
                            var_r3_825 = M2C_FIELD(&sp68, s32 *, 8);
                            if (var_r3_825 <= 0x139) {
                                var_r3_825 = 0x13A;
                                M2C_FIELD(&sp68, s32 *, 8) = 0x13A;
                            }
                            if (var_r3_825 > (s32)&Value_0000027a) {
                                M2C_FIELD(&sp68, s32 *, 8) = 0x27A;
                                var_r3_825 = (s32)&Value_0000027a;
                            }
                            var_r2_839 = var_r3_825 + 0xFFFFFEC6;
                            if (var_r2_839 < 0) {
                                var_r2_839 = var_r3_825 - 0xBB;
                            }
                            var_r4_848 = 3 - (var_r2_839 >> 7);
                            switch (arg1) {         /* switch 3; irregular */
                            case 0:                 /* switch 3 */
                                var_r4_848 = __modsi3((var_r8_743 * 4) + var_fp_449, 9);
                                /* fallthrough */
                            case 3:                 /* switch 3 */
                            case 4:                 /* switch 3 */
                            case 5:                 /* switch 3 */
                                temp_r0_873 = *(u8 *)(0x080EDE96 + var_r4_848);
                                temp_r4_875 = temp_r0_873 >> 1;
                                sp0 = (u32 *) temp_r0_873;
                                sp4 = (s32 *) temp_r0_873;
                                callbacks[1](sp44, sp48 + *(u16 *)(0x080EDE84 + (var_r4_848 * 2)), M2C_FIELD(&sp68, s32 *, 0) - temp_r4_875, M2C_FIELD(&sp68, s32 *, 4) - temp_r4_875, temp_r0_873, temp_r0_873);
                                break;
                            default:                /* switch 3 */
                                temp_r0_887 = var_r4_848 * 2;
                                sp4 = (s32 *)temp_r0_887;
                                sp0 = (u32 *)var_r4_848;
                                callbacks[1](sp44, sp38 + *(u16 *)(0x080EDE48 + (s32) (temp_r0_887 - 2)), M2C_FIELD(&sp68, s32 *, 0) - ((s32) (var_r4_848 + (var_r4_848 >> 0x1F)) >> 1), M2C_FIELD(&sp68, s32 *, 4) - var_r4_848, var_r4_848, temp_r0_887);
                                break;
                            }
                            if ((arg1 <= 2U) || (arg1 == 6)) {
                                temp_r5_913 = M2C_FIELD(var_r6_753, s32 *, 0);
                                M2C_FIELD(var_r6_753, s32 *, 0) = (s32) (temp_r5_913 - __divsi3(temp_r5_913, temp_r7_802));
                                temp_r5_919 = M2C_FIELD(var_r6_753, s32 *, 4);
                                M2C_FIELD(var_r6_753, s32 *, 4) = (s32) (temp_r5_919 - __divsi3(temp_r5_919, temp_r7_802));
                                temp_r5_925 = M2C_FIELD(var_r6_753, s32 *, 8);
                                M2C_FIELD(var_r6_753, s32 *, 8) = (s32) (temp_r5_925 - __divsi3(temp_r5_925, temp_r7_802));
                            }
                        }
                    }
                    var_r8_743 += 1;
                    var_r6_753 += 0x1C;
                    if (var_r8_743 != sp34) {
                        goto loop_90;
                    }
                }
            }
            sp14 += 0xFFFFF000;
            temp_r1_951 = sp3C + 1;
            sp10 += 2;
            sp8 += 0x700;
            sp3C = temp_r1_951;
            if (temp_r1_951 != M2C_FIELD(M2C_FIELD(sp48, void **, 0x7828), s32 *, 0x14)) {
                goto loop_72;
            }
        }
        ObjectGroup_TickMemberTimers();
        M2C_FIELD(sp48, s32 *, 0x7824) = 1;
        WaitFrames(1);
        var_fp_449 += 1;
        spC += 0x800;
        if (var_fp_449 != sp40) {
            goto loop_55;
        }
    }
    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(0x2F);
    Runtime_ReleaseHeapBlock(0x2E);
    BattleFx_EndCanvasLayer();
}
