/* Draft, not exact. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];
extern u8 gBattleFxWork[];
extern s32 gCameraWork[];
extern volatile u32 gKeysRepeat;

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 channel, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 channel, s32 x, s32 y);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void *GetMotionRecordFar(struct MotionObject *object, s32 index);
void Object_InitializeMode(void *object, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32 x, s32 y);

extern u8 Data_080ee910[];
extern u16 Data_080ee916[];
extern u8 Data_080ee920[];
extern u8 Data_080ee925[];
extern u16 Data_080ee92a[];
extern u8 Data_080ee930[];
extern u16 Data_080ee934[];
extern u8 Data_080ee93e[];
extern u8 Data_080ee943[];
extern u16 Data_080ee948[];
extern u8 Data_080ee952[];
extern u16 Data_080ee958[];
extern u16 Data_080ee966[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* The eight objects the effect spawns, kept in the work block. */
#define WORK_OBJECTS(work) ((void **)((u8 *)(work) + 0x77d8))

/* Battle effect: vines grow up through the ground under the enemy side,
   thorns scatter, the picture dissolves through a shuffled dither, and
   rings burst on every affected unit. */
void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 height;
    s32 speed;
    s32 phase;
    s32 rise;
    s32 i;
    s32 j;
    s32 point[3];
    struct EffectPosition screen;
    DrawRectangle draw[2];

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_SpawnObjects(8, 0x177, 1);

    for (i = 0; i != 1024; i++)
        Ram_MapCellBuffer[i] = i & 127;
    for (i = 0; i != 8; i++) {
        for (j = 0; j != 128; j++) {
            s32 a = Random16() & 127;
            s32 b = Random16() & 127;
            u8 t = Ram_MapCellBuffer[i * 128 + b];

            Ram_MapCellBuffer[i * 128 + b] = Ram_MapCellBuffer[i * 128 + a];
            Ram_MapCellBuffer[i * 128 + a] = t;
        }
    }

    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)gWorkSlot[47];
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    REG_BG2X = -0x2000;

    for (i = 0; i != 16; i++) {
        struct EffectStep *vine = &work->particles[i];

        if (i <= 4) {
            vine->x = i * 20;
            vine->y = (Random16() & 7) + 104;
        } else {
            vine->x = i * 20 - 90;
            vine->y = (Random16() & 7) + 108;
        }
        vine->velocity_y = (Random16() & 7) + 4;
        vine->variant = (Random16() & 15) + 16;
    }
    for (i = 0; i != 16; i++) {
        struct EffectStep *spark = &work->particles[16 + i];

        spark->x = (Random16() & 63) + 32;
        spark->y = (Random16() & 31) + 64;
        spark->variant = -(Random16() & 7) - 8;
    }
    for (i = 0; i != 16; i++) {
        struct EffectStep *thorn = &work->particles[32 + i];

        thorn->x = 0x800000;
        thorn->y = 0x400000;
        thorn->velocity_x = -((Random16() & 255) + 200) << 9;
        thorn->velocity_y = 0;
        thorn->variant = 0;
    }
    BattleFx_SelectLivingTargets(work->effect);

    height = -0x400000;
    speed = 0;
    for (frame = 0; frame != 366; frame++) {
        s32 facing = gCameraWork[0];

        if ((gKeysRepeat & 3) != 0 && frame > 190 && frame <= 285) {
            Iwram_ClearWords(canvas, 0x4000);
            frame = 286;
        }
        if (frame == 224)
            work->transfer_mode = 0;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame == 31) {
            work->shake_frames = 8;
            Audio_PlayCue(157);
            for (i = 0; i != work->effect->count; i++)
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
        }
        if (frame == 72)
            Audio_PlayCue(136);
        if (frame == 140)
            Audio_PlayCue(156);
        speed += 0x4000;
        height += speed;
        if (height > 0x400000)
            height = 0x400000;
        BattleFx_PlaceFormationObjects(2, 0x800000, height);
        if (frame >= 48 && frame <= 96) {
            s32 pose = (frame - 48) / 24 % 3;

            Object_InitializeMode(WORK_OBJECTS(work)[3], Data_080ee910[pose * 2]);
            Object_InitializeMode(WORK_OBJECTS(work)[4], Data_080ee910[pose * 2 + 1]);
        }
        if (frame >= 72 && frame <= 127) {
            for (i = 0; i != 16; i++) {
                struct EffectStep *thorn = &work->particles[32 + i];

                if (frame >= i + 72 && thorn->y <= 0x67ffff) {
                    s32 cel = (frame + i) / 4 % 5;
                    u32 width;
                    u32 tall;

                    draw[0](canvas, (u8 *)work + Data_080ee916[cel],
                        HI(thorn->x) - ((width = Data_080ee920[cel]) >> 1),
                        (thorn->y >> 16) - ((tall = Data_080ee925[cel]) >> 1),
                        width, tall);
                    EffectStep_AdvanceWithGravity2D(thorn, 64, 0x1000);
                }
            }
        }
        if (frame == 128) {
            for (i = 0; i != 48; i++) {
                struct EffectStep *chip = &work->particles[16 + i];

                chip->x = (Random16() % 96) << 16;
                chip->y = ((Random16() & 7) + 88) << 16;
                chip->velocity_x = ((Random16() & 255) - 128) << 11;
                chip->velocity_y = -(Random16() & 255) << 11;
                chip->variant = -(Random16() & 15) - 16;
            }
            REG_BG2X = 0;
        }

        phase = frame - 128;
        if (phase >= 0 && phase <= 96) {
            rise = phase;
            if (rise > 80)
                rise = 80;
            else
                work->shake_frames = 2;
            for (i = 0; i != 10; i++) {
                struct EffectStep *vine = &work->particles[i];

                if (rise > vine->variant) {
                    u8 *sheet = (u8 *)work;
                    s32 top;
                    s32 wrap;

                    if (i > 5)
                        sheet = (u8 *)work + 0x6c0;
                    top = (rise - vine->variant) * vine->velocity_y;
                    for (wrap = top; wrap > 184; wrap -= 64)
                        ;
                    if (wrap <= 119)
                        draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
                    for (j = 0; j != 3; j++) {
                        s32 y = vine->y - wrap + j * 64;
                        s32 skip = 0;
                        s32 rows = 64;

                        if (y >= -64) {
                            if (y < 0) {
                                skip = -y * 24;
                                rows = y + 64;
                                y = 0;
                            }
                            if (y + rows > vine->y)
                                rows -= y + rows - vine->y;
                            draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
                        }
                    }
                    if ((i & 1) != 0) {
                        s32 y = ((vine->y - top) & 127) - 16;
                        s32 kind = i % 3;
                        s32 full = Data_080ee930[kind];
                        s32 rows = full;

                        if (y + rows > vine->y)
                            rows -= y + rows - vine->y;
                        if (rows > 0)
                            draw[i & 1](canvas, (u8 *)work + Data_080ee92a[kind],
                                vine->x + 8, y, full, rows);
                    }
                }
            }
        }
        if (phase >= 0 && phase <= 95) {
            for (i = 0; i != 32; i++) {
                struct EffectStep *chip = &work->particles[32 + i];

                if (chip->variant >= 0) {
                    s32 cel = i % 5;

                    draw[0](canvas, (u8 *)work + Data_080ee934[cel],
                        HI(chip->x), HI(chip->y), Data_080ee93e[cel], Data_080ee943[cel]);
                    chip->x += chip->velocity_x;
                    chip->y += chip->velocity_y;
                    chip->velocity_y += 0x4000;
                    if ((u32)chip->y > 0x780000 && frame <= 159) {
                        chip->x = (Random16() % 96) << 16;
                        chip->y = ((Random16() & 7) + 88) << 16;
                        chip->velocity_x = ((Random16() & 255) - 128) << 11;
                        chip->velocity_y = -(Random16() & 255) << 11;
                    }
                }
                chip->variant++;
            }
        }

        if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
            u16 *color = (u16 *)BG_PLTT;

            for (i = 0; i != 64; i++) {
                s32 red = *color & 31;
                s32 green = (*color >> 5) & 31;
                s32 blue = (*color >> 10) & 31;
                s32 grey = (red + green + blue) / 3;

                if (red > grey)
                    red--;
                if (red < grey)
                    red++;
                if (green > grey)
                    green--;
                if (green < grey)
                    green++;
                if (blue > grey)
                    blue--;
                if (blue < grey)
                    blue++;
                *color++ = (blue << 10) | (green << 5) | red;
            }
        }

        if (phase >= 0 && phase <= 172) {
            for (i = 0; i != work->effect->count; i++) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                point[0] = object->x;
                point[1] = object->y;
                point[2] = object->z;
                EffectPosition_ApplyBaseAndYOffset(point, &screen);
                for (j = 0; j != 6; j++) {
                    struct EffectStep *spark = &work->particles[16 + i * 6 + j];

                    if (spark->variant == 0) {
                        spark->x = screen.x + (Random16() & 15) - 8;
                        spark->y = screen.y + (Random16() & 15) - 40;
                    }
                    if ((u32)spark->variant <= 4) {
                        u8 *cell = (u8 *)work + Data_080ee948[spark->variant];
                        u32 size = Data_080ee952[spark->variant];
                        u32 half = size >> 1;

                        draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
                    }
                    spark->variant++;
                    if (frame <= 199 && spark->variant == 5)
                        spark->variant = -(Random16() & 7);
                }
            }
        }

        if (frame > 232) {
            s32 row = frame * 2 - 496;
            s32 line;

            line = row;
            for (i = 0; i != 32; i++) {
                for (j = 0; j != 4; j++) {
                    if ((u32)line <= 127) {
                        s32 x = Ram_MapCellBuffer[((line & 7) * 32 + i) * 4 + j];

                        ((u8 *)canvas)[((line / 8 * 16 + x / 8) * 8 + (line & 7)) * 8
                            + (x & 7)] = 0;
                    }
                }
                line++;
            }
            line = row + 1;
            for (i = 0; i != 32; i++) {
                for (j = 0; j != 4; j++) {
                    if ((u32)line <= 127) {
                        s32 x = Ram_MapCellBuffer[((line & 7) * 32 + i) * 4 + j];

                        ((u8 *)canvas)[((line / 8 * 16 + x / 8) * 8 + (line & 7)) * 8
                            + (x & 7)] = 0;
                    }
                }
                line++;
            }
        }

        if (frame >= 161 && frame <= 223) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame > i * 8 + 160) {
                    struct BattleObjectSlot *slot =
                        GetBattleObjectSlotFar(work->effect->actors[i]);
                    void *record;

                    slot->object->y += 0x80000;
                    if (slot->object->y > 0x800000)
                        slot->object->y = 0x800000;
                    slot->object->vertical_motion_strength = 0;
                    for (j = 0; (record = GetMotionRecordFar(slot->object, j)) != 0; j++)
                        Object_InitializeMode(record, 5);
                }
            }
        }
        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 5 + 286) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                object->y = 0x600000;
                object->vertical_motion_strength = 0xab85;
            }
            if (frame == i * 5 + 302) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
                Audio_PlayCue(134);
                work->shake_frames = 8;
            }
        }

        if (frame == 302) {
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
            Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work, 1, 0);
            Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, (u8 *)work + 0x1680, 1, 1);
            BattleEffect_LoadWork(46, 7, 7, 3, 2);
            draw[0] = (DrawRectangle)gWorkSlot[46];
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            draw[1] = (DrawRectangle)gWorkSlot[47];
            REG_BLDCNT = 0x3f46;
            REG_BG2PA = 0x80;
            REG_BG2X = 0;
            work->transfer_mode = 2;
            work->transfer_value = 75;
            Scheduler_AddOrUpdateCallback(
                (s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
            for (i = 0; i != work->effect->count; i++) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                for (j = 0; j != 10; j++) {
                    struct EffectStep *ember = &work->particles[i * 10 + j];

                    ember->x = object->x;
                    ember->y = 0x140000;
                    ember->z = object->z;
                    ember->velocity_x = Trig_Sin(j * 0x3334) << 2;
                    ember->velocity_y = (Random16() & 0x7fff) + 0x10000;
                    ember->velocity_z = Trig_Cos(j * 0x3334) << 2;
                    ember->variant = 0;
                }
            }
        }
        if (frame > 301) {
            for (i = 0; i != work->effect->count; i++) {
                s32 start = i * 4 + 302;

                if (frame >= start && frame < i * 4 + 314) {
                    s32 cel = (frame - start) / 2;
                    struct MotionObject *object =
                        GetBattleObjectSlotFar(work->effect->actors[i])->object;

                    point[0] = object->x;
                    point[1] = 0;
                    point[2] = object->z;
                    EffectPosition_ApplyBaseAndYOffset(point, &screen);
                    screen.x >>= 1;
                    draw[0](canvas, (u8 *)work + cel * 480,
                        screen.x - 20, screen.y - 24, 20, 24);
                    draw[1](canvas, (u8 *)work + cel * 480,
                        screen.x, screen.y - 24, 20, 24);
                }
                if (frame >= start + 6) {

                    for (j = 0; j != 5; j++) {
                        struct EffectStep *ember = &work->particles[i * 10 + j];

                        EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
                        screen.x >>= 1;
                        if ((u32)ember->variant <= 26) {
                            s32 cel = 6;
                            u32 size = Data_080ee966[cel];

                            draw[0](canvas, (u8 *)work + Data_080ee958[cel],
                                screen.x - (size >> 1), screen.y - (size >> 1), size, size);
                        }
                        EffectStep_AdvanceWithGravity2D(ember, 60, 0x1000);
                        ember->variant++;
                    }
                }
            }
        }

        if (frame <= 127)
            Camera_ApplyShake(4, 16);
        else if (frame <= 301)
            Camera_ApplyShake(2, 2);
        else
            Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleEventRuntime_BeginPhaseFar(134);
    BattleEffect_RunImpactBurst(2, 0x800000, height);
    for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar(WORK_OBJECTS(work)[i]);
    BattleFx_EndCanvasLayer();
}
