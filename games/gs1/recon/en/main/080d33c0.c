#include "types.h"

/*
 * Semantic reconstruction of a battle presentation that seeds thirty moving
 * particles around one actor, steers them toward the listed party members,
 * and draws their approach and impact frames through the two generated
 * rectangle routines.  Names remain conservative where the retained owner
 * establishes layout and behavior but not the original design terminology.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

void *Func_08002f40(s32 resource_id);
void **Func_080b5098(s32 member_id);
extern u8 Value_000000cd;

typedef void (*DrawRectangleFn)(
    void *destination, const void *source, s32 x, s32 y, s32 width, s32 height);

s32 Func_080d33c0(s32 actor) {
    s32 sp8;
    DrawRectangleFn rectangle[2];
    void *canvas;
    s32 sp18[3];
    s32 *var_r2_216;
    s32 temp_r0_123;
    s32 temp_r0_28;
    s32 temp_r0_340;
    s32 temp_r1_354;
    s32 temp_r4_347;
    s32 temp_r9_71;
    s32 var_fp_203;
    s32 var_r1_144;
    s32 var_r2_362;
    s32 var_r2_371;
    s32 var_r2_380;
    s32 var_r3_217;
    s32 var_r3_276;
    s32 var_r8_136;
    s32 var_r8_251;
    s32 var_r8_76;
    s32 var_r9_259;
    s32 temp_r6_209;
    u32 temp_r0_460;
    u32 temp_r3_438;
    void **temp_r8_25;
    void **heap_cache;
    void **cursor;
    void *temp_r1_333;
    void *temp_r2_151;
    void *temp_r5_407;
    void *temp_r5_422;
    void *temp_r6_64;
    void *temp_r7_17;
    void *var_r5_145;
    void *var_r5_78;
    void *var_r6_260;

    heap_cache = (void **)0x03001eec;
    cursor = heap_cache;
    temp_r7_17 = *cursor++;
    canvas = *cursor;
    M2C_FIELD(temp_r7_17, s32 *, 0x7828) = actor;
    temp_r8_25 = temp_r7_17 + 0x7828;
    Func_080cd594(1);
    temp_r0_28 = Func_08002f40((s32)&Value_000000cd);
    Func_080072f0(0x05000000, temp_r0_28, 0x80, 0x03001388);
    Func_08005340(temp_r0_28 + 0x80, temp_r7_17);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    rectangle[0] = (DrawRectangleFn)heap_cache[7];
    Func_080ed408(0x2F, 7, 7, 0xF, 2);
    *(s16 *)0x04000052 = 0xF0F;
    rectangle[1] = (DrawRectangleFn)heap_cache[8];
    temp_r6_64 = *Func_080b5098(M2C_FIELD(*temp_r8_25, s32 *, 8));
    temp_r9_71 = M2C_FIELD(temp_r6_64, s32 *, 0xC) + Func_080b5070(M2C_FIELD(*temp_r8_25, s32 *, 8));
    var_r8_76 = 0;
    var_r5_78 = temp_r7_17 + 0x7080;
    do {
        M2C_FIELD(var_r5_78, s32 *, 4) = temp_r9_71;
        M2C_FIELD(var_r5_78, s32 *, 0) = (s32) M2C_FIELD(temp_r6_64, s32 *, 8);
        M2C_FIELD(var_r5_78, s32 *, 8) = (s32) M2C_FIELD(temp_r6_64, s32 *, 0x10);
        M2C_FIELD(var_r5_78, s32 *, 0xC) = (s32) ((s32) ((Func_08004458() - 0x7F) << 0x10) >> 5);
        M2C_FIELD(var_r5_78, s32 *, 0x10) = (s32) ((s32) (((0x7F & Func_08004458()) - 0x10) << 0x10) >> 6);
        M2C_FIELD(var_r5_78, s32 *, 0x14) = (s32) ((s32) ((Func_08004458() - 0x7F) << 0x10) >> 5);
        temp_r0_123 = var_r8_76;
        var_r8_76 += 1;
        M2C_FIELD(var_r5_78, s32 *, 0x18) = -1;
        ((u8 *)0x02010000)[temp_r0_123] = 0;
        var_r5_78 += 0x1C;
    } while (var_r8_76 != 0x1E);
    var_r8_136 = 0;
    if (M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x14) != 0) {
        var_r1_144 = 0x24;
        var_r5_145 = temp_r7_17 + 0x7400;
        do {
            sp8 = var_r1_144;
            temp_r2_151 = *Func_080b5098(*(s16 *)((u8 *)M2C_FIELD(
                temp_r7_17, void **, 0x7828) + var_r1_144));
            M2C_FIELD(var_r5_145, s32 *, 0) = (s32) M2C_FIELD(temp_r2_151, s32 *, 8);
            M2C_FIELD(var_r5_145, s32 *, 4) = 0;
            M2C_FIELD(var_r5_145, s32 *, 8) = (s32) M2C_FIELD(temp_r2_151, s32 *, 0x10);
            var_r8_136 += 1;
            var_r1_144 += 2;
            var_r5_145 += 0x1C;
        } while (var_r8_136 != M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x14));
    }
    M2C_FIELD(temp_r7_17, s32 *, 0x77AC) = 0;
    M2C_FIELD(temp_r7_17, s32 *, 0x77B0) = 0;
    Func_080041d8(0x080D6505, 0x480);
    M2C_FIELD(temp_r7_17, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r7_17, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    Func_080f9010(0xA4);
    var_fp_203 = 0;
    if (*((u8 *)0x080EE1C4 + (s32) ((M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2) + 1)) == 0) {

    } else {
loop_7:
        temp_r6_209 = *(s32 *)0x03001e80;
        if ((u32) (var_fp_203 - 0x11) <= 0x2EU) {
            var_r2_216 = temp_r7_17 + 0x77AC;
            var_r3_217 = 0x180;
        } else {
            var_r2_216 = temp_r7_17 + 0x77AC;
            var_r3_217 = 0;
        }
        *var_r2_216 = var_r3_217;
        if (var_fp_203 == (*((u8 *)0x080EE1C4 + (s32) ((M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2) + 1)) - 0x10)) {
            Func_080b50e8(0x84);
        }
        Func_080049ac();
        Func_080051d8(temp_r6_209, temp_r6_209 + 12);
        var_r8_251 = 0;
        if (*((u8 *)0x080EE1C4 + (M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2)) == 0) {

        } else {
            var_r9_259 = 0;
            var_r6_260 = temp_r7_17 + 0x7080;
loop_15:
            if ((var_fp_203 > var_r9_259) && ((s8) ((u8 *)0x02010000)[var_r8_251] == 0)) {
                Func_080e3944(var_r6_260, &sp18);
                M2C_FIELD(&sp18, s32 *, 0) = (s32) ((s32) M2C_FIELD(&sp18, s32 *, 0) >> 1);
                var_r3_276 = M2C_FIELD(&sp18, s32 *, 8);
                if (var_r3_276 <= 0x9F) {
                    var_r3_276 = 0xA0;
                    M2C_FIELD(&sp18, s32 *, 8) = 0xA0;
                }
                if (var_r3_276 > 0x31F) {
                    M2C_FIELD(&sp18, s32 *, 8) = 0x31F;
                }
                rectangle[0](canvas, temp_r7_17 + 0xC00,
                    M2C_FIELD(&sp18, s32 *, 0) - 6,
                    M2C_FIELD(&sp18, s32 *, 4) - 0xC, 0xC, 0x18);
                M2C_FIELD(var_r6_260, s32 *, 0) = (s32) (M2C_FIELD(var_r6_260, s32 *, 0) + M2C_FIELD(var_r6_260, s32 *, 0xC));
                M2C_FIELD(var_r6_260, s32 *, 4) = (s32) (M2C_FIELD(var_r6_260, s32 *, 4) + M2C_FIELD(var_r6_260, s32 *, 0x10));
                M2C_FIELD(var_r6_260, s32 *, 8) = (s32) (M2C_FIELD(var_r6_260, s32 *, 8) + M2C_FIELD(var_r6_260, s32 *, 0x14));
            }
            if ((var_fp_203 > (s32) (var_r9_259 + 0x30)) && ((s8) ((u8 *)0x02010000)[var_r8_251] == 0)) {
                temp_r1_333 = temp_r7_17 + (Func_080022fc(var_r8_251, M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x14)) * 0x1C);
                temp_r0_340 = M2C_FIELD(var_r6_260, s32 *, 0xC) + ((s32) (M2C_FIELD(temp_r1_333, s32 *, 0x7400) - M2C_FIELD(var_r6_260, s32 *, 0)) >> 9);
                M2C_FIELD(var_r6_260, s32 *, 0xC) = temp_r0_340;
                temp_r4_347 = M2C_FIELD(var_r6_260, s32 *, 0x10) + ((s32) (M2C_FIELD(temp_r1_333, s32 *, 0x7404) - M2C_FIELD(var_r6_260, s32 *, 4)) >> 9);
                M2C_FIELD(var_r6_260, s32 *, 0x10) = temp_r4_347;
                temp_r1_354 = M2C_FIELD(var_r6_260, s32 *, 0x14) + ((s32) (M2C_FIELD(temp_r1_333, s32 *, 0x7408) - M2C_FIELD(var_r6_260, s32 *, 8)) >> 9);
                M2C_FIELD(var_r6_260, s32 *, 0x14) = temp_r1_354;
                if (var_fp_203 < (s32) (var_r9_259 + 0x55)) {
                    var_r2_362 = temp_r0_340 * 0x3C;
                    if (var_r2_362 < 0) {
                        var_r2_362 += 0x3F;
                    }
                    M2C_FIELD(var_r6_260, s32 *, 0xC) = (s32) (var_r2_362 >> 6);
                    var_r2_371 = temp_r4_347 * 0x3C;
                    if (var_r2_371 < 0) {
                        var_r2_371 += 0x3F;
                    }
                    M2C_FIELD(var_r6_260, s32 *, 0x10) = (s32) (var_r2_371 >> 6);
                    var_r2_380 = temp_r1_354 * 0x3C;
                    if (var_r2_380 < 0) {
                        var_r2_380 += 0x3F;
                    }
                    M2C_FIELD(var_r6_260, s32 *, 0x14) = (s32) (var_r2_380 >> 6);
                }
                if ((s32) M2C_FIELD(var_r6_260, s32 *, 4) < 0) {
                    ((u8 *)0x02010000)[var_r8_251] = 1;
                    M2C_FIELD(var_r6_260, u32 *, 0x18) = 0U;
                    M2C_FIELD(var_r6_260, s32 *, 0) = (s32) M2C_FIELD(&sp18, s32 *, 0);
                    temp_r5_407 = M2C_FIELD(temp_r7_17, void **, 0x7828);
                    M2C_FIELD(var_r6_260, s32 *, 4) = (s32) ((M2C_FIELD(&sp18, s32 *, 4) + (0x1F & Func_08004458())) - 0x10);
                    Func_080d6888(*(s16 *)((u8 *)temp_r5_407 + ((Func_080022fc(var_r8_251, M2C_FIELD(temp_r5_407, s32 *, 0x14)) * 2) + 0x24)), 7, 5, 4);
                    temp_r5_422 = M2C_FIELD(temp_r7_17, void **, 0x7828);
                    Func_080b5088(*(s16 *)((u8 *)temp_r5_422 + ((Func_080022fc(var_r8_251, M2C_FIELD(temp_r5_422, s32 *, 0x14)) * 2) + 0x24)), 0);
                    M2C_FIELD(temp_r7_17, s32 *, 0x77A8) = 4;
                    Func_080f9010(0x84);
                }
            }
            temp_r3_438 = M2C_FIELD(var_r6_260, u32 *, 0x18);
            if (temp_r3_438 <= 0xFU) {
                rectangle[0](canvas, temp_r7_17 + (Func_080022fc((s32) (temp_r3_438 + (temp_r3_438 >> 0x1F)) >> 1, 3) << 0xA), M2C_FIELD(var_r6_260, s32 *, 0) - 0x10, M2C_FIELD(var_r6_260, s32 *, 4) - 0x38, 0x10, 0x40);
                temp_r0_460 = M2C_FIELD(var_r6_260, u32 *, 0x18);
                rectangle[1](canvas, temp_r7_17 + (Func_080022fc((s32) (temp_r0_460 + (temp_r0_460 >> 0x1F)) >> 1, 3) << 0xA), M2C_FIELD(var_r6_260, s32 *, 0), M2C_FIELD(var_r6_260, s32 *, 4) - 0x38, 0x10, 0x40);
                M2C_FIELD(var_r6_260, u32 *, 0x18) = (u32) (M2C_FIELD(var_r6_260, u32 *, 0x18) + 1);
            }
            var_r8_251 += 1;
            var_r9_259 += 2;
            var_r6_260 += 0x1C;
            if (var_r8_251 != *((u8 *)0x080EE1C4 + (M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2))) {
                goto loop_15;
            }
        }
        Func_080e155c((M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2) + 2);
        if (M2C_FIELD(temp_r7_17, s32 *, 0x77B0) == 0) {
            M2C_FIELD(temp_r7_17, s32 *, 0x77B0) = 1;
        }
        Func_080cd52c();
        M2C_FIELD(temp_r7_17, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        var_fp_203 += 1;
        if (var_fp_203 != *((u8 *)0x080EE1C4 + (s32) ((M2C_FIELD(M2C_FIELD(temp_r7_17, void **, 0x7828), s32 *, 0x18) * 2) + 1))) {
            goto loop_7;
        }
    }
    Func_08004278(0x080D6505);
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}
