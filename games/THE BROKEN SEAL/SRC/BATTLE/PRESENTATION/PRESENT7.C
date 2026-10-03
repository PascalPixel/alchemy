#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"
/* Battle presentation: set up the effect display. Windows 0 and 1 cover the
   screen, blending starts from a clean slate, and the display control write
   (mode 1, BG0/BG1/BG2 and objects) is queued for the next frame; then wait
   one frame for it to land. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "RAM_BUFFER.H"

void WaitFrames(s32 frames);


extern struct BattleEffectWork *gBattleFxWork;

extern u8 gMapCellBuffer[];

void BattlePres_ConfigureEffectDisplay(void)
{
    /* FAKEMATCH: the queued write is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
     * written out inline with that function's two odd constructs: the loop that
     * runs once around the IME read and the count stored through an explicit u16
     * pointer. */
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;

    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;

    q = &gIoWriteQueue;
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        ime = &REG_IME;
        saved = *ime;
    } while (0);
    *ime = (u16)ime;
    count = q->count;
    if (count <= 31) {
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
        *(u16 *)&q->count = count + 1;
        *destination++ = 0x7741;
        *destination++ = 0x04000000;
        *destination = 0x20000;
    }
    *ime = saved;

    WaitFrames(1);
}

void BattleFx_AdvanceScrollOnInterval(void)
{
    struct BattleEffectWork *work = gBattleFxWork;
    u32 *counter = (u32 *)&work->scroll_timer;

    (*counter)++;
    if (*counter == (u32)work->scroll_interval) {
        gBgScroll[1].x += work->scroll_step_x;
        gBgScroll[1].y += work->scroll_step_y;
        *counter = 0;
    }
}

void Camera_AdvanceBg2Reference(void)
{
    s32 count;
    struct BattleEffectWork *work;

    work = gBattleFxWork;
    count = work->scroll_timer + 1;
    work->scroll_timer = count;
    if (count == work->scroll_interval) {
        REG_BG2X = work->bg2x;
        REG_BG2Y = work->bg2y;
        work->bg2x += work->scroll_step_x;
        work->bg2y += work->scroll_step_y;
        work->scroll_timer = 0;
    }
}

/* H-blank callback: feed the per-line WIN0H table at 0x02010000 to WIN0H. */
void BattleFx_ArmWin0HBlankDma(void)
{
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set((void *)gMapCellBuffer, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
}

/* The heap slot table; BattleEffect_LoadWork leaves its rectangle blitters
   in slots 46 and 47. */

extern u16 BattleFx_PuffCells[];
extern u8 BattleFx_PuffSizes[];
/* For each variant: how many shards fall and how many frames it lasts. */
extern u8 FallingShards_Counts[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void BattleFx_EndCanvasLayer(void);

/*
 * Battle effect: ice shards fall in a slanted line towards the targets'
 * side; each bursts where it lands, shakes the targets and throws off puffs.
 */
void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    u8 *graphics;
    DrawRectangle draw[2];
    struct EffectStep *shard;
    struct EffectStep *puff;
    u16 *line;
    s32 i;
    s32 k;

    heap_cache = (void **)&gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2001);
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, graphics, 0, 0);
    BattlePres_ConfigureEffectDisplay();
    REG_BLDCNT = 0x3f44;
    REG_WININ = 0x3337;

    {
        s32 y;
        s32 slant;

        for (i = 0, y = -128, slant = -16; i != 32; y -= 64, i++, slant -= 8) {
            s32 x;

            shard = &work->particles[i];
            x = (Random16() & 63) + (Random16() & 7) + 24;
            if (work->effect->side == 1)
                x = x + slant + 24;
            else
                x = x - slant + 80;
            shard->y = y;
            shard->x = x << 3;
            shard->variant = -1;
        }
    }
    for (i = 0; i != 32; i++)
        work->particles[32 + i].variant = -1;

    if (work->effect->side == 0) {
        BattleEffect_LoadWork(46, 7, 7, 2, 2);
        BattleEffect_LoadWork(47, 7, 7, 2, 3);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 6, 2);
        BattleEffect_LoadWork(47, 7, 7, 6, 3);
    }
    draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[46];
    draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[47];

    if (work->effect->side == 0) {
        line = (u16 *)gMapCellBuffer;
        for (i = 0; i != 160; i++) {
            if ((u32)(i - 8) <= 95)
                line[i] = (0xf0 - i) | ((0x70 - i) << 8);
            else if (i <= 135)
                line[i] = 0x888;
            else
                line[i] = 0x100;
        }
    } else {
        line = (u16 *)gMapCellBuffer;
        for (i = 0; i != 160; i++) {
            if ((u32)(i - 8) <= 87)
                line[i] = (i - 8 + 160) | ((i + 0x18) << 8);
            else if (i <= 135)
                line[i] = 0x78f8;
            else
                line[i] = 0x100;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmWin0HBlankDma, 0x480);
    work->transfer_mode = 2;
    if (work->effect->variant == 1)
        work->transfer_value = 75;
    else
        work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != FallingShards_Counts[work->effect->variant * 2 + 1]; frame++) {
        if (frame == FallingShards_Counts[work->effect->variant * 2 + 1] - 16)
            BattleEventRuntime_BeginPhaseFar(132);
        for (i = 0; i != FallingShards_Counts[work->effect->variant * 2]; i++) {
            shard = &work->particles[i];
            if (shard->variant == -1) {
                s32 x = shard->x / 8;
                s32 y = shard->y / 8;

                draw[work->effect->variant == 2 ? 1 : 0](canvas, work, x, y, 32, 32);
                if (shard->y <= 0x27f) {
                    if (work->effect->side == 0)
                        shard->x -= 64;
                    else
                        shard->x += 64;
                    shard->y += 64;
                } else {
                    if ((i & 3) == 0)
                        AudioCommand_PlayFar(115);
                    work->shake_frames = 2;
                    shard->variant = 0;
                    for (k = 0; k != work->effect->count; k++)
                        ObjectGroup_UpdateMembers(work->effect->actors[k], 9, 5, k, 8);
                }
            }
            if (shard->variant != -1) {
                s32 x = shard->x / 8;
                s32 y = shard->y / 8;

                if ((u32)(shard->variant - 1) <= 13) {
                    draw[work->effect->variant == 2 ? 1 : 0](canvas,
                        (u8 *)work + 0x400 + ((shard->variant / 3) << 10), x, y, 32, 32);
                }
                if ((u32)(shard->variant - 9) <= 2) {
                    for (k = 0; k != 32; k++) {
                        puff = &work->particles[32 + k];
                        if (puff->variant == -1) {
                            puff->variant = 18;
                            puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
                            puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
                            break;
                        }
                    }
                }
                if (shard->variant <= 14)
                    shard->variant++;
            }
        }
        for (i = 0; i != 32; i++) {
            puff = &work->particles[32 + i];
            if (puff->variant != -1) {
                if (puff->variant <= 17) {
                    s32 image = puff->variant / 2;
                    s32 x = puff->x / 8 - (BattleFx_PuffSizes[image] >> 1);
                    s32 y = puff->y / 8 - (BattleFx_PuffSizes[image] >> 1);

                    draw[work->effect->variant == 2 ? 1 : 0](canvas,
                        graphics + BattleFx_PuffCells[image], x, y,
                        BattleFx_PuffSizes[image], BattleFx_PuffSizes[image]);
                }
                if (puff->variant > -1)
                    puff->variant--;
            }
        }
        ObjectGroup_TickMemberTimers();
        Camera_ApplyShake(4, 4);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((u32)BattleFx_ArmWin0HBlankDma);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
    BattlePres_ConfigureEffectDisplay();
}

extern u16 BattleFx6_FlareCells[];
/* For each variant: how many bolts fall, how many sparks each throws off
   where it lands, how far apart the bolts start and how many frames the
   effect lasts. */
extern u8 FallingBolts_Counts[];

/* The map cell buffer as this effect uses it: WIN0H for every screen line,
   then the sparks. */
struct BoltSparkBuffer {
    u16 lines[160];
    struct EffectStep sparks[512];
};

#define gBoltSparks ((struct BoltSparkBuffer *)Ram_MapCellBuffer)

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

/*
 * Battle effect: bolts fall in a slanted line towards the targets' side;
 * each bursts where it lands, shakes the targets and throws off sparks that
 * fall away under gravity.
 */
void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw_spark;
    DrawRectangle draw_bolt;
    struct EffectStep *bolt;
    struct EffectStep *spark;
    u16 *line;
    s32 i;
    s32 k;

    heap_cache = (void **)&gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2001);
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, (u8 *)work + 0x604, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work, 0, 0);
    BattlePres_ConfigureEffectDisplay();
    REG_BLDCNT = 0x3f44;
    REG_WININ = 0x3337;

    BattleEffect_LoadWork(46, 7, 7, 2, 2);
    draw_bolt = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 2, 3);
    draw_spark = heap_cache[8];

    for (i = 0; i != 512; i++)
        gBoltSparks->sparks[i].variant = -1;

    for (i = 0; i != 64; i++) {
        s32 x;
        s32 y;

        bolt = &work->particles[i];
        x = Random16() & 63;
        y = -(i * FallingBolts_Counts[work->effect->variant * 4 + 2] + 16);
        if (work->effect->side == 1)
            x = x + y / 2 - 48;
        else
            x = x - y / 2 + 72;
        bolt->x = x << 3;
        bolt->y = y << 3;
        bolt->variant = -1;
    }

    if (work->effect->side == 0) {
        line = (u16 *)gMapCellBuffer;
        for (i = 0; i != 160; i++) {
            if ((u32)(i - 8) <= 95)
                line[i] = ((0x34 - i / 2) << 8) | (0xb4 - i / 2);
            else if (i <= 135)
                line[i] = 0x80;
            else
                line[i] = 0x100;
        }
    } else {
        line = (u16 *)gMapCellBuffer;
        for (i = 0; i != 160; i++) {
            if ((u32)(i - 8) <= 95)
                line[i] = ((i / 2 + 60) << 8) | (i / 2 + 188);
            else if (i <= 135)
                line[i] = 0x70f0;
            else
                line[i] = 0x100;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmWin0HBlankDma, 0x480);
    if (work->effect->variant == 0) {
        work->transfer_mode = 1;
        work->transfer_value = 0;
    } else if (work->effect->variant == 1) {
        work->transfer_mode = 2;
        work->transfer_value = 50;
    } else {
        work->transfer_mode = 2;
        work->transfer_value = 75;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != FallingBolts_Counts[work->effect->variant * 4 + 3]; frame++) {
        if (frame == FallingBolts_Counts[work->effect->variant * 4 + 3] - 64)
            BattleEventRuntime_BeginPhaseFar(132);
        for (i = 0; i != FallingBolts_Counts[work->effect->variant * 4]; i++) {
            s32 x;
            s32 y;

            bolt = &work->particles[i];
            x = bolt->x / 8;
            y = bolt->y / 8;
            if (bolt->variant == -1) {
                draw_bolt(canvas, (u8 *)work + 0x604, x, y, 24, 24);
                if (bolt->y <= 0x27f) {
                    if (work->effect->side == 0)
                        bolt->x -= 32;
                    else
                        bolt->x += 32;
                    bolt->y += 64;
                } else {
                    bolt->variant = 0;
                    for (k = 0; k != FallingBolts_Counts[work->effect->variant * 4 + 1]; k++) {
                        spark = &gBoltSparks->sparks[
                            i * FallingBolts_Counts[work->effect->variant * 4 + 1] + k];
                        spark->x = (x + 12) << 16;
                        spark->y = y << 16;
                        spark->velocity_x = ((Random16() & 255) - 128) << 9;
                        if (work->effect->variant == 2)
                            spark->velocity_y = ((Random16() & 0x1ff) - 0x180) << 10;
                        else
                            spark->velocity_y = ((Random16() & 255) - 255) << 10;
                        spark->variant = (Random16() & 15) + 16;
                    }
                    if ((i & 3) == 0)
                        AudioCommand_PlayFar(132);
                    for (k = 0; k != work->effect->count; k++)
                        ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 2);
                }
            } else {
                if ((u32)bolt->variant <= 3)
                    draw_bolt(canvas, (u8 *)work + 0x844, x, y, 24, 24);
                else if (bolt->variant <= 7)
                    draw_bolt(canvas, (u8 *)work + 0xa84, x - 9, y - 9, 42, 42);
                if (bolt->variant <= 14)
                    bolt->variant++;
            }
        }
        for (i = 0; i != 512; i++) {
            spark = &gBoltSparks->sparks[i];
            if (spark->variant != -1) {
                s32 size = spark->variant + 1;

                if (size > 6)
                    size = 6;
                draw_spark(canvas, (u8 *)work + BattleFx6_FlareCells[size - 1],
                    ((s16 *)&spark->x)[1] - size, ((s16 *)&spark->y)[1] - size,
                    size * 2, size * 2);
                EffectStep_AdvanceWithGravity2D(spark, 60, 0x2000);
                spark->variant--;
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattleFx_ArmWin0HBlankDma);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
    BattlePres_ConfigureEffectDisplay();
}
