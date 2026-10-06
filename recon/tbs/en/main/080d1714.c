/* 080d1714: differ 1184 of 1481 instructions (1491 generated, common
 * 894). BattleEffectWork, BattleEffectArgument, MotionObject and
 * EffectStep replace local partial layouts. The control flow already
 * follows the listing; what remains is register allocation and spill
 * slots: the frame matches at 356 bytes, but the native keeps index in
 * r9 and the inner step counters in r11 where the draft spills them, and
 * keeps frame at sp+80; it strength-reduces frame - threshold into its own slot and
 * hoists (wave_x >> 17) + 32 out of the threshold loop. Loops the native
 * addresses from a literal or runtime + constant base are written by
 * index; loops it walks with a pointer keep one.
 * The native writes the random z velocity over velocity_y and uses that
 * value for both y and z advancement; preserve this behavior.
 * Trials:
 * - baseline 1479; types 1475; approved IWRAM calls 1477.
 * - AudioCommand_PlayFar, the listing's callee, for the cues: 1477.
 * - threshold as index * 12 + 330, no biv: 1490.
 * - mark pointer for particles[60 + index] outside the inner loop: 1486
 *   (frame 352); also inside it: 1473 (frame 336).
 * - if (frame - threshold >= 0): 1494.
 * - particles[step + 32].variant = 4 indexed, as the native's
 *   runtime + 0x7418 shows: 1471.
 * - map cell buffer cleared by index, the native's literal base: 1404.
 * - first particle loop by index: 1348; burst setup loop by index: 1338;
 *   distance loop by index: 1324; spark spawn loop by index: 1320.
 * - first-phase particle loop by index: 1581; spark update loop by
 *   index: 1370 (both kept as pointers).
 * - the inner spark loop of the threshold loop reads the mark position
 *   as runtime->particles[index + 60], which the native addresses from
 *   runtime plus an index * 28 offset rather than the mark pointer:
 *   1191, frame 356 (was 340).
 * - the 64-step init loop by pointer with its own counter: 1249 (base
 *   0x7080 as native, but 120 and 63 hoisted into registers); point =
 *   &runtime->particles[index] set at the top of each iteration: 1184,
 *   the loop matches but for index on the stack. */
#include "CANVAS.H"
#include "MOTION_OBJECT.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "PROJECT.H"
#include "B5_CONTEXT.H"
#include "BATTLE_TYPES.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"


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

extern void *gBattleFxWork[];

extern u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
extern const struct ScalePair Data_080eda78;
extern u8 Data_080ee15a[];
extern u8 Data_080ee163[];
extern u16 Data_080ee16c[];
extern u8 Data_080ee174[];
extern u8 Data_080ee177[];
extern u8 Data_080ee17a[];
extern u16 Data_080ee17e[];

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 interval);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
s32 Random16(void);
s32 ArcTan2(s32 x, s32 z);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);
s32 __umodsi3(s32 numerator, s32 denominator);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *source, void *destination);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyRoll(s32 pitch);
void SceneTransform_ApplyPosition(struct Vector3 *position);
void AudioCommand_PlayFar(s32 cue);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Object_ApplyProjectedPlacementFar(void *handle, struct Placement *place, struct ScalePair *scale,
    s32 mode);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 count);

void Unnamed_080d1714(struct BattleEffectArgument *argument)
{
    void **cells;
    void *draw_destination;
    void *view;
    struct BattleEffectWork *runtime;
    DrawRectangle draw_rectangle_alt;
    DrawRectangle draw_rectangle;
    s32 frame;
    u8 *graphics;
    s32 wave_y;
    s32 wave_x;
    s32 drift_y;
    s32 drift_x;
    struct MotionObject *object;
    struct BattleObjectSlot *context;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *mark;
    u8 *tbl;
    u8 *src;
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
    s32 heading[8];
    s32 radius[8];
    s32 spin[8];
    struct ScalePair home[8];
    s32 base_angle[8];
    struct Vector3 work;
    struct EffectPosition screen;
    struct Vector3 shake;
    struct Placement place;
    struct ScalePair scale;

    cells = gBattleFxWork;
    draw_destination = cells[0];
    view = *(void **)((u8 *)cells - 112);
    runtime = cells[-1];
    graphics = (u8 *)cells[1];
    runtime->effect = argument;

    BattleFx_BeginCanvasLayer(0);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_rectangle = (DrawRectangle)cells[6];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw_rectangle_alt = (DrawRectangle)cells[7];

    tbl = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
    Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
    tbl += 0x80;
    Resource_DecodeType01(tbl, runtime);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

    runtime->transfer_mode = 2;
    runtime->transfer_value = 50;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    wave_x = 0x800000;
    wave_y = 0x280000;
    drift_x = 0;
    drift_y = 0;

    index = 0;
    if (runtime->effect->count != 0) {
        do {
            context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
            object = (struct MotionObject *)context->object;
            home[index].x = object->x;
            home[index].y = object->z;
            base_angle[index] = object->angle;
            heading[index] = (u16)ArcTan2(object->x, object->z);
            radius[index] = Iwram_Sqrt(
                (object->x >> 8) * (object->x >> 8)
                    + (object->z >> 8) * (object->z >> 8)) >> 7;
            spin[index] = 0;
            object->vertical_motion_strength = 0;
            index++;
        } while (index != runtime->effect->count);
    }

    index = 0;
    do {
        point = &runtime->particles[index];
        point->z = 120;
        point->y = 0;
        Random16();
        point->velocity_y = 0;
        point->velocity_x = 0;
        point->variant = Random16() & 63;
        index++;
    } while (index != 64);

    frame = 0;
    do {
        if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
            if (frame <= 159)
                frame = 160;
            else if (frame <= 394)
                frame = 395;
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(view, (u8 *)view + 12);

        if (frame == 16)
            AudioCommand_PlayFar(141);
        if (frame == 256)
            AudioCommand_PlayFar(140);
        if (frame == 334)
            AudioCommand_PlayFar(212);
        if (frame == 347)
            AudioCommand_PlayFar(212);
        if (frame == 359)
            AudioCommand_PlayFar(212);
        if (frame == 372)
            AudioCommand_PlayFar(212);

        index = 0;
        if (runtime->effect->count != 0) {
            do {
                if (frame > index * 16) {
                    context = GetBattleObjectSlotFar(
                        runtime->effect->actors[index]);
                    object = (struct MotionObject *)context->object;
                    object->x =
                        (radius[index] * Trig_Sin(heading[index])) >> 1;
                    object->z =
                        (radius[index] * Trig_Cos(heading[index])) >> 1;

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
                        object->vertical_motion_strength = 0xab85;

                    slot = 0;
                    do {
                        if (frame == Data_080ee16c[slot]) {
                            object->velocity_y = 0;
                            ObjectGroup_UpdateMembers(
                                runtime->effect->actors[index],
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
            } while (index != runtime->effect->count);
        }

        if ((u32)(frame - 16) <= 143) {
            size = frame * 2 - 32;
            if (size > 48)
                size = 48;
            draw_rectangle(
                draw_destination,
                (u8 *)runtime + __modsi3(frame / 4, 3) * 3072
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
                (u8 *)runtime + __modsi3(frame / 4, 3) * 3072
                    + (64 - size) * 48,
                32,
                64 - size,
                48,
                size);
        }

        if ((u32)(frame - 160) <= 239) {
            src = (u8 *)runtime + __modsi3(frame / 4, 3) * 3072;
            draw_rectangle(draw_destination, src, 8, 0, 48, 64);
            draw_rectangle(draw_destination, src, 8, 64, 48, 64);
        }

        if (frame <= 159) {
            point = runtime->particles;
            index = 0;
            do {
                if (point->variant == 0) {
                    work.x = point->z * Trig_Sin(point->velocity_y);
                    work.y = point->y;
                    work.z = point->z * Trig_Cos(point->velocity_y);
                    EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                    screen.x >>= 1;
                    if (point->y <= 0x3fffff) {
                        draw_rectangle(
                            draw_destination,
                            (u8 *)runtime + 0x2400,
                            screen.x - 12,
                            screen.y - 12,
                            16,
                            16);
                    }
                    if (point->z > 24)
                        point->z -= 4;
                    point->velocity_y += point->velocity_x * 2;
                    if (point->velocity_y > 0x10000)
                        point->velocity_y -= 0x10000;
                    point->velocity_x += 50;
                    if (point->velocity_x > 4096)
                        point->velocity_x = 4096;
                    point->velocity_y += 2048;
                    point->y += point->velocity_y * 2;
                    if (point->y > 0x300000) {
                        point->y = 0;
                        point->z = 100;
                        point->velocity_y = 0;
                        point->velocity_x = 0;
                    }
                } else {
                    point->variant--;
                }
                index++;
                point++;
            } while (index != 24);
        }

        if (frame == 160) {
            BattleBackground_LoadFar(1, 0x3b, 0);
            BattlePresentation_ConfigurePaletteFadeFar(1, 0x3b, 8);
            BattleFx_SelectLivingTargets(runtime->effect);
            BattleFx_SpawnObjects(9, 376, 2);

            tbl = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
            Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
            tbl += 0x80;
            Resource_DecodeType01(tbl, (u8 *)runtime + 0x3600);

            index = 0;
            do {
                ((struct EffectStep *)Ram_MapCellBuffer)[index].variant = 0;
                index++;
            } while (index != 512);

            index = 0;
            if (runtime->effect->count != 0) {
                do {
                    context = GetBattleObjectSlotFar(
                        runtime->effect->actors[index]);
                    object = (struct MotionObject *)context->object;
                    object->y = ((Random16() & 15) + 8) << 16;
                    index++;
                } while (index != runtime->effect->count);
            }

            gProjection.center_x = 72;

            index = 0;
            do {
                speed = Random16() & 127;
                angle = (Random16() & 0x7fff) + 0x9fff;
                runtime->particles[index].x = speed * Trig_Sin(angle);
                runtime->particles[index].y = speed * Trig_Cos(angle);
                runtime->particles[index].variant = 0;
                runtime->particles[index].z =
                    (__umodsi3(Random16(), 200) - 100) << 16;
                index++;
            } while (index != 32);

            index = 0;
            if (runtime->effect->count != 0) {
                do {
                    radius[index] = 16;
                    spin[index] = 6000;
                    index++;
                } while (index != runtime->effect->count);
            }
        }

        if (frame > 159) {
            scale = Data_080eda78;
            size = 16;
            if (frame - 160 <= 64)
                size = 32;

            wave_x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
            wave_y += (size * Trig_Sin((frame - 160) << 9)) >> 6;
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
                Object_ApplyProjectedPlacementFar(runtime->objects[slot], &place, &scale, 0);
                slot++;
            } while (slot != 9);

            if (frame > 255) {
                shake.z = 0x1000000;
                shake.x = 0;
                shake.y = 0;
                Render_ResetTransformState();
                SceneTransform_ApplyPosition(&shake);

                alive = 0;
                if (frame <= 329) {
                    index = 0;
                    do {
                        dist = Iwram_Sqrt(
                            (runtime->particles[index].x >> 8) * (runtime->particles[index].x >> 8)
                                + (runtime->particles[index].y >> 8)
                                    * (runtime->particles[index].y >> 8)
                                + (runtime->particles[index].z >> 8)
                                    * (runtime->particles[index].z >> 8)) >> 9;
                        if (dist != 0) {
                            EffectPosition_ApplyBaseAndYOffset(&runtime->particles[index].x, &screen);
                            screen.x =
                                (screen.x >> 17) + (wave_x >> 17) + 32;
                            screen.y = ((s16 *)&screen)[3]
                                + (wave_y >> 16) - 4;
                            screen.depth = ((s16 *)&screen)[5];
                            if (screen.depth <= 169)
                                screen.depth = 170;
                            if (screen.depth > 350)
                                screen.depth = 350;
                            span = 6 - __divsi3(screen.depth - 170, 36);
                            draw_rectangle_alt(
                                draw_destination,
                                graphics + ParticleStreams_CellOffsets[span - 1],
                                screen.x - span / 2,
                                screen.y - span,
                                span,
                                span * 2);
                            runtime->particles[index].x -=
                                __divsi3(runtime->particles[index].x, dist);
                            runtime->particles[index].y -=
                                __divsi3(runtime->particles[index].y, dist);
                            runtime->particles[index].z -=
                                __divsi3(runtime->particles[index].z, dist);
                        } else {
                            alive++;
                        }
                        index++;
                    } while (index != 32);

                    if (alive > 0) {
                        span = __divsi3(alive, 10) + 1;
                        draw_rectangle_alt(
                            draw_destination,
                            graphics + ParticleStreams_CellOffsets[span - 1],
                            (wave_x >> 17) - span / 2 + 32,
                            (wave_y >> 16) - span - 4,
                            span,
                            span * 2);
                    }
                }

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        index = 0;
                        do {
                            speed = (Random16() & 127) + 16;
                            angle = Random16() & 0xffff;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].velocity_x =
                                (Trig_Sin(angle) * speed) >> 6;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].velocity_y =
                                -(Trig_Cos(angle) * speed) >> 6;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].velocity_y =
                                ((Random16() & 255) - 128) << 10;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].x = 0;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].y = 0;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].z = 0;
                            ((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index].variant = (Random16() & 15) + 64;
                            index++;
                        } while (index != 96);
                    }

                    if (frame >= Data_080ee16c[0]) {
                        spark = ((struct EffectStep *)Ram_MapCellBuffer);
                        index = 0;
                        do {
                            if (spark->variant > 0) {
                                EffectPosition_ApplyBaseAndYOffset(&spark->x, &screen);
                                screen.x = (screen.x >> 17) + 32;
                                screen.y = ((s16 *)&screen)[3] + 56;
                                screen.depth = ((s16 *)&screen)[5];
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span =
                                    3 - __divsi3(screen.depth - 170, 90);
                                draw_rectangle(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                spark->x += spark->velocity_x;
                                spark->y += spark->velocity_y;
                                spark->z += spark->velocity_y;
                                spark->variant--;
                            }
                            index++;
                            spark++;
                        } while (index != 512);
                    }
                    slot++;
                } while (slot != 4);

                mark = &runtime->particles[60];
                index = 0;
                threshold = 330;
                do {
                    if (frame == threshold) {
                        mark->y = (wave_y >> 16) - 4;
                        mark->velocity_y = (wave_y >> 16) - 4;
                        mark->x = (wave_x >> 17) + 32;
                        mark->velocity_x = (wave_x >> 17) + 32;

                        step = 0;
                        do {
                            runtime->particles[step + 32].variant = 4;
                            step++;
                        } while (step != 28);

                        runtime->shake_frames = 8;
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
                            mark->x,
                            mark->y - Data_080ee17a[size],
                            Data_080ee174[size],
                            Data_080ee177[size]);
                        mark->x -= 8;
                        mark->y += 2;

                        if (frame < threshold + 8) {
                            SceneTransform_ApplyRoll(-2048);
                            SceneTransform_ApplyYaw(-4096);

                            angle = 0;
                            spark = &runtime->particles[32];
                            step = 0;
                            do {
                                work.x = 0;
                                work.y = spark->variant
                                    * Trig_Cos(angle);
                                work.z = spark->variant
                                    * Trig_Sin(angle);
                                spark->variant += 2;
                                EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                                screen.x =
                                    (screen.x >> 17) + runtime->particles[index + 60].velocity_x;
                                screen.y = ((s16 *)&screen)[3]
                                    + runtime->particles[index + 60].velocity_y;
                                screen.depth = ((s16 *)&screen)[5];
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span = 3
                                    - __divsi3(screen.depth - 170, 90);
                                draw_rectangle(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
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
                    mark++;
                    threshold += 12;
                } while (index != 4);
            }
        }

        ObjectGroup_TickMemberTimers();
        runtime->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 400);

    BattleEventRuntime_BeginPhaseFar(134);

    index = 0;
    if (runtime->effect->count != 0) {
        do {
            context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
            object = (struct MotionObject *)context->object;
            object->x = home[index].x;
            object->z = home[index].y;
            object->angle = base_angle[index];
            index++;
        } while (index != runtime->effect->count);
    }

    gProjection.center_x = 120;

    BattleEffect_SetupBlendedDisplay();

    index = 0;
    do {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)runtime->objects[index]);
        index++;
    } while (index != 9);

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
