/* Battle presentation: set up the effect display. Windows 0 and 1 cover the
   screen, blending starts from a clean slate, and the display control write
   (mode 1, BG0/BG1/BG2 and objects) is queued for the next frame; then wait
   one frame for it to land.

   FAKEMATCH: the queued write is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
   written out inline with that function's two odd constructs: the loop that
   runs once around the IME read and the count stored through an explicit u16
   pointer. */
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

void WaitFrames(s32 frames);

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 gBattleFxWork;
extern struct Position gBgScroll;

extern u8 gMapCellBuffer[];

void BattlePres_ConfigureEffectDisplay(void)
{
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
    u8 *base = (u8 *)gBattleFxWork;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        gBgScroll.x += *(s32 *)(base + 0x7798);
        gBgScroll.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}

void Camera_AdvanceBg2Reference(void)
{
    s32 cnt;
    void *state;

    state = *(void **)((u32)&gBattleFxWork);
    cnt = FIELD_AT_OFFSET(state, s32 *, 0x7790) + 1;
    FIELD_AT_OFFSET(state, s32 *, 0x7790) = cnt;
    if (cnt == FIELD_AT_OFFSET(state, s32 *, 0x7794)) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = (s32)FIELD_AT_OFFSET(state, s32 *, 0x77D0);
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 4) = (s32)FIELD_AT_OFFSET(state, s32 *, 0x77D4);
        FIELD_AT_OFFSET(state, s32 *, 0x77D0) = (s32)(FIELD_AT_OFFSET(state, s32 *, 0x77D0) + FIELD_AT_OFFSET(state, s32 *, 0x7798));
        FIELD_AT_OFFSET(state, s32 *, 0x77D4) = (s32)(FIELD_AT_OFFSET(state, s32 *, 0x77D4) + FIELD_AT_OFFSET(state, s32 *, 0x779C));
        FIELD_AT_OFFSET(state, s32 *, 0x7790) = 0;
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
struct HeapSlots {
    void *blocks[46];
    DrawRectangle blitters[2];
};

extern struct HeapSlots gWorkSlot;
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
    draw[0] = gWorkSlot.blitters[0];
    draw[1] = gWorkSlot.blitters[1];

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
