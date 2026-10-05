#include "PROTO.H"
#include "GLOBAL.H"
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "BATTLE_PRESENTATION.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)


void Audio_PlayCue(s32 cue);


void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 i;
    s32 frame;
    DrawRectangle draw[2];
    u8 *sheet;
    struct EffectPosition position;
    s32 j;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = heap_cache[7];
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet + 0xc56, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    for (j = 0; j != 1024; j++)
        ((struct EffectStep *)Ram_MapCellBuffer)[j].variant = 0;
    for (j = 0; j != 64; j++)
        work->particles[j].variant = -1;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(138);
    for (frame = 0; frame != work->effect->count * 8 + 40; frame++) {
        if (frame == 24)
            BattleEventRuntime_BeginPhaseFar(133);
        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 8)
                Iwram_FillWords(canvas, 0x4000, 0x10101010);
        }
        for (i = 0; i != work->effect->count; i++) {
            s32 start = i * 8;

            EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i], &position);
            position.x /= 2;
            if (frame == start + 1)
                work->shake_frames = 4;
            if (frame == start + 4) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
            }
            if (frame >= start && frame < start + 16) {
                s32 height = (frame - start) << 6;

                if (height > 104)
                    height = 104;
                for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 3]; j++) {
                    draw[0](canvas, work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
                        position.x - 12, 0, 24, height);
                }
                if (frame == start + 2) {
                    for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4]; j++) {
                        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i * 128 + j];
                        s32 speed = (Random16() & 0x1ff) + 64;
                        s32 angle = (Random16() & 0x7fff) - 0x4000;

                        spark->x = position.x << 16;
                        spark->y = 0x680000;
                        spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
                        spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
                        spark->variant = (Random16() & 7) + 32;
                    }
                }
            }
            if (frame >= start + 2 && frame < start + 24) {
                for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 1]; j++) {
                    s32 kind = j & 3;
                    s32 drop = Random16() % LightningBolts_Counts[work->effect->variant * 4 + 2];
                    s32 spread = LightningBolts_Counts[work->effect->variant * 4 + 2] - drop + 1;
                    s32 y = position.y - drop - (BattleFx_GlintCellHeights[kind] >> 1) + 8;
                    u32 random = Random16();
                    s32 x = position.x;

                    x += random % spread;
                    x -= spread / 2;
                    x -= (BattleFx_GlintCellWidths[kind] >> 1);

                    BattleEffect_LoadWork(47, 7, 7, LightningBolts_GlintPalettes[Random16() & 3] | 3,
                        LightningBolts_GlintModes[work->effect->variant]);
                    ((DrawRectangle *)gBattleFxWork)[8](canvas,
                        work->sheet + BattleFx_GlintCellOffsets[kind], x, y,
                        BattleFx_GlintCellWidths[kind], BattleFx_GlintCellHeights[kind]);
                    Runtime_ReleaseHeapBlock(47);
                }
            }
        }
        for (j = 0; j != 1024; j++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[j];

            if (spark->variant > 0) {
                spark->variant--;
                EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
                if (spark->y > 0x680000) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
                    s32 size = spark->variant / 16 + 1;
                    s32 x = spark->x >> 16;
                    s32 y = spark->y >> 16;

                    draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        x - size / 2, y - size, size, size * 2);
                }
            }
        }
        Camera_ApplyShake(2, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
