/* Draft, not exact (2026-09-24): candidate=1036 reference=1036 differing_halfwords=418. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling.
   2026-09-29 slice 4: alchemy permute cannot parse this draft, because
   M2C_FIELD takes a type as a macro argument. Preprocessed, it scores 6,142
   with 18 symbols the linked build does not define, too far for a 10-minute
   search, so none was run. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
extern u8 Value_00007828;
extern u8 Value_00000100;
extern u8 Value_00001000;
extern u8 Value_00007780;
extern u8 Value_00007784;
extern u8 Value_00000480;
extern u8 Value_0000aaab;
extern u8 Value_00005555;
extern u8 Value_00000600;
extern u8 Value_00007080;
extern u8 Value_00007824;
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef s32 (*WordCopy)(void *destination, const void *source, s32 size);
void *Resource_GetTableEntry(s32 resource);
u32 Resource_DecodeType01(const void *source, void *destination);
void **GetBattleObjectSlotFar(s32 member_id);
void EffectPosition_ApplyStepAndYOffset(s32 member_id, struct EffectPosition *result);

typedef void (*BattleEffectDrawFn)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

extern void *gWorkSlot[];
extern const u16 BattleFx6_FlareCells[];

struct BattleEffectWorkGlobals {
    void *work;
    void *render_context;
    void *resource_context;
};

struct BattleEffectVectorWork {
    s32 modulation[3];
    s32 projected[3];
    s32 position[3];
    s32 output[3];
};

s32 Unnamed_080d0ad4(s32 actor) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    void *sp20;
    s32 *sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    BattleEffectDrawFn sp34;
    BattleEffectDrawFn sp38;
    s32 sp3C;
    void *sp40;
    void *sp44;
    struct BattleEffectVectorWork vector_work;
    u8 *palette;
    s32 temp_r1_252;
    s32 temp_r3_103;
    s32 temp_r3_257;
    s32 temp_r3_376;
    s32 temp_r5_357;
    s32 temp_r5_429;
    s32 temp_r6_346;
    s32 temp_r6_359;
    s32 temp_r7_314;
    s32 temp_r7_342;
    s32 var_fp_191;
    s32 var_r2_331;
    s32 var_r4_339;
    s32 var_r7_258;
    s32 var_r7_304;
    s32 var_r8_341;
    void *temp_r6_25;
    void **temp_r5_28;
    void *temp_r2_18;
    void *temp_r3_135;
    void *temp_r6_203;
    void *temp_r9_332;
    void *var_r5_253;
    struct BattleEffectWorkGlobals *globals;

    globals = (struct BattleEffectWorkGlobals *)0x03001EEC;
    temp_r2_18 = globals->work;
    sp44 = temp_r2_18;
    sp40 = globals->render_context;
    temp_r6_25 = globals->resource_context;
    temp_r5_28 = temp_r2_18 + 0x7828;
    sp2C = *(s32 *)((u8 *)globals - 108);
    M2C_FIELD(temp_r2_18, s32 *, 0x7828) = actor;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000020 = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    ((WordCopy)0x03001388)((void *)0x05000000, palette, 0x80);
    Resource_DecodeType01(palette + 0x80, sp44);
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
        temp_r6_25);
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD),
        temp_r2_18 + 0x1000);
    M2C_FIELD(sp44, s32 *, 0x7780) = 3;
    M2C_FIELD(sp44, s32 *, 0x7784) = 0x04040404;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    sp24 = vector_work.output;
    EffectPosition_ApplyStepAndYOffset(
        M2C_FIELD(*temp_r5_28, s16 *, 0x24),
        (struct EffectPosition *)vector_work.output);
    temp_r3_103 = 0x40 - *sp24;
    sp28 = temp_r3_103;
    *(s32 *)0x04000028 = temp_r3_103 << 8;
    Audio_PlayCue(0x8E);
    sp3C = 0;
    if ((M2C_FIELD(*temp_r5_28, s32 *, 0x14) * 0x14) == -0x48) {

    } else {
loop_3:
        if (sp3C == 0x40) {
            BattleEventRuntime_BeginPhaseFar(0);
        }
        if (sp3C == 0x2E) {
            temp_r3_135 = M2C_FIELD(sp44, void **, 0x7828);
            BattleMotion_ApproachTargetFar(M2C_FIELD(temp_r3_135, s32 *, 8), M2C_FIELD(temp_r3_135, s16 *, 0x24), 0x10, 0);
        }
        Graphics_UpdatePhasePalette(sp3C, 0xAAAB, 0x5555, 0);
        BattleEffect_LoadWork(0x2E, 7, 7, 3, 2);
        sp34 = (BattleEffectDrawFn)gWorkSlot[46];
        BattleEffect_LoadWork(0x2F, 7, 7, 7, 2);
        sp38 = (BattleEffectDrawFn)gWorkSlot[47];
        if ((sp3C > 0x10) && !(0xF & sp3C)) {
            M2C_FIELD(sp44, s32 *, 0x7784) = (s32) (M2C_FIELD(sp44, s32 *, 0x7784) + 0x01010101);
        }
        sp30 = 0;
        sp10 = 0;
        spC = sp3C * 0x600;
        var_fp_191 = sp3C;
loop_11:
        temp_r6_203 = *GetBattleObjectSlotFar(M2C_FIELD(
            M2C_FIELD(sp44, void **, 0x7828), s16 *,
            (sp30 * 2) + 0x24));
        if ((u32) var_fp_191 > 0x5FU) {

        } else {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(sp2C, sp2C + 0xC);
            vector_work.position[0] = M2C_FIELD(temp_r6_203, s32 *, 8);
            vector_work.position[1] = M2C_FIELD(temp_r6_203, s32 *, 0xC);
            vector_work.position[2] = M2C_FIELD(temp_r6_203, s32 *, 0x10);
            EffectPosition_ApplyBaseAndYOffset(vector_work.position, vector_work.projected);
            vector_work.projected[0] = *sp24 + sp28;
            vector_work.projected[1] -= 0x18;
            if (var_fp_191 > 0x43) {

            } else {
                sp14 = 0;
                temp_r1_252 = 0x2A000 - spC;
                var_r5_253 = (sp10 * 0x1C) + sp44 + 0x7080;
                temp_r3_257 = (0x40 - var_fp_191) << 9;
                var_r7_258 = 0;
                do {
                    Render_ResetTransformState();
                    if (var_fp_191 <= 0x3F) {
                        vector_work.modulation[0] = temp_r1_252;
                        vector_work.modulation[1] = temp_r1_252;
                        vector_work.modulation[2] = temp_r1_252;
                        SceneTransform_ApplyScale(vector_work.modulation);
                        SceneTransform_ApplyRoll(temp_r3_257);
                        SceneTransform_ApplyYaw(temp_r3_257);
                    }
                    SceneTransform_ApplyRoll(sp14);
                    EffectPosition_ApplyBaseAndYOffset((s32 *)0x080EE134, vector_work.position);
                    M2C_FIELD(var_r5_253, s32 *, 0xC) =
                        vector_work.position[0] + vector_work.projected[0];
                    M2C_FIELD(var_r5_253, s32 *, 0x10) =
                        vector_work.position[1] + vector_work.projected[1] + 0x10;
                    var_r7_258 += 1;
                    sp14 += (s32)&Value_00005555;
                    var_r5_253 += 0x1C;
                } while (var_r7_258 != 3);
                sp1C = sp10;
                var_r7_304 = 0;
                do {
                    temp_r7_314 = var_r7_304 + 1;
                    sp20 = sp44 + ((var_r7_304 + sp1C) * 0x1C) + 0x7080;
                    sp18 = temp_r7_314;
                    var_r2_331 = var_fp_191;
                    temp_r9_332 = sp44 + ((Math_Mod(temp_r7_314, 3) + sp1C) * 0x1C) + 0x7080;
                    if (var_r2_331 < 0) {
                        var_r2_331 += 0xF;
                    }
                    var_r4_339 = 5 - (var_r2_331 >> 4);
                    var_r8_341 = 0;
                    temp_r7_342 = var_r4_339 * 2;
loop_23:
                    temp_r6_346 = M2C_FIELD(sp20, s32 *, 0xC);
                    sp8 = var_r4_339;
                    temp_r5_357 = M2C_FIELD(sp20, s32 *, 0x10);
                    temp_r6_359 = temp_r6_346 + Math_Div(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0xC) - temp_r6_346), 0x18);
                    temp_r3_376 = (temp_r5_357 + Math_Div(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0x10) - temp_r5_357), 0x18)) - var_r4_339;
                    sp34(sp40,
                        (u8 *)sp44
                            + BattleFx6_FlareCells[var_r4_339 - 1]
                            + 0x1000,
                        temp_r6_359 - var_r4_339,
                        temp_r3_376,
                        temp_r7_342,
                        temp_r7_342);
                    var_r8_341 += 1;
                    if (var_r8_341 != 0x18) {
                        goto loop_23;
                    }
                    var_r7_304 = sp18;
                } while (var_r7_304 != 3);
            }
            if (var_fp_191 > 0x3F) {
                sp34(sp40, sp44,
                    vector_work.projected[0] - 0x18,
                    vector_work.projected[1] - 0x18,
                    0x18, 0x30);
                sp38(sp40, sp44,
                    vector_work.projected[0],
                    vector_work.projected[1] - 0x18,
                    0x18, 0x30);
            }
        }
        temp_r5_429 = sp30 + 1;
        sp10 += 0x20;
        spC += 0xFFFFD000;
        var_fp_191 -= 8;
        sp30 = temp_r5_429;
        if (temp_r5_429 != 1) {
            goto loop_11;
        }
        Runtime_ReleaseHeapBlock(0x2F);
        Runtime_ReleaseHeapBlock(0x2E);
        M2C_FIELD(sp44, s32 *, 0x7824) = temp_r5_429;
        WaitFrames(1);
        sp3C += 1;
        if (sp3C != ((M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) * 0x14) + 0x48)) {
            goto loop_3;
        }
    }
    Scheduler_RemoveCallback(0x080CD261);
    return BattleFx_EndCanvasLayer();
}
