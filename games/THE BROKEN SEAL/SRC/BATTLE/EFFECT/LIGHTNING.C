#include "CANVAS.H"
#include "RUNTIME_MEM.H"
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

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];
extern u16 ParticleStreams_CellOffsets[];
/* By variant: how many sparks a pillar throws, and how long it shakes. */
extern u16 LightningPillar_Sparks[];
/* By side, seven columns each: where each pillar strikes. */
extern u8 LightningPillar_Columns[];
/* By variant: how many pillars strike. */
extern u8 LightningPillar_Counts[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

/*
 * Pillars of lightning strike one column after another, eight frames apart.
 * Each pillar flashes the canvas, shows its two cells for three frames,
 * throws sparks that bounce along the ground, and knocks the affected units.
 * The variant picks how many pillars strike; the strongest also scrolls the
 * camera toward the struck side for its first 64 frames.
 */
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 i;
    u8 *graphics;
    s32 count;
    s32 frame;
    s32 k;
    s32 start;
    s32 *slot;
    struct EffectStep *spark;
    struct EffectStep *sparks;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000052 = 0x1010;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    draw[0] = (DrawRectangle)heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw[1] = (DrawRectangle)heap_cache[8];
    Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
    if (work->effect->variant != 2) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
        /* FAKEMATCH: calling through a local copy of the routine's address
           loads that address before the palette address is shifted into
           place; the direct call swaps those two instructions. */
        WordCopy copy = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }
    slot = &((struct EffectStep *)gMapCellBuffer)->variant;
    for (k = 0; k != 1024; k++) {
        *slot = 0;
        slot += 7;
    }
    sparks = (struct EffectStep *)gMapCellBuffer;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    count = LightningPillar_Counts[work->effect->variant];

    for (frame = 0; frame != count * 8 - count + 48; frame++) {
        if (work->effect->variant == 2 && frame <= 63) {
            struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + 12 * 4);
            s32 speed;

            if (frame <= 55)
                speed = 256;
            else
                speed = 0x2c0 - frame * 8;
            if (work->effect->side == 1)
                camera->scroll -= speed;
            else
                camera->scroll += speed;
        }
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(134);
        for (i = 0; i != count; i++) {
            start = i * 8;
            if (frame == start) {
                AudioCommand_PlayFar(134);
                Iwram_FillWords(canvas, 0x4000, 0x10101010);
            }
            if (frame >= start && frame < start + 9) {
                if (frame >= start + 1 && frame < start + 2)
                    draw[0](canvas, work,
                        LightningPillar_Columns[i + work->effect->side * 7] - 24, 0, 48, 112);
                if (frame >= start + 2 && frame < start + 4)
                    draw[0](canvas, (u8 *)work + 0x1500,
                        LightningPillar_Columns[i + work->effect->side * 7] - 24, 0, 48, 112);
                if (frame == start + 2) {
                    s32 spawned = 0;

                    for (k = 0; k != 1024; k++) {
                        spark = &sparks[k];
                        if (spark->variant == 0) {
                            s32 speed;
                            s32 angle;

                            speed = (Random16() & 0x3ff) + 32;
                            angle = (Random16() & 0x7fff) - 0x4000;

                            spark->x = LightningPillar_Columns[i + work->effect->side * 7] << 16;
                            spark->y = 0x680000;
                            spark->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                            spark->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                            spark->variant = (Random16() & 7) + 32;
                            spawned++;
                            if (spawned == LightningPillar_Sparks[work->effect->variant * 2])
                                break;
                        }
                    }
                    work->shake_frames = LightningPillar_Sparks[work->effect->variant * 2 + 1];
                }
            }
            if (frame == start + 4) {
                for (k = 0; k != work->effect->count; k++) {
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[k], 1);
                    ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
                }
            }
        }
        for (k = 0; k != 1024; k++) {
            spark = &sparks[k];
            if (spark->variant > 0) {
                spark->variant--;
                EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
                if (spark->y > 0x680000) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else {
                    s32 x = spark->x >> 16;
                    s32 y;
                    s32 size;

                    if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
                        y = spark->y >> 16;
                        size = spark->variant / 8 + 1;
                        draw[k & 1](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
                            x - size / 2, y - size, size, size * 2);
                    }
                }
            }
        }
        Camera_ApplyShake(8, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
