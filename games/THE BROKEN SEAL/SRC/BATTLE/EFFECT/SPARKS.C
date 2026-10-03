#include "RESOURCE.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

struct CameraWork {
    u8 unknown_00[0x36];
    u16 scroll;
};

typedef s32 (*WordCopy)(void *, const void *, s32);

extern void *gWorkSlot[];
extern u8 gMapCellBuffer[];
extern u16 ParticleStreams_CellOffsets[];
/* Five words by variant: sparks per group, groups, and each group's column. */
extern u16 SparkGroups_Shapes[];
/* By a ring flash's age in threes: which cell of the sheet it shows. */
extern u8 SparkGroups_FlashCells[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
s32 BattleFx_EndCanvasLayer(void);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding the copy through this helper loads the routine's
       address before the palette address is shifted into place; a direct
       call swaps those two instructions at both palette copies. */
    copy(destination, source, size);
}

/*
 * Groups of sparks burst one after another, eight frames apart. Each group
 * flashes for two frames, shows a ring of twelve fading flashes around its
 * column, then throws its sparks, which fall and bounce until they fade.
 * The variant picks how many groups there are, how many sparks each throws
 * and where each column stands; kind 0 and 1 burst at a fixed place and any
 * other kind at the acting unit, and kind 2 makes the sparks rise.
 */
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind)
{
    struct EffectPosition pos;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 i;
    s32 frame;
    u8 *graphics;
    s32 origin_x;
    s32 origin_y;
    s32 j;
    s32 start;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;

    heap_cache = gWorkSlot + 39;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    work->effect = effect;
    if (kind == 0) {
        BattleFx_BeginCanvasLayer(1);
        origin_x = 60;
        origin_y = 48;
    } else if (kind == 1) {
        BattleFx_BeginCanvasLayer(0);
        origin_x = 60;
        origin_y = 64;
    } else {
        BattleFx_BeginCanvasLayer(0);
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
        origin_x = pos.x / 2;
        origin_y = pos.y + 48;
    }
    *(u16 *)0x04000052 = 0x1010;
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw[1] = (DrawRectangle)gWorkSlot[47];
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
    if (kind == 1) {
        CopyPalette(Iwram_CopyWords, (void *)0x05000000,
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    } else if (kind == 2) {
        CopyPalette(Iwram_CopyWords, (void *)0x05000000,
            Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
    }
    sparks = (struct EffectStep *)gMapCellBuffer;

    for (i = 0; i != SparkGroups_Shapes[work->effect->variant * 5 + 1]; i++) {
        for (j = 0; j != 16; j++) {
            s32 radius;
            s32 angle;

            point = &work->particles[i * 16 + j];
            radius = j * 2;
            angle = Random16() & 0xffff;
            point->x = Trig_Sin(angle) * radius;
            point->y = -(Trig_Cos(angle) * radius);
            point->variant = j / 2 + 25;
        }
        for (j = 0; j != SparkGroups_Shapes[work->effect->variant * 5]; j++) {
            s32 speed;
            s32 angle;

            spark = &sparks[i * SparkGroups_Shapes[work->effect->variant * 5]] + j;
            speed = (Random16() & 0x3ff) + 32;
            angle = Random16() & 0xffff;
            if (work->effect->side == 1)
                spark->x = (origin_x - SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 28) << 16;
            else
                spark->x = (origin_x + SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 28) << 16;
            spark->y = origin_y << 16;
            spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
            spark->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 6;
            spark->variant = (Random16() & 7) + 32;
        }
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != SparkGroups_Shapes[work->effect->variant * 5 + 1] * 8 + 56; frame++) {
        struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + 12);

        if (work->effect->variant == 2 && frame <= 51) {
            if (work->effect->side == 0)
                camera->scroll += 256;
            else
                camera->scroll -= 256;
        }
        if (work->effect->variant == 3 && frame == 4)
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if (kind == 1 || kind == 2) {
            if (frame == 2)
                BattleEventRuntime_BeginPhaseFar(145);
        } else {
            if (frame == 2)
                AudioCommand_PlayFar(145);
            if (frame == 24)
                BattleEventRuntime_BeginPhaseFar(134);
        }
        for (i = 0; i != SparkGroups_Shapes[work->effect->variant * 5 + 1]; i++) {
            start = i * 8;
            if (frame == start)
                work->shake_frames = 12;
            if (frame >= start && frame < start + 2) {
                if (work->effect->side == 1)
                    draw[0](canvas, work,
                        origin_x - SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 12,
                        origin_y - 32, 32, 64);
                else
                    draw[0](canvas, work,
                        origin_x + SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 44,
                        origin_y - 32, 32, 64);
            }
            if (frame >= start) {
                for (j = 0; j != 12; j++) {
                    s32 x;
                    s32 y;

                    point = &work->particles[i * 16] + j;
                    y = ((s16 *)&point->y)[1] + origin_y;
                    if (work->effect->side == 1)
                        x = ((s16 *)&point->x)[1] + origin_x
                            - SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 28;
                    else
                        x = ((s16 *)&point->x)[1] + origin_x
                            + SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 28;
                    if (point->variant >= 0 && point->variant < 18)
                        draw[0](canvas,
                            (u8 *)work + (SparkGroups_FlashCells[point->variant / 3] << 11),
                            x - 16, y - 32, 32, 64);
                    if (point->variant > 0)
                        point->variant--;
                    else
                        point->variant = -1;
                }
            }
            if (frame > start + 5) {
                s32 gravity;

                if (kind == 2)
                    gravity = -0x1000;
                else
                    gravity = 0x1000;
                for (j = 0; j != SparkGroups_Shapes[work->effect->variant * 5]; j++) {
                    spark = &sparks[i * SparkGroups_Shapes[work->effect->variant * 5]] + j;
                    if (spark->variant > 0) {
                        EffectStep_AdvanceWithGravity2D(spark, 60, gravity);
                        spark->variant--;
                        if (spark->y > 0x6c0000) {
                            spark->velocity_y = -spark->velocity_y / 2;
                        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
                            s32 y = spark->y >> 16;
                            s32 x = spark->x >> 16;
                            s32 size = spark->variant / 5 + 1;

                            draw[j & 1](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
                                x - size / 2, y - size, size, size * 2);
                        }
                    }
                }
            }
            for (j = 0; j != work->effect->count; j++) {
                if (frame == start + 6) {
                    ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 10);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
                }
            }
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
