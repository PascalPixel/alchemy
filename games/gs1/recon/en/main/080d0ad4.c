#include "types.h"
#include "effect_step.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void **Func_080b5098(s32 member_id);
void Func_080e396c(s32 member_id, struct EffectPosition *result);
s32 Func_080ed408(s32 id, s32 width, s32 height, s32 mode, s32 variant);

typedef void (*BattleEffectDrawFn)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

extern u8 Value_00000073;
extern u8 Value_00000076;
extern u8 Value_00000079;
extern void *Data_03001e50[];
extern const u16 Data_080ede5c[];

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

s32 Func_080d0ad4(s32 actor) {
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
    s32 temp_r0_37;
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
    Func_080cd594(0);
    *(u16 *)0x04000020 = 0x100;
    temp_r0_37 = Func_08002f40((s32)&Value_00000079);
    Func_080072f0(0x05000000, temp_r0_37, 0x80, 0x03001388);
    Func_08005340(temp_r0_37 + 0x80, sp44);
    Func_08005340(
        Func_08002f40((s32)&Value_00000073),
        temp_r6_25);
    Func_08005340(
        Func_08002f40((s32)&Value_00000076),
        temp_r2_18 + 0x1000);
    M2C_FIELD(sp44, s32 *, 0x7780) = 3;
    M2C_FIELD(sp44, s32 *, 0x7784) = 0x04040404;
    Func_080041d8(0x080CD261, 0x480);
    sp24 = vector_work.output;
    Func_080e396c(
        M2C_FIELD(*temp_r5_28, s16 *, 0x24),
        (struct EffectPosition *)vector_work.output);
    temp_r3_103 = 0x40 - *sp24;
    sp28 = temp_r3_103;
    *(s32 *)0x04000028 = temp_r3_103 << 8;
    Func_080f9010(0x8E);
    sp3C = 0;
    if ((M2C_FIELD(*temp_r5_28, s32 *, 0x14) * 0x14) == -0x48) {

    } else {
loop_3:
        if (sp3C == 0x40) {
            Func_080b50e8(0);
        }
        if (sp3C == 0x2E) {
            temp_r3_135 = M2C_FIELD(sp44, void **, 0x7828);
            Func_080b5078(M2C_FIELD(temp_r3_135, s32 *, 8), M2C_FIELD(temp_r3_135, s16 *, 0x24), 0x10, 0);
        }
        Func_080d40ec(sp3C, 0xAAAB, 0x5555, 0);
        Func_080ed408(0x2E, 7, 7, 3, 2);
        sp34 = (BattleEffectDrawFn)Data_03001e50[46];
        Func_080ed408(0x2F, 7, 7, 7, 2);
        sp38 = (BattleEffectDrawFn)Data_03001e50[47];
        if ((sp3C > 0x10) && !(0xF & sp3C)) {
            M2C_FIELD(sp44, s32 *, 0x7784) = (s32) (M2C_FIELD(sp44, s32 *, 0x7784) + 0x01010101);
        }
        sp30 = 0;
        sp10 = 0;
        spC = sp3C * 0x600;
        var_fp_191 = sp3C;
loop_11:
        temp_r6_203 = *Func_080b5098(M2C_FIELD(
            M2C_FIELD(sp44, void **, 0x7828), s16 *,
            (sp30 * 2) + 0x24));
        if ((u32) var_fp_191 > 0x5FU) {

        } else {
            Func_080049ac();
            Func_080051d8(sp2C, sp2C + 0xC);
            vector_work.position[0] = M2C_FIELD(temp_r6_203, s32 *, 8);
            vector_work.position[1] = M2C_FIELD(temp_r6_203, s32 *, 0xC);
            vector_work.position[2] = M2C_FIELD(temp_r6_203, s32 *, 0x10);
            Func_080e3944(vector_work.position, vector_work.projected);
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
                    Func_080049ac();
                    if (var_fp_191 <= 0x3F) {
                        vector_work.modulation[0] = temp_r1_252;
                        vector_work.modulation[1] = temp_r1_252;
                        vector_work.modulation[2] = temp_r1_252;
                        Func_08004cf0(vector_work.modulation);
                        Func_08004c6c(temp_r3_257);
                        Func_08004c1c(temp_r3_257);
                    }
                    Func_08004c6c(sp14);
                    Func_080e3944((s32 *)0x080EE134, vector_work.position);
                    M2C_FIELD(var_r5_253, s32 *, 0xC) =
                        vector_work.position[0] + vector_work.projected[0];
                    M2C_FIELD(var_r5_253, s32 *, 0x10) =
                        vector_work.position[1] + vector_work.projected[1] + 0x10;
                    var_r7_258 += 1;
                    sp14 += 0x5555;
                    var_r5_253 += 0x1C;
                } while (var_r7_258 != 3);
                sp1C = sp10;
                var_r7_304 = 0;
                do {
                    temp_r7_314 = var_r7_304 + 1;
                    sp20 = sp44 + ((var_r7_304 + sp1C) * 0x1C) + 0x7080;
                    sp18 = temp_r7_314;
                    var_r2_331 = var_fp_191;
                    temp_r9_332 = sp44 + ((Func_080022fc(temp_r7_314, 3) + sp1C) * 0x1C) + 0x7080;
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
                    temp_r6_359 = temp_r6_346 + Func_080022ec(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0xC) - temp_r6_346), 0x18);
                    temp_r3_376 = (temp_r5_357 + Func_080022ec(var_r8_341 * (M2C_FIELD(temp_r9_332, s32 *, 0x10) - temp_r5_357), 0x18)) - var_r4_339;
                    sp34(sp40,
                        (u8 *)sp44
                            + Data_080ede5c[var_r4_339 - 1]
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
        Func_08002dd8(0x2F);
        Func_08002dd8(0x2E);
        M2C_FIELD(sp44, s32 *, 0x7824) = temp_r5_429;
        Func_080030f8(1);
        sp3C += 1;
        if (sp3C != ((M2C_FIELD(M2C_FIELD(sp44, void **, 0x7828), s32 *, 0x14) * 0x14) + 0x48)) {
            goto loop_3;
        }
    }
    Func_08004278(0x080CD261);
    return Func_080cdbc0();
}
