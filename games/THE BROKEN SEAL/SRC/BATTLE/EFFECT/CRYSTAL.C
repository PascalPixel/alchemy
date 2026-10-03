#include "CANVAS.H"
#include "RUNTIME_MEM.H"
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

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];
/* By variant: how many crystals fall, and for how many frames. */
extern u8 Crystal_Counts[];
/* Sixteen x and y pairs: where each shard of a broken crystal starts. */
extern u8 Crystal_ShardStarts[];
/* By shard cell: its width, its height and where it sits in the sheet. */
extern u8 Crystal_ShardWidths[];
extern u8 Crystal_ShardHeights[];
extern s32 Crystal_ShardOffsets[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

/*
 * Crystals fall one after another, eight frames apart, toward the affected
 * side. A crystal that reaches the ground breaks into sixteen shards that
 * fly apart, tumble through three cells each and slow down, while the
 * screen shakes and the affected units are knocked. The variant picks how
 * many crystals fall and how long the effect runs; the strongest also
 * scrolls the camera for its first 104 frames.
 */
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    struct EffectStep *seed;
    struct EffectStep *point;
    struct EffectStep *sparks;
    struct EffectStep *spark;
    s32 x;
    s32 cell;
    s32 i;
    s32 j;

    sparks = (struct EffectStep *)gMapCellBuffer;
    cursor = (void **)(gWorkSlot + 39 * 4);
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work, 1, 1);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (j = 0; j != Crystal_Counts[work->effect->variant * 2]; j++) {
        seed = &work->particles[j];
        x = Random16() & 31;
        seed->y = -0x400000;
        if (work->effect->side == 1) {
            x = ((Random16() & 31) + 80) << 16;
            seed->velocity_x = (Random16() & 63) << 12;
        } else {
            x = ((Random16() & 31) + 8) << 16;
            seed->velocity_x = -(s32)(Random16() & 63) << 12;
        }
        seed->x = x - seed->velocity_x * 18;
        seed->velocity_y = 0;
        seed->z = 0;
        seed->variant = j * 8;
    }

    for (frame = 0; frame != Crystal_Counts[work->effect->variant * 2 + 1]; frame++) {
        if (work->effect->variant == 2 && frame <= 103) {
            struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + 12 * 4);
            s32 speed;

            if (frame <= 95)
                speed = 192;
            else
                speed = 0x9c0 - frame * 24;
            if (work->effect->side == 0)
                camera->scroll -= speed;
            else
                camera->scroll += speed;
        }
        if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 80)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 8) {
            work->transfer_mode = 3;
            work->transfer_value = 0x06060606;
        }
        if (frame <= Crystal_Counts[work->effect->variant * 2 + 1] - 8) {
            for (i = 0; i != Crystal_Counts[work->effect->variant * 2]; i++) {
                point = &work->particles[i];
                if (point->z == 1) {
                    for (j = 0; j != 16; j++) {
                        cell = (j % 5) * 3 + (sparks[i * 16 + j].variant / 96) % 3;
                        spark = &sparks[i * 16 + j];
                        draw[j <= 2](canvas,
                            (u8 *)work + Crystal_ShardOffsets[cell] + 0x800,
                            ((s16 *)&spark->x)[1] - Crystal_ShardWidths[cell] / 2,
                            ((s16 *)&spark->y)[1] - Crystal_ShardHeights[cell] / 2,
                            Crystal_ShardWidths[cell], Crystal_ShardHeights[cell]);
                        EffectStep_AdvanceWithGravity2D(spark, 64, 0x2000);
                        spark->variant += spark->z;
                        if (spark->z > 1 && (frame & 1))
                            spark->z--;
                    }
                } else if (frame >= point->variant) {
                    draw[i & 1](canvas, work, ((s16 *)&point->x)[1] - 16, ((s16 *)&point->y)[1], 32, 64);
                    EffectStep_AdvanceWithGravity2D(point, 64, 0x10000);
                    if (point->y > 0x380000) {
                        point->z = 1;
                        point->y = 0x380000;
                        for (j = 0; j != 16; j++) {
                            spark = &sparks[i * 16 + j];
                            spark->x = ((Crystal_ShardStarts[j * 2] - 40) << 16) + point->x;
                            spark->y = Crystal_ShardStarts[j * 2 + 1] << 16;
                            spark->velocity_x = ((s32)(Random16() & 127) - 64) << 11;
                            spark->velocity_y = -(s32)(Random16() & 127) << 11;
                            if (i & 1) {
                                spark->velocity_x *= 2;
                                spark->velocity_y *= 2;
                            }
                            spark->z = 32;
                            spark->variant = 0;
                        }
                        work->shake_frames = 8;
                        AudioCommand_PlayFar(144);
                        for (j = 0; j != work->effect->count; j++)
                            ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
                    }
                }
            }
        }
        Camera_ApplyShake(work->effect->variant * 2 + 4, work->effect->variant * 4 + 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
