#include "TYPES.H"
#include "RESOURCE.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "RAM_BUFFER.H"

struct Point2D {
    s32 x;
    s32 y;
};

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];
extern volatile u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
/* Three x and y pairs: where each ember column stands. */
extern u8 EmberColumns_Columns[];
/* By ember index modulo four: how fast it is pulled down. */
extern s32 EmberColumns_Gravity[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
u32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
struct BattleEffectTargetArgument;
void BattleFx_SelectLivingTargets(struct BattleEffectTargetArgument *argument);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void BattleFx_PlaceFormationObjects(s32 channel, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 channel, s32 x, s32 y);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
s32 BattleFx_EndCanvasLayer(void);

extern u8 gBattleFxWork[];
/* By drop index modulo four: how fast a drop leaves the ground. */
extern u8 PaletteParticles_DropSpeeds[];
/* The spark's two pixels. */
extern u8 PaletteParticles_SparkPixels[];
/* By drop index modulo four: which particle picture it is drawn with. */
extern u8 PaletteParticles_DropSizes[];
/* The streak's six pictures: width, height, how far below the streak each
   is drawn and where it starts in the sheet. */
extern u8 PaletteParticles_StreakWidths[];
extern u8 PaletteParticles_StreakHeights[];
extern u8 PaletteParticles_StreakDrops[];
extern u16 PaletteParticles_StreakOffsets[];

#define gDrops ((struct EffectStep *)Ram_MapCellBuffer)

void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Object_InitializeMode(void *object, s32 mode);
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);

typedef s32 (*WordCopy)(void *, const void *, s32);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the palette copies share the routine's address but build
       the palette address again at each call, which only an inlined constant
       argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

/*
 * Sixteen streaks come down in steps and land; drops rise off the ground,
 * one a frame and then eight, sparks fly from the focus on odd frames, and
 * at the end bursts open there while the canvas brightens and fades. A held
 * button skips the middle. Mode 0 plays it on the formation objects it
 * spawns and ends in the impact burst; mode 1 plays it over the actor in
 * the lightning palette, mirrored for the far side.
 */
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition origin;
    void **cursor;
    void *canvas;
    struct BattleCamera *camera;
    struct BattleEffectWork *work;
    struct BlitterPair draw;
    s32 frame;
    u8 *sheet;
    struct Point2D focus;
    s32 direction;
    u8 *palette;
    s32 i;

    cursor = (void **)(gBattleFxWork + 4);
    canvas = cursor[0];
    camera = cursor[-28];
    work = cursor[-1];
    sheet = cursor[1];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x0400000c = 0x784;
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw.upper = cursor[6];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw.lower = cursor[7];
    palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x3000);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
    if (mode == 1)
        CopyPalette(Iwram_CopyWords, (void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (mode == 0)
        direction = 1;
    else if (work->effect->side == 1)
        direction = -1;
    else
        direction = 1;
    if (mode == 1) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &origin);
        origin.x /= 2;
        origin.y = 66;
        if (work->effect->side == 1)
            origin.x = 76;
        else
            origin.x = 44;
    }
    focus.x = 0xd40000;
    focus.y = -0x3c0000;
    for (i = 0; i != 64; i++)
        work->particles[i].variant = -1;
    for (i = 0; i != 16; i++) {
        struct EffectStep *streak = &work->particles[24 + i];

        if (direction == 1)
            streak->x = (Random16() & 127) + 128;
        else
            streak->x = (Random16() & 127) - 128;
        streak->y = (Random16() & 7) - 72;
        streak->variant = -(s32)(Random16() & 31);
    }
    for (i = 0; i != 512; i++)
        gDrops[i].variant = -1;
    if (mode == 0) {
        BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);
        WaitFrames(1);
        BattleFx_SpawnObjects(8, 377, 2);
    }

    for (frame = 0; frame != 208; frame++) {
        if ((gKeysRepeat & 3) && frame > 48 && frame <= 159) {
            if (mode == 0) {
                Object_InitializeMode(work->objects[0], 8);
                Object_InitializeMode(work->objects[1], 9);
                Object_InitializeMode(work->objects[3], 10);
                Object_InitializeMode(work->objects[4], 11);
            }
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
            }
            frame = 160;
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame == 178)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 128)
            work->transfer_value = 50;
        if (frame == 176) {
            work->transfer_mode = 3;
            work->transfer_value = 0x02020202;
            palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
            Iwram_CopyWords((void *)0x05000000, palette, 128);
        } else if ((u32)(frame - 160) <= 15) {
            work->transfer_mode = 1;
            work->transfer_value = 0x10101010;
            if (frame > 173)
                work->transfer_value = 0x3f3f3f3f;
            Palette_BrightenBgEntries(2, 2, 2);
        }
        if ((u32)(frame - 33) <= 142) {
            s32 spawned = 0;
            s32 limit = 1;

            if (frame > 103)
                limit = 8;
            for (i = 0; i != 512; i++) {
                struct EffectStep *drop = &gDrops[i];

                if (drop->variant == -1) {
                    drop->x = ((Random16() & 255) - 32) << 16;
                    drop->y = 0x700000;
                    drop->velocity_x = ((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3]) << 9;
                    drop->velocity_y = -((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3]) << 11;
                    drop->variant = 0;
                    spawned++;
                    if (spawned == limit)
                        break;
                }
            }
        }
        if ((u32)(frame - 41) <= 86) {
            s32 spawned = 0;

            if (frame & 1) {
                for (i = 0; i != 24; i++) {
                    struct EffectStep *spark = &work->particles[40 + i];

                    if (spark->variant == -1) {
                        s32 speed = (Random16() & 255) + 128;
                        s32 angle = (Random16() & 0x1fff) + 20000;

                        if (mode == 0) {
                            spark->x = ((Random16() & 7) + 78) << 16;
                            spark->y = 0x460000;
                        } else {
                            spark->x = ((Random16() & 7) + origin.x - 8) << 16;
                            spark->y = origin.y << 16;
                        }
                        spark->velocity_x = speed * Trig_Sin(angle) >> 9;
                        spark->velocity_y = speed * Trig_Cos(angle) >> 9;
                        spawned++;
                        spark->variant = 0;
                        if (spawned == 1)
                            break;
                    }
                }
            }
        }
        if (frame == 48)
            AudioCommand_PlayFar(141);
        if (frame == 128)
            AudioCommand_PlayFar(145);
        if ((u32)(frame - 129) <= 46) {
            s32 spawned = 0;

            for (i = 0; i != 24; i++) {
                struct EffectStep *burst = &work->particles[i];

                if (burst->variant == -1) {
                    s32 speed = (Random16() & 255) + 128;
                    s32 angle = (Random16() & 0x1fff) - 20000;

                    if (mode == 0) {
                        burst->x = 0x440000;
                        burst->y = 0x400000;
                    } else {
                        burst->x = origin.x << 16;
                        burst->y = origin.y << 16;
                    }
                    burst->velocity_x = speed * Trig_Sin(angle) >> 6;
                    burst->velocity_y = speed * Trig_Cos(angle) >> 6;
                    burst->variant = 0;
                    spawned++;
                    if (spawned == 1)
                        break;
                }
            }
        }
        if (frame <= 175) {
            for (i = 0; i != 24; i++) {
                struct EffectStep *burst = &work->particles[i];

                if (burst->variant >= 0) {
                    draw.upper(canvas, work->sheet + ((burst->variant >> 2) << 11),
                        ((s16 *)&burst->x)[1] - 16, ((s16 *)&burst->y)[1] - 32, 32, 64);
                    burst->x += burst->velocity_x * direction;
                    burst->y += burst->velocity_y;
                    burst->variant++;
                    if (burst->variant == 24)
                        burst->variant = -1;
                }
            }
        }
        for (i = 0; i != 24; i++) {
            struct EffectStep *spark = &work->particles[40 + i];
            s32 size = 1;

            if (spark->variant >= 0) {
                draw.lower(canvas, PaletteParticles_SparkPixels,
                    ((s16 *)&spark->x)[1], ((s16 *)&spark->y)[1] - size, size, size * 2);
                spark->x += spark->velocity_x * direction;
                spark->y += spark->velocity_y;
                spark->velocity_y += -0x400;
                spark->variant++;
                if (spark->variant == 48)
                    spark->variant = -1;
            }
        }
        if (frame <= 175) {
            for (i = 0; i != 512; i++) {
                struct EffectStep *drop = &gDrops[i];

                if (drop->variant >= 0) {
                    u8 size = PaletteParticles_DropSizes[i & 3];
                    s32 height = size * 2;

                    draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        ((s16 *)&drop->x)[1] - size / 2, ((s16 *)&drop->y)[1] - size, size, height);
                    drop->x += drop->velocity_x * direction;
                    drop->y += drop->velocity_y;
                    if (frame > 128) {
                        if (i & 1)
                            drop->velocity_x += -0x8000;
                        else
                            drop->velocity_x += -0x2000;
                    } else {
                        drop->velocity_y += -0x400;
                    }
                    drop->variant++;
                    if (drop->variant == 256)
                        drop->variant = -1;
                }
            }
        }
        if (frame == 128)
            work->shake_frames = 48;
        if (mode == 0 && frame == 48)
            work->shake_frames = 8;
        if ((u32)(frame - 40) <= 7) {
            focus.x += -0x80000;
            focus.y += 0x100000;
        }
        if (mode == 0) {
            if (frame == 128) {
                Object_InitializeMode(work->objects[0], 8);
                Object_InitializeMode(work->objects[1], 9);
                Object_InitializeMode(work->objects[3], 10);
                Object_InitializeMode(work->objects[4], 11);
            }
            if (frame == 176) {
                Object_InitializeMode(work->objects[0], 0);
                Object_InitializeMode(work->objects[1], 1);
                Object_InitializeMode(work->objects[3], 3);
                Object_InitializeMode(work->objects[4], 4);
            }
            BattleFx_PlaceFormationObjects(3, focus.x, focus.y);
        }
        if (frame == 138) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
            }
        }
        if (frame <= 175) {
            for (i = 0; i != 16; i++) {
                struct EffectStep *streak = &work->particles[24 + i];

                if (streak->y > 55) {
                    if ((u32)streak->variant <= 11) {
                        s32 picture = streak->variant / 2;

                        draw.upper(canvas, work->sheet + PaletteParticles_StreakOffsets[picture] + 0x3000,
                            streak->x - PaletteParticles_StreakWidths[picture] / 2,
                            streak->y + PaletteParticles_StreakDrops[picture],
                            PaletteParticles_StreakWidths[picture], PaletteParticles_StreakHeights[picture]);
                    }
                    streak->variant++;
                    if (streak->variant == 12)
                        streak->variant = 0;
                } else if (streak->variant == 0) {
                    s32 size = 10;

                    streak->x -= direction * 6;
                    streak->y += 6;
                    if (frame <= 47 && streak->y > 55)
                        AudioCommand_PlayFar(136);
                    draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        streak->x - size / 2, streak->y + 30, size, size * 2);
                } else {
                    streak->variant++;
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    if (mode == 0) {
        BattleEffect_RunImpactBurst(3, focus.x, focus.y);
        for (i = 0; i != 8; i++)
            ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }
    BattleFx_EndCanvasLayer();
}

/*
 * A scrolling band of spray rises while a focus point glides in from the
 * right and settles; from frame 28 embers burst around it, and for 48 frames
 * three columns of fire climb while sixteen more embers join each frame.
 * Embers grow, fall under a per-lane pull, and die once they drop past the
 * ground. A held button skips the middle of the effect. Afterwards the
 * impact burst plays at the focus point and the spawned objects are freed.
 */
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **cursor;
    void *canvas;
    struct BattleEffectWork *work;
    u8 *graphics;
    u8 *palette;

    cursor = (void **)(gWorkSlot + 40 * 4);
    canvas = cursor[0];
    work = cursor[-1];
    graphics = cursor[1];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000052 = 0x1010;
    BattleFx_FetchRectangleBlitters(0, draw);
    palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
    palette += 128;
    Resource_DecodeType01(palette, (u8 *)work + 0x6e4);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    {
        struct Point2D focus;
        struct Point2D velocity;
        s32 rise;
        s32 frame;
        s32 k;
        struct EffectStep *point;
        struct EffectStep *spark;
        struct EffectStep *sparks;
        s32 *life;

        rise = 0;
        focus.x = 256 << 16;
        focus.y = 88 << 16;
        velocity.x = -0x100000;
        velocity.y = -0x40000;
        for (k = 0; k != 64; k++)
            work->particles[k].variant = -1;
        for (k = 0; k != 16; k++) {
            point = &work->particles[24 + k];
            point->x = Random16() & 127;
            point->y = (Random16() & 7) + 56;
            point->variant = -(s32)(Random16() & 15);
        }
        life = &((struct EffectStep *)gMapCellBuffer)->variant;
        for (k = 0; k != 1024; k++) {
            *life = -1;
            life += 7;
        }
        sparks = (struct EffectStep *)gMapCellBuffer;
        BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);
        WaitFrames(1);
        BattleFx_SpawnObjects(12, 380, 2);

        for (frame = 0; frame != 124; frame++) {
            if ((gKeysRepeat & 3) && frame > 32 && frame <= 97)
                frame = 98;
            if (frame == 120)
                BattleEventRuntime_BeginPhaseFar(134);
            if (frame <= 15)
                rise += 2;
            if (frame <= 99) {
                focus.x += velocity.x;
                focus.y += velocity.y;
                velocity.x = velocity.x * 58 / 64;
                velocity.y = velocity.y * 56 / 64;
                if (focus.x <= 0x77ffff)
                    velocity.x += 0x8000;
            }
            BattleFx_PlaceFormationObjects(1, focus.x, focus.y);
            if (frame == 28) {
                for (k = 0; k != 256; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
                        spark->velocity_x = ((s32)(Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(s32)(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                    }
                }
            }
            if (frame >= 32 && frame < 80) {
                s32 spawned = 0;

                for (k = 0; k != 1024; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
                        spark->velocity_x = ((s32)(Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(s32)(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                        spawned++;
                        if (spawned == 16)
                            break;
                    }
                }
            }
            if (frame == 0)
                AudioCommand_PlayFar(164);
            if (frame == 32)
                AudioCommand_PlayFar(145);
            if (frame == 80)
                AudioCommand_PlayFar(144);
            if (frame >= 32 && frame < 80) {
                for (k = 0; k != 3; k++) {
                    s32 span = (frame * 16 - 256 + k * 25) % 104;

                    draw[0](canvas, (u8 *)work + 0x6e4, EmberColumns_Columns[k * 2] - 17,
                        EmberColumns_Columns[k * 2 + 1] - span - 104, 34, 104);
                    draw[0](canvas, (u8 *)work + 0x6e4, EmberColumns_Columns[k * 2] - 17,
                        EmberColumns_Columns[k * 2 + 1] - span, 34, span);
                }
            }
            if (frame <= 95) {
                for (k = 0; k != 5; k++)
                    draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise, 32, 32);
            }
            for (k = 0; k != 1024; k++) {
                spark = &sparks[k];
                if (spark->variant >= 0) {
                    s32 size = k % 3 + 2;

                    if (spark->velocity_y > 0)
                        size += 2;
                    if (frame > 68 && size <= 5)
                        size = 6;
                    if (frame > 70 && size <= 6)
                        size = 7;
                    if (frame > 72 && size <= 7)
                        size = 8;
                    if (frame > 74 && size <= 8)
                        size = 9;
                    if (frame > 76)
                        size = 10;
                    draw[spark->velocity_y > 0](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
                        ((s16 *)&spark->x)[1] - size / 2, ((s16 *)&spark->y)[1] - size, size, size * 2);
                    spark->x += spark->velocity_x;
                    spark->y += spark->velocity_y;
                    if (frame > 80)
                        spark->velocity_y += -0x8000;
                    else
                        spark->velocity_y += EmberColumns_Gravity[k & 3];
                    spark->velocity_x = spark->velocity_x * 62 / 64;
                    spark->velocity_y = spark->velocity_y * 62 / 64;
                    spark->variant++;
                    if (spark->velocity_y > 0 && ((s16 *)&spark->y)[1] > 104)
                        spark->variant = -1;
                }
            }
            if (frame <= 79) {
                for (k = 0; k != work->effect->count; k++) {
                    if (frame > 29) {
                        s32 phase = frame % 12;

                        if (phase == 0) {
                            struct MotionObject *object = GetBattleObjectSlotFar(work->effect->actors[k])->object;

                            ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, -1, 0);
                            object->velocity_y = 0x48000;
                            object->vertical_motion_strength = 0xab85;
                        }
                        if (phase == 6)
                            ObjectGroup_UpdateMembers(work->effect->actors[k], 0, 5, -1, 0);
                    }
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }

        Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        BattleEffect_RunImpactBurst(1, focus.x, focus.y);
        for (k = 0; k != 12; k++)
            ResourceObject_ReleaseFar((struct ResourceObjectWork *)((void **)((u8 *)work + 0x77d8))[k]);
    }
    BattleFx_EndCanvasLayer();
}
