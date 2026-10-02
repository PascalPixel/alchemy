#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
s32 BattleFx_BeginTiledCanvas(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
/* Per variant; the second column is how many sparks one bolt throws. */
extern const u8 LightningBolts_Sparks[][2];

#define SPARK_COUNT 1024

/* Battle effect: for sixteen frames four lightning bolts strike down on an
   arc that closes in on the middle of the screen, each throwing sparks from
   where it lands; the sparks bounce on the ground and shrink as they age. */
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    void *sheet;
    s32 bolt;
    s32 count;
    DrawRectangle draw[2];
    s32 i;

    heap_cache = &gWorkSlot[39];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginTiledCanvas(1);
    if (work->effect->variant == 2)
        REG_BG2PA = 0x80;
    else
        REG_BG2PA = 0x100;
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 3);
    draw[1] = (DrawRectangle)gWorkSlot[47];
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, (u8 *)work + 0x60e, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    if (work->effect->variant == 2) {
        if (work->effect->side == 1)
            REG_BG2X = -0x1000;
        else
            REG_BG2X = 0x1000;
    } else {
        if (work->effect->side == 1)
            REG_BG2X = -0x8000;
    }

    for (i = 0; i != SPARK_COUNT; i++) {
        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 speed;
        s32 angle;

        speed = (Random16() & 0x3ff) + 0x100;
        angle = (Random16() & 0x7fff) - 0x4000;
        spark->x = 0x4000;
        spark->y = 0x7000;
        spark->z = (Trig_Sin(angle) * speed) >> 16;
        spark->velocity_z = -(Trig_Cos(angle) * speed * 2) >> 16;
        spark->variant = 0;
    }

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(138);

    for (frame = 0; frame != 64; frame++) {
        if (frame == 20)
            BattleEventRuntime_BeginPhaseFar(133);
        if (frame < 16) {
            if (frame % 5 == 2)
                Iwram_FillWords(canvas, 0x4000, 0x10101010);
            for (bolt = 0; bolt != 4; bolt++) {
                s32 angle;
                s32 x;
                s32 y;

                count = 0;
                angle = ((frame << 11) + 0x4000) * bolt;
                x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
                y = -((Trig_Cos(angle) << 3) >> 16) - 8;
                if (work->effect->variant == 0)
                    draw[0](canvas, (u8 *)work + (Random16() & 3) * 2880 + 0x60e,
                        x + (Random16() & 7) - 16, y, 24, 120);
                else
                    draw[bolt & 1](canvas, (u8 *)work + (Random16() & 3) * 2880 + 0x60e,
                        x + (Random16() & 7) - 16, y, 24, 120);
                for (i = 0; i != SPARK_COUNT; i++) {
                    struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                    if (spark->variant == 0) {
                        s32 speed;
                        s32 spread;

                        speed = (Random16() & 0x1ff) + 128;
                        spread = (Random16() & 0x7fff) - 0x4000;
                        spark->x = x << 16;
                        spark->y = (y + 112) << 16;
                        spark->velocity_x = (Trig_Sin(spread) * speed) >> 9;
                        spark->velocity_y = -(Trig_Cos(spread) * speed * 2) >> 7;
                        spark->variant = (Random16() & 7) + 32;
                        count++;
                        if (count == LightningBolts_Sparks[work->effect->variant][1])
                            break;
                    }
                }
            }
            work->shake_frames = 1;
        }

        for (i = 0; i != SPARK_COUNT; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            if (spark->variant > 0) {
                spark->variant--;
                EffectStep_AdvanceWithGravity2D(spark, 60, -0x800);
                if (spark->y > 0x780000) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
                    s32 x = spark->x >> 16;
                    s32 y = spark->y >> 16;
                    s32 size = spark->variant / 8 + 1;

                    draw[0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                        x - size / 2, y - size, size, size * 2);
                }
            }
        }

        if (frame >= 4 && frame < 96) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame == i * 4 + 4)
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
            }
        }
        Camera_ApplyShake(2, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
