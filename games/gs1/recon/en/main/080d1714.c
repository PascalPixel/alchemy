#include "b5_context.h"
#include "battle_types.h"
#include "types.h"

/*
 * Battle-presentation scene at 0x080d1714, a 400-frame sequence driven from
 * the shared cell window at Data_03001eec.  Structure recovered against
 * games/gs1/asm/080d1714.s; the neighbouring draft
 * games/gs1/recon/en/main/080d1350.c supplied the cell window, the
 * EffectRuntime offsets (0x7780 display pair, 0x77a8 impact mode, 0x7824
 * ready flag, 0x7828 argument pointer) and the indirect draw/copy idioms.
 *
 * The frame body is one do-while over `frame`; the reference reaches it with
 * a long `bl` back-branch because the body exceeds Thumb's short-branch
 * range.  Skip input (Data_03001b04 & 3) fast-forwards `frame` to 160 or 395.
 *
 * runtime->points[64] is one 28-byte record array that the scene reuses in
 * three disjoint ranges: [0..31] for the ring/burst particles, [32..59] for
 * the twenty-eight orbiting sparks, and [60..63] for the four ground panels.
 * The array bounds are fixed by the reference: 0x7080 + 64 * 28 lands exactly
 * on the 0x7780 display pair, 0x7400 is points[32] and 0x7710 is points[60].
 * Field roles are only partly established, so they keep offset names; the
 * same offset serves more than one purpose across phases (points[i].field_10
 * is an angle in the frame <= 159 pass and a velocity in the later ones).
 *
 * Residual: structure, extents and call arguments track the reference, but
 * register allocation does not.  The reference keeps three loop counters in
 * r9/sl/fp where this draft spills them, and its twenty-four spill slots are
 * assigned in a different order, so nearly every sp-relative access reads as
 * a difference even where the surrounding instructions agree.  The candidate
 * also carries a few extra induction variables in the four-panel loop and
 * places its literal pools at different points.
 *
 * Uncertain / preserved oddities, all present in the reference:
 *   - the points[] reset loop calls Func_08004458 twice and discards the
 *     first result;
 *   - the spawn loop at Data_080ee16c[] writes field_10 twice, the second
 *     store overwriting the first;
 *   - the 512-entry Data_02010000 simulation sits inside the four-iteration
 *     Data_080ee16c[] loop, so it advances and draws four times per frame;
 *   - the two-word `scale` pair is copied from Data_080eda78 and then
 *     immediately overwritten with 0x10000 in both lanes.
 */

typedef void (*WordCopy)(void *destination, const void *source, s32 size);
typedef s32 (*IntegerSquareRoot)(s32 value);
typedef void (*DrawRectangle)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct EffectArgument {
    u8 unknown_00[8];
    s32 source_id;
    u8 unknown_0c[8];
    s32 target_count;
    u8 unknown_18[12];
    s16 target_ids[BATTLE_TARGET_CAPACITY];
};

struct EffectObject {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x14];
    s32 word_28;
    u8 unknown_2c[0x1c];
    s32 word_48;
};

struct Particle {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
};

struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

struct Placement {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

struct ScalePair {
    s32 x;
    s32 y;
};

struct SceneCameraObject {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
};

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    struct Particle points[64];
    s32 display_mode;
    s32 display_value;
    u8 unknown_7788[0x20];
    s32 impact_mode;
    u8 unknown_77ac[0x2c];
    s32 objects[9];
    u8 unknown_77fc[0x28];
    s32 frame_ready;
    struct EffectArgument *argument;
};

/*
 * Cell window shared by the battle-presentation scenes.  The owner walks it
 * from the destination cell: cell[-1] is the effect work block, cell[0] the
 * draw destination, cell[1] the graphics block, cell[6] and cell[7] the two
 * rectangle helpers, and the transfer window sits 112 bytes below.  The
 * neighbouring draft games/gs1/recon/en/main/080d1350.c walks the same
 * window from cell[-1] instead.
 */
extern void *Data_03001eec[];
extern struct SceneCameraObject Data_03001ce0;
extern volatile u32 Data_03001b04;
extern struct Particle Data_02010000[];
extern struct ScalePair Data_080eda78;
extern u8 Data_0000003b[];
extern u8 Data_00000073[];
extern u8 Data_00000082[];
extern u8 Data_00000088[];
extern u16 Data_080ede48[];
extern u8 Data_080ee15a[];
extern u8 Data_080ee163[];
extern u16 Data_080ee16c[];
extern u8 Data_080ee174[];
extern u8 Data_080ee177[];
extern u8 Data_080ee17a[];
extern u16 Data_080ee17e[];

void Func_080cd594(s32 mode);
void Func_080ed408(s32, s32, s32, s32, s32);
u8 *Func_08002f40(s32 resource_id);
void Func_08005340(const void *source, void *destination);
void Func_080041d8(void (*callback)(void), s32 interval);
void Func_08004278(void (*callback)(void));
void Func_080cd260(void);
s32 Func_08004458(void);
s32 Func_080044d0(s32 x, s32 z);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
s32 Func_08002304(s32 numerator, s32 denominator);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_080049ac(void);
void Func_080051d8(void *source, void *destination);
void Func_08004c1c(s32 yaw);
void Func_08004c6c(s32 pitch);
void Func_08004cb4(struct Vector3 *position);
void Func_080f9010(s32 cue);
void Func_080e3944(const void *source, struct Vector3 *output);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080b5038(s32, s32, s32);
void Func_080b5040(s32, s32, s32);
void Func_080b50e8(s32 value);
void Func_080d6750(struct EffectArgument *argument);
void Func_080dbb24(s32, s32, s32);
void Func_08009008(s32 handle, struct Placement *place, struct ScalePair *scale,
    s32 mode);
void Func_08009038(s32 handle);
void Func_080d67dc(void);
void Func_080cd52c(void);
void Func_080030f8(s32 count);
void Func_08002dd8(s32 block);
void Func_080cdbc0(void);

void Func_080d1714(struct EffectArgument *argument)
{
    void **cells;
    struct EffectRuntime *runtime;
    struct EffectObject *object;
    struct B5Context *context;
    struct Particle *point;
    struct Particle *spark;
    void *draw_destination;
    void *view;
    u8 *graphics;
    u8 *tbl;
    u8 *src;
    DrawRectangle draw_rectangle;
    DrawRectangle draw_rectangle_alt;
    s32 frame;
    s32 index;
    s32 slot;
    s32 step;
    s32 size;
    s32 span;
    s32 dist;
    s32 alive;
    s32 speed;
    s32 angle;
    s32 threshold;
    s32 wave_x;
    s32 wave_y;
    s32 drift_x;
    s32 drift_y;
    s32 heading[8];
    s32 radius[8];
    s32 spin[8];
    struct ScalePair home[8];
    s32 base_angle[8];
    struct Vector3 work;
    struct Vector3 screen;
    struct Vector3 shake;
    struct Placement place;
    struct ScalePair scale;

    cells = &Data_03001eec[1];
    draw_destination = cells[0];
    view = *(void **)((u8 *)cells - 112);
    runtime = cells[-1];
    graphics = (u8 *)cells[1];
    draw_rectangle = (DrawRectangle)cells[6];
    draw_rectangle_alt = (DrawRectangle)cells[7];
    runtime->argument = argument;

    Func_080cd594(0);
    Func_080ed408(46, 7, 7, 3, 2);
    Func_080ed408(47, 7, 7, 3, 3);

    tbl = Func_08002f40((s32)Data_00000082);
    ((WordCopy)0x03001388)((void *)0x05000000, tbl, 0x80);
    Func_08005340(tbl + 0x80, runtime);
    Func_08005340(Func_08002f40((s32)Data_00000073), graphics);

    runtime->display_mode = 2;
    runtime->display_value = 50;
    Func_080041d8(Func_080cd260, 0x480);

    wave_x = 0x800000;
    wave_y = 0x280000;
    drift_x = 0;
    drift_y = 0;

    index = 0;
    if (runtime->argument->target_count != 0) {
        do {
            context = Func_080b5098(runtime->argument->target_ids[index]);
            object = (struct EffectObject *)context->object;
            home[index].x = object->x;
            home[index].y = object->z;
            base_angle[index] = object->angle;
            heading[index] = (u16)Func_080044d0(object->x, object->z);
            radius[index] = ((IntegerSquareRoot)0x030001d8)(
                (object->x >> 8) * (object->x >> 8)
                    + (object->z >> 8) * (object->z >> 8)) >> 7;
            spin[index] = 0;
            object->word_48 = 0;
            index++;
        } while (index != runtime->argument->target_count);
    }

    point = runtime->points;
    index = 0;
    do {
        point->field_08 = 120;
        point->field_04 = 0;
        /* The reference discards this draw; kept so the sequence matches. */
        Func_08004458();
        point->field_10 = 0;
        point->field_0c = 0;
        point->field_18 = Func_08004458() & 63;
        index++;
        point++;
    } while (index != 64);

    frame = 0;
    do {
        if ((Data_03001b04 & 3) != 0) {
            if (frame <= 159)
                frame = 160;
            else if (frame <= 394)
                frame = 395;
        }

        Func_080049ac();
        Func_080051d8(view, (u8 *)view + 12);

        if (frame == 16)
            Func_080f9010(141);
        if (frame == 256)
            Func_080f9010(140);
        if (frame == 334)
            Func_080f9010(212);
        if (frame == 347)
            Func_080f9010(212);
        if (frame == 359)
            Func_080f9010(212);
        if (frame == 372)
            Func_080f9010(212);

        index = 0;
        if (runtime->argument->target_count != 0) {
            do {
                if (frame > index * 16) {
                    context = Func_080b5098(
                        runtime->argument->target_ids[index]);
                    object = (struct EffectObject *)context->object;
                    object->x =
                        (radius[index] * Func_08002322(heading[index])) >> 1;
                    object->z =
                        (radius[index] * Func_0800231c(heading[index])) >> 1;

                    if (frame <= 159) {
                        if (frame > index * 16 + 16)
                            spin[index] += 48;
                        if (radius[index] <= 31)
                            object->y += 0x60000;
                        else
                            object->y += 0x8000;
                        if (object->y > 0x7c0000)
                            object->y = 0x7c0000;
                    }

                    if (frame <= 490) {
                        heading[index] += spin[index];
                        if (heading[index] > 0x10000)
                            heading[index] -= 0x10000;
                        object->angle += spin[index] / 4;
                    }

                    if (frame == 395)
                        object->word_48 = 0xab85;

                    slot = 0;
                    do {
                        if (frame == Data_080ee16c[slot]) {
                            object->word_28 = 0;
                            Func_080d6888(
                                runtime->argument->target_ids[index],
                                7,
                                5,
                                index,
                                8);
                        }
                        slot++;
                    } while (slot != 4);

                    if (radius[index] > 16)
                        radius[index] -= 2;
                }
                index++;
            } while (index != runtime->argument->target_count);
        }

        if ((u32)(frame - 16) <= 143) {
            size = frame * 2 - 32;
            if (size > 48)
                size = 48;
            draw_rectangle(
                draw_destination,
                (u8 *)runtime + Func_080022fc(frame / 4, 3) * 3072
                    + (48 - size) * 48,
                32,
                112 - size,
                48,
                size);
        }

        if ((u32)(frame - 48) <= 111) {
            size = frame * 2 - 96;
            if (size > 64)
                size = 64;
            draw_rectangle(
                draw_destination,
                (u8 *)runtime + Func_080022fc(frame / 4, 3) * 3072
                    + (64 - size) * 48,
                32,
                64 - size,
                48,
                size);
        }

        if ((u32)(frame - 160) <= 239) {
            src = (u8 *)runtime + Func_080022fc(frame / 4, 3) * 3072;
            draw_rectangle(draw_destination, src, 8, 0, 48, 64);
            draw_rectangle(draw_destination, src, 8, 64, 48, 64);
        }

        if (frame <= 159) {
            point = runtime->points;
            index = 0;
            do {
                if (point->field_18 != 0) {
                    point->field_18--;
                } else {
                    work.x = point->field_08 * Func_08002322(point->field_10);
                    work.y = point->field_04;
                    work.z = point->field_08 * Func_0800231c(point->field_10);
                    Func_080e3944(&work, &screen);
                    screen.x >>= 1;
                    if (point->field_04 <= 0x3fffff) {
                        draw_rectangle(
                            draw_destination,
                            (u8 *)runtime + 0x2400,
                            screen.x - 12,
                            screen.y - 12,
                            16,
                            16);
                    }
                    if (point->field_08 > 24)
                        point->field_08 -= 4;
                    point->field_10 += point->field_0c * 2;
                    if (point->field_10 > 0x10000)
                        point->field_10 -= 0x10000;
                    point->field_0c += 50;
                    if (point->field_0c > 4096)
                        point->field_0c = 4096;
                    point->field_10 += 2048;
                    point->field_04 += point->field_10 * 2;
                    if (point->field_04 > 0x300000) {
                        point->field_04 = 0;
                        point->field_08 = 100;
                        point->field_10 = 0;
                        point->field_0c = 0;
                    }
                }
                index++;
                point++;
            } while (index != 24);
        }

        if (frame == 160) {
            Func_080b5040(1, (s32)Data_0000003b, 0);
            Func_080b5038(1, (s32)Data_0000003b, 8);
            Func_080d6750(runtime->argument);
            Func_080dbb24(9, 376, 2);

            tbl = Func_08002f40((s32)Data_00000088);
            ((WordCopy)0x03001388)((void *)0x05000000, tbl, 0x80);
            Func_08005340(tbl + 0x80, (u8 *)runtime + 0x3600);

            spark = Data_02010000;
            index = 0;
            do {
                spark->field_18 = 0;
                index++;
                spark++;
            } while (index != 512);

            index = 0;
            if (runtime->argument->target_count != 0) {
                do {
                    context = Func_080b5098(
                        runtime->argument->target_ids[index]);
                    object = (struct EffectObject *)context->object;
                    object->y = ((Func_08004458() & 15) + 8) << 16;
                    index++;
                } while (index != runtime->argument->target_count);
            }

            Data_03001ce0.field_0c = 72;

            point = runtime->points;
            index = 0;
            do {
                speed = Func_08004458() & 127;
                angle = (Func_08004458() & 0x7fff) + 0x9fff;
                point->field_00 = speed * Func_08002322(angle);
                point->field_04 = speed * Func_0800231c(angle);
                point->field_18 = 0;
                point->field_08 =
                    (Func_08002304(Func_08004458(), 200) - 100) << 16;
                index++;
                point++;
            } while (index != 32);

            index = 0;
            if (runtime->argument->target_count != 0) {
                do {
                    radius[index] = 16;
                    spin[index] = 6000;
                    index++;
                } while (index != runtime->argument->target_count);
            }
        }

        if (frame > 159) {
            scale = Data_080eda78;
            if (frame - 160 <= 64)
                size = 32;
            else
                size = 16;

            wave_x -= (Func_08002322((frame - 160) << 7) * 32) >> 6;
            wave_y += (size * Func_08002322((frame - 160) << 9)) >> 6;
            wave_x += drift_x;
            wave_y += drift_y;
            drift_x /= 2;
            drift_y /= 2;

            scale.x = 0x10000;
            scale.y = 0x10000;
            place.unknown_0c = 0;
            slot = 0;
            do {
                place.x = (Data_080ee15a[slot] << 16) + wave_x + 0x280000;
                place.y = 0xff0000 + slot * 0x400000;
                place.z =
                    ((Data_080ee163[slot] + slot * 64) << 16) + wave_y;
                Func_08009008(runtime->objects[slot], &place, &scale, 0);
                slot++;
            } while (slot != 9);

            if (frame > 255) {
                shake.z = 0x1000000;
                shake.x = 0;
                shake.y = 0;
                Func_080049ac();
                Func_08004cb4(&shake);

                alive = 0;
                if (frame <= 329) {
                    point = runtime->points;
                    index = 0;
                    do {
                        dist = ((IntegerSquareRoot)0x030001d8)(
                            (point->field_00 >> 8) * (point->field_00 >> 8)
                                + (point->field_04 >> 8)
                                    * (point->field_04 >> 8)
                                + (point->field_08 >> 8)
                                    * (point->field_08 >> 8)) >> 9;
                        if (dist == 0) {
                            alive++;
                        } else {
                            Func_080e3944(point, &screen);
                            screen.x =
                                (screen.x >> 17) + (wave_x >> 17) + 32;
                            screen.y = ((s16 *)&screen)[3]
                                + (wave_y >> 16) - 4;
                            screen.z = ((s16 *)&screen)[5];
                            if (screen.z <= 169)
                                screen.z = 170;
                            if (screen.z > 350)
                                screen.z = 350;
                            span = 6 - Func_080022ec(screen.z - 170, 36);
                            draw_rectangle_alt(
                                draw_destination,
                                graphics + Data_080ede48[span - 1],
                                screen.x - span / 2,
                                screen.y - span,
                                span,
                                span * 2);
                            point->field_00 -=
                                Func_080022ec(point->field_00, dist);
                            point->field_04 -=
                                Func_080022ec(point->field_04, dist);
                            point->field_08 -=
                                Func_080022ec(point->field_08, dist);
                        }
                        index++;
                        point++;
                    } while (index != 32);

                    if (alive > 0) {
                        span = Func_080022ec(alive, 10) + 1;
                        draw_rectangle_alt(
                            draw_destination,
                            graphics + Data_080ede48[span - 1],
                            (wave_x >> 17) - span / 2 + 32,
                            (wave_y >> 16) - span - 4,
                            span,
                            span * 2);
                    }
                }

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        spark = &Data_02010000[slot * 128];
                        index = 0;
                        do {
                            speed = (Func_08004458() & 127) + 16;
                            angle = Func_08004458() & 0xffff;
                            spark->field_0c =
                                (speed * Func_08002322(angle)) >> 6;
                            spark->field_10 =
                                -(speed * Func_0800231c(angle)) >> 6;
                            spark->field_10 =
                                ((Func_08004458() & 255) - 128) << 10;
                            spark->field_00 = 0;
                            spark->field_04 = 0;
                            spark->field_08 = 0;
                            spark->field_18 = (Func_08004458() & 15) + 64;
                            index++;
                            spark++;
                        } while (index != 96);
                    }

                    if (frame >= Data_080ee16c[0]) {
                        spark = Data_02010000;
                        index = 0;
                        do {
                            if (spark->field_18 > 0) {
                                Func_080e3944(spark, &screen);
                                screen.x = (screen.x >> 17) + 32;
                                screen.y = ((s16 *)&screen)[3] + 56;
                                screen.z = ((s16 *)&screen)[5];
                                if (screen.z <= 169)
                                    screen.z = 170;
                                if (screen.z > 350)
                                    screen.z = 350;
                                span =
                                    3 - Func_080022ec(screen.z - 170, 90);
                                draw_rectangle(
                                    draw_destination,
                                    graphics + Data_080ede48[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                spark->field_00 += spark->field_0c;
                                spark->field_04 += spark->field_10;
                                spark->field_08 += spark->field_10;
                                spark->field_18--;
                            }
                            index++;
                            spark++;
                        } while (index != 512);
                    }
                    slot++;
                } while (slot != 4);

                index = 0;
                do {
                    threshold = index * 12 + 330;
                    if (frame == threshold) {
                        runtime->points[index + 60].field_04 = (wave_y >> 16) - 4;
                        runtime->points[index + 60].field_10 = (wave_y >> 16) - 4;
                        runtime->points[index + 60].field_00 = (wave_x >> 17) + 32;
                        runtime->points[index + 60].field_0c = (wave_x >> 17) + 32;

                        spark = &runtime->points[32];
                        step = 0;
                        do {
                            spark->field_18 = 4;
                            step++;
                            spark++;
                        } while (step != 28);

                        runtime->impact_mode = 8;
                        drift_x = 0x80000;
                        drift_y = -0x20000;
                    }

                    if (frame >= threshold) {
                        size = (frame - threshold) / 2;
                        if (size > 2)
                            size = 2;
                        draw_rectangle_alt(
                            draw_destination,
                            (u8 *)runtime + Data_080ee17e[size],
                            runtime->points[index + 60].field_00,
                            runtime->points[index + 60].field_04 - Data_080ee17a[size],
                            Data_080ee174[size],
                            Data_080ee177[size]);
                        runtime->points[index + 60].field_00 -= 8;
                        runtime->points[index + 60].field_04 += 2;

                        if (frame < threshold + 8) {
                            Func_08004c6c(-2048);
                            Func_08004c1c(-4096);

                            angle = 0;
                            spark = &runtime->points[32];
                            step = 0;
                            do {
                                work.x = 0;
                                work.y = spark->field_18
                                    * Func_0800231c(angle);
                                work.z = spark->field_18
                                    * Func_08002322(angle);
                                spark->field_18 += 2;
                                Func_080e3944(&work, &screen);
                                screen.x =
                                    (screen.x >> 17) + runtime->points[index + 60].field_0c;
                                screen.y = ((s16 *)&screen)[3]
                                    + runtime->points[index + 60].field_10;
                                screen.z = ((s16 *)&screen)[5];
                                if (screen.z <= 169)
                                    screen.z = 170;
                                if (screen.z > 350)
                                    screen.z = 350;
                                span = 3
                                    - Func_080022ec(screen.z - 170, 90);
                                draw_rectangle(
                                    draw_destination,
                                    graphics + Data_080ede48[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                angle += 2340;
                                step++;
                                spark++;
                            } while (step != 28);
                        }
                    }
                    index++;
                } while (index != 4);
            }
        }

        Func_080cd52c();
        runtime->frame_ready = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 400);

    Func_080b50e8(134);

    index = 0;
    if (runtime->argument->target_count != 0) {
        do {
            context = Func_080b5098(runtime->argument->target_ids[index]);
            object = (struct EffectObject *)context->object;
            object->x = home[index].x;
            object->z = home[index].y;
            object->angle = base_angle[index];
            index++;
        } while (index != runtime->argument->target_count);
    }

    Data_03001ce0.field_0c = 120;

    Func_080d67dc();

    index = 0;
    do {
        Func_08009038(runtime->objects[index]);
        index++;
    } while (index != 9);

    Func_08004278(Func_080cd260);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
