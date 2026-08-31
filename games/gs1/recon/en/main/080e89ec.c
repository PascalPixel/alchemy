#include "types.h"

/*
 * Battle-presentation effect recovered from callback-table entry 49. It
 * builds two opposing particle streams, draws the expanding centre sprites,
 * and advances the eight orbiting particles through the 150-frame sequence.
 * Names remain conservative until the effect is tied to its in-game move.
 */

typedef s32 M2C_UNK;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, u32 width, s32 height);

void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);

struct M2cAggregate_absolute_02010000 {
    u8 unknown_0000[4];
    u32 field_0004;
    u32 field_0008;
    u32 field_000c;
    u32 field_0010;
    u32 field_0014;
    u32 field_0018;
    u8 field_001c;
    u8 unknown_001d[0x1d];
    u8 field_003a;
};

struct M2cAggregate_deref_absolute_03001e80_0 {
    s16 field_0000;
    u8 unknown_0002[0x1a];
    u32 field_001c;
    u32 field_0020;
    u8 unknown_0024[0x12];
    u16 field_0036;
};

struct BattleWorkCache {
    struct M2cAggregate_absolute_02010000 *field_0000;
    void *field_0004;
    void *field_0008;
};

struct BattlePresentationState {
    struct M2cAggregate_deref_absolute_03001e80_0 *field_0000;
};

#define absolute_02010000 (*(struct M2cAggregate_absolute_02010000 *)0x02010000)
#define absolute_03001e80 (*(struct BattlePresentationState *)0x03001e80)
#define absolute_03001eec (*(struct BattleWorkCache *)0x03001eec)
#define absolute_080ede48 ((const u16 *)0x080ede48)
#define absolute_080ede9f ((const u8 *)0x080ede9f)
#define absolute_080edea5 ((const u8 *)0x080edea5)
#define absolute_080edeab ((const u8 *)0x080edeab)
#define absolute_080edeb2 ((const u16 *)0x080edeb2)

#define BattleEffect_RunDualParticleStream Func_080e89ec

void BattleEffect_RunDualParticleStream(void *object) {
    u32 draw_width;
    s32 draw_height;
    u32 phase_frame;
    void **state_slot;
    DrawRectangleFn *callback_ptr;
    void *graphics;
    s32 vertical_offset;
    u32 horizontal_offset;
    void *motion_data;
    s32 vertical_velocity;
    s32 frame;
    void *draw_destination;
    s32 position[3];
    s32 history[40];
    DrawRectangleFn callbacks[2];
    s32 temp_r0_331;
    s32 temp_r0_447;
    s32 temp_r0_715;
    s32 temp_r1_737;
    s32 temp_r1_831;
    s32 temp_r2_584;
    s32 temp_r2_603;
    s32 temp_r3_441;
    s32 temp_r3_563;
    s32 temp_r3_609;
    s32 temp_r3_690;
    s32 temp_r4_786;
    s32 temp_r5_454;
    s32 var_r0_571;
    s32 var_r2_680;
    s32 var_r3_143;
    s32 var_r3_185;
    s32 var_r3_235;
    s32 var_r3_512;
    s32 var_r3_598;
    s32 var_r3_667;
    s32 var_r3_77;
    s32 var_r6_133;
    s32 var_r6_174;
    s32 var_r6_219;
    s32 var_r6_434;
    s32 var_r6_54;
    s32 var_r7_220;
    s32 var_r7_437;
    s32 var_r7_47;
    s32 var_r7_654;
    s32 var_r7_778;
    s8 *var_r1_56;
    struct M2cAggregate_absolute_02010000 *work;
    struct M2cAggregate_absolute_02010000 *var_r2_57;
    struct M2cAggregate_absolute_02010000 *var_r5_221;
    struct M2cAggregate_absolute_02010000 *var_r6_779;
    struct M2cAggregate_deref_absolute_03001e80_0 *temp_fp_275;
    u16 var_r3_306;
    u32 temp_r3_256;
    u32 temp_r3_375;
    u32 temp_r3_568;
    u32 temp_r3_781;
    u32 temp_r4_687;
    u8 temp_r3_72;
    u8 temp_r5_792;
    void *var_r4_50;
    void *var_r5_135;
    void *var_r5_177;
    void *var_r6_655;

    draw_destination = absolute_03001eec.field_0004;
    vertical_velocity = 0;
    horizontal_offset = 0;
    vertical_offset = 0;
    work = M2C_FIELD(&absolute_03001eec, struct M2cAggregate_absolute_02010000 **, 0);
    graphics = absolute_03001eec.field_0008;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;
    Func_080e0524(0xC2, work, 1, 1);
    var_r7_47 = 1;
    var_r4_50 = work + 0x3A8;
    do {
        var_r6_54 = 0;
        var_r1_56 = var_r4_50 + 0xFFFFF1F0;
        var_r2_57 = work + 0x1680;
loop_2:
        temp_r3_72 = var_r2_57->unknown_0000[0];
        var_r2_57 += 1;
        if (var_r7_47 > 0xA) {
            var_r3_77 = (temp_r3_72 - (var_r7_47 * 4)) + 0x28;
            if (var_r3_77 < 0) {
                var_r3_77 = 0;
            }
            *var_r1_56 = (s8) var_r3_77;
        }
        var_r6_54 += 1;
        var_r1_56 += 1;
        if (var_r6_54 != 0x3A8) {
            goto loop_2;
        }
        var_r7_47 += 1;
        var_r4_50 += 0x3A8;
    } while (var_r7_47 != 0x14);
    Func_080e0524(0x73, (struct M2cAggregate_absolute_02010000 *) graphics, 0, 0);
    Func_080e0524(0xB4, work + 0x3C00, 1, 1);
    Func_080e0524(0x7D, &absolute_02010000, 1, 0);
    callback_ptr = callbacks;
    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4),
        callback_ptr);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    var_r6_133 = 0;
    var_r5_135 = work + 0x7160;
    do {
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            var_r3_143 = 0xFFC80000;
        } else {
            var_r3_143 = 0x380000;
        }
        M2C_FIELD(var_r5_135, s32 *, 0) = var_r3_143;
        M2C_FIELD(var_r5_135, s32 *, 4) = 0;
        M2C_FIELD(var_r5_135, s32 *, 8) = 0;
        M2C_FIELD(var_r5_135, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_135, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xD);
        var_r6_133 += 1;
        M2C_FIELD(var_r5_135, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_135, s32 *, 0x18) = 1;
        var_r5_135 += 0x1C;
    } while (var_r6_133 != 0x28);
    var_r6_174 = 0;
    var_r5_177 = work + 0x75C0;
    do {
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            var_r3_185 = 0xFFC80000;
        } else {
            var_r3_185 = 0x380000;
        }
        M2C_FIELD(var_r5_177, s32 *, 0) = var_r3_185;
        M2C_FIELD(var_r5_177, s32 *, 4) = 0x140000;
        M2C_FIELD(var_r5_177, s32 *, 8) = 0;
        M2C_FIELD(var_r5_177, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_177, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xC);
        var_r6_174 += 1;
        M2C_FIELD(var_r5_177, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_177, s32 *, 0x18) = 0;
        var_r5_177 += 0x1C;
    } while (var_r6_174 != 0x10);
    var_r6_219 = 0xFFFFC000;
    var_r7_220 = 0;
    var_r5_221 = work + 0x7080;
    do {
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
            var_r3_235 = ((s32) (Func_08002322(var_r6_219) * 0x18) >> 0x10) + 0x58;
        } else {
            var_r3_235 = ((s32) (0 - (Func_08002322(var_r6_219) * 0x18)) >> 0x10) + 0x10;
        }
        M2C_FIELD(var_r5_221, s32 *, 0) = var_r3_235;
        var_r5_221->field_0004 = ((s32) (Func_0800231c(var_r6_219) * 0x10) >> 0x10) + 0x28;
        temp_r3_256 = 0 - (var_r7_220 * 2);
        var_r7_220 += 1;
        var_r5_221->field_0018 = temp_r3_256;
        var_r6_219 += 0x1000;
        var_r5_221 += 0x1C;
    } while (var_r7_220 != 8);
    motion_data = Func_08002f40(0xD3);
    frame = 0;
    state_slot = work + 0x7828;
loop_24:
    temp_fp_275 = absolute_03001e80.field_0000;
    if (frame == 0x53) {
        Func_080b50e8(0x86);
    }
    if (frame == 0) {
        Func_080f9010(0x88);
    }
    if (frame == 0x32) {
        Func_080f9010(0x88);
    }
    if (M2C_FIELD(*state_slot, s32 *, 4) == 0) {
        if (frame <= 0x3F) {
            var_r3_306 = temp_fp_275->field_0036 + 0xFFFFFF00;
            goto block_35;
        }
    } else if (frame <= 0x3F) {
        var_r3_306 = temp_fp_275->field_0036 + 0x100;
block_35:
        temp_fp_275->field_0036 = var_r3_306;
    }
    Func_080b5028(0, 0, 0, 0x64);
    if (frame <= 0x11) {
        temp_r0_331 = Func_080022ec(frame, 3);
        draw_width = (u32) absolute_080ede9f[temp_r0_331];
        draw_height = (s32) absolute_080edea5[temp_r0_331];
        callbacks[0](draw_destination, &work->unknown_0000[absolute_080edeb2[temp_r0_331]] + 0x3C00, 0x30, absolute_080edeab[temp_r0_331] + 0x3C, draw_width, draw_height);
        draw_width = (u32) absolute_080ede9f[temp_r0_331];
        draw_height = (s32) absolute_080edea5[temp_r0_331];
        callbacks[1](draw_destination, &work->unknown_0000[absolute_080edeb2[temp_r0_331]] + 0x3C00, 0x38, absolute_080edeab[temp_r0_331] + 0x3C, draw_width, draw_height);
    }
    temp_r3_375 = frame - 0x12;
    phase_frame = temp_r3_375;
    if (temp_r3_375 <= 0x28U) {
        if (frame == 0x12) {
            horizontal_offset = (M2C_FIELD(motion_data, s8 *, 0) << 8) + M2C_FIELD(motion_data, u8 *, 1);
            vertical_offset = (M2C_FIELD(motion_data, s8 *, 2) << 8) + M2C_FIELD(motion_data, u8 *, 3) + 0x10;
            motion_data += 4;
        } else {
            horizontal_offset += M2C_FIELD(motion_data, s8 *, 0);
            vertical_offset += (s8) M2C_FIELD(motion_data, u8 *, 1);
            motion_data += 2;
        }
    }
    if ((u32) (frame - 0x4E) <= 0x28U) {
        if (frame == 0x4E) {
            horizontal_offset = -0x38U;
            vertical_offset = 0x30;
        } else {
            vertical_offset -= 0x10;
        }
    }
    var_r6_434 = 0x13;
    var_r7_437 = 0x9C;
    do {
        temp_r3_441 = var_r6_434 + 0x12;
        if ((frame > temp_r3_441) && (frame <= (s32) (temp_r3_441 + 0x41))) {
            temp_r0_447 = var_r6_434 * 8;
            history[temp_r0_447 / 4] = history[(temp_r0_447 - 8) / 4];
            temp_r5_454 = history[(temp_r0_447 - 4) / 4];
            history[var_r7_437 / 4] = temp_r5_454;
            if (var_r6_434 > 0xA) {
                draw_width = 0x18;
                draw_height = 0x27;
                callbacks[0](draw_destination, (var_r6_434 * 0x3A8) + work + 0xFFFFF1F0, history[temp_r0_447 / 4], temp_r5_454, draw_width, draw_height);
            } else {
                draw_height = 0x27;
                draw_width = 0x18;
                callbacks[0](draw_destination, (work + 0x1680)->unknown_0000, history[temp_r0_447 / 4], temp_r5_454, draw_width, draw_height);
            }
        }
        var_r6_434 -= 1;
        var_r7_437 -= 8;
    } while (var_r6_434 != 0);
    Func_080049ac();
    Func_080051d8(temp_fp_275, &temp_fp_275->unknown_0002[0xA]);
    if (phase_frame <= 0x41U) {
        if (M2C_FIELD(*state_slot, s32 *, 4) == 1) {
            var_r3_512 = 0x40 - ((s32) (horizontal_offset + (horizontal_offset >> 0x1F)) >> 1);
        } else {
            var_r3_512 = ((s32) (horizontal_offset + (horizontal_offset >> 0x1F)) >> 1) + 0x40;
        }
        position[0] = var_r3_512;
        temp_r3_563 = 0x3C - vertical_offset;
        position[1] = temp_r3_563;
        temp_r3_568 = (temp_r3_563 - history[1]) - 0x18;
        var_r0_571 = (s32) (temp_r3_568 + (temp_r3_568 >> 0x1F)) >> 1;
        if (var_r0_571 > 2) {
            var_r0_571 = 2;
        }
        if (var_r0_571 < -2) {
            var_r0_571 = -2;
        }
        temp_r2_584 = vertical_velocity + var_r0_571;
        vertical_velocity = temp_r2_584;
        if (temp_r2_584 > 8) {
            vertical_velocity = 8;
        }
        if (vertical_velocity < -8) {
            vertical_velocity = -8;
        }
        var_r3_598 = vertical_velocity;
        if (var_r3_598 < 0) {
            var_r3_598 += 3;
        }
        temp_r2_603 = position[0];
        history[0] = temp_r2_603 - 0xC;
        temp_r3_609 = position[1];
        history[1] = temp_r3_609 - 0x14;
        draw_width = 0x18;
        draw_height = 0x30;
        callbacks[0](draw_destination, (((var_r3_598 >> 2) + 2) * 0x480) + work, temp_r2_603 - 0x12, temp_r3_609 - 0x16, draw_width, draw_height);
    }
    if (frame == 0x53) {
        M2C_FIELD(work, s32 *, 0x77A8) = 8;
        draw_width = 8;
        Func_080d6888(M2C_FIELD(*state_slot, s16 *, 0x24), 7, 5, 0);
        Func_080b5088(M2C_FIELD(*state_slot, s16 *, 0x24), 1);
    }
    if (frame > 0x53) {
        var_r7_654 = 0;
        var_r6_655 = work + 0x7160;
        do {
            if ((s32) M2C_FIELD(var_r6_655, s32 *, 4) >= 0) {
                Func_080e3944(var_r6_655, position);
                position[0] >>= 1;
                var_r3_667 = position[2];
                if (var_r3_667 <= 0x9F) {
                    var_r3_667 = 0xA0;
                    position[2] = 0xA0;
                }
                if (var_r3_667 > 0x31F) {
                    position[2] = 0x31F;
                    var_r3_667 = 0x31F;
                }
                var_r2_680 = var_r3_667 - 0xA0;
                if (var_r2_680 < 0) {
                    var_r2_680 += 0x3F;
                }
                temp_r4_687 = 9 - (var_r2_680 >> 6);
                if (var_r7_654 > 0x2F) {
                    temp_r3_690 = M2C_FIELD(var_r6_655, s32 *, 0x18);
                    if (temp_r3_690 <= 0xB) {
                        draw_width = 0x20;
                        draw_height = 0x40;
                        callbacks[0](draw_destination, (((s32) (temp_r3_690 + ((u32) temp_r3_690 >> 0x1F)) >> 1) << 0xB) + &absolute_02010000, position[0] - 0x10, position[1] - 0x20, draw_width, draw_height);
                        M2C_FIELD(var_r6_655, s32 *, 0x18) = (s32) (M2C_FIELD(var_r6_655, s32 *, 0x18) + 1);
                    }
                } else {
                    temp_r0_715 = temp_r4_687 * 2;
                    draw_width = temp_r4_687;
                    draw_height = temp_r0_715;
                    callbacks[0](draw_destination, graphics + *(absolute_080ede48 + (temp_r0_715 - 2)), position[0] - ((s32) (temp_r4_687 + (temp_r4_687 >> 0x1F)) >> 1), position[1] - temp_r4_687, draw_width, draw_height);
                }
                temp_r1_737 = M2C_FIELD(var_r6_655, s32 *, 0x10);
                M2C_FIELD(var_r6_655, s32 *, 0) = (s32) (M2C_FIELD(var_r6_655, s32 *, 0) + M2C_FIELD(var_r6_655, s32 *, 0xC));
                M2C_FIELD(var_r6_655, s32 *, 4) = (s32) (M2C_FIELD(var_r6_655, s32 *, 4) + temp_r1_737);
                M2C_FIELD(var_r6_655, s32 *, 8) = (s32) (M2C_FIELD(var_r6_655, s32 *, 8) + M2C_FIELD(var_r6_655, s32 *, 0x14));
                M2C_FIELD(var_r6_655, s32 *, 0x10) = (s32) (temp_r1_737 + 0xFFFFE000);
            }
            var_r7_654 += 1;
            var_r6_655 += 0x1C;
        } while (var_r7_654 != 0x38);
    }
    if (frame == 0x32) {
        M2C_FIELD(work, s32 *, 0x77A8) = 0xC;
        draw_width = 8;
        Func_080d6888(M2C_FIELD(*state_slot, s16 *, 0x24), 7, 5, 0);
    }
    if (frame > 0x31) {
        var_r7_778 = 0;
        var_r6_779 = work + 0x7080;
        do {
            temp_r3_781 = var_r6_779->field_0018;
            if (temp_r3_781 <= 0xBU) {
                temp_r4_786 = (s32) (temp_r3_781 + (temp_r3_781 >> 0x1F)) >> 1;
                temp_r5_792 = absolute_080ede9f[temp_r4_786];
                draw_width = (u32) temp_r5_792;
                draw_height = (s32) absolute_080edea5[temp_r4_786];
                callbacks[1](draw_destination, &work->unknown_0000[absolute_080edeb2[temp_r4_786]] + 0x3C00, M2C_FIELD(var_r6_779, s32 *, 0) - (temp_r5_792 >> 1), var_r6_779->field_0004 + absolute_080edeab[temp_r4_786], draw_width, draw_height);
            }
            var_r7_778 += 1;
            var_r6_779->field_0018 += 1;
            var_r6_779 += 0x1C;
        } while (var_r7_778 != 8);
    }
    Func_080e155c(8, 8);
    Func_080cd52c();
    M2C_FIELD(work, s32 *, 0x7824) = 1;
    Func_080030f8(1);
    temp_r1_831 = frame + 1;
    frame = temp_r1_831;
    if (temp_r1_831 != 0x96) {
        goto loop_24;
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
