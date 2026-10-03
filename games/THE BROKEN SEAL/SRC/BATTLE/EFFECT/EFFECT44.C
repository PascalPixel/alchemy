#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RAM_BUFFER.H"
#include "EFFECT_STEP.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_PRESENTATION.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

u32 Random16(void);


static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from push {r5, r6, lr} to push {r5, r6, r7, lr} (122/122 assembly lines). */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
    /* FAKEMATCH: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from mov r0, r5 to lsl r1, r1, #7 (122/122 assembly lines). */
    Iwram_FillWords(destination, size, value);
}

void ColorBuffer_BackupAndHalve(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndDarken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_BackupAndBrighten(void *source, s32 amount, void *destination, s32 size);

void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void ObjectGroup_TickMemberTimers(void);

/* Battle effect: a spider web drawn as four 32 by 32 rectangles on the
   canvas layer, centred between the first and the last affected unit. The
   blend fades it in over the first nine of 63 frames and out over the last
   nine; frame 10 hits every affected unit. */
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
    struct EffectPosition first;
    struct EffectPosition last;
    void *canvas;
    struct BattleEffectWork *work;
    s32 size;
    s32 step;
    /* Left to the allocator the frame takes r10 and the step r9, which
       exchanges the two registers in 15 instructions. */
    /* FAKEMATCH: the frame counter is pinned to r9, as the reference keeps it. */
    register s32 frame __asm__("r9");
    s32 i;

    /* FAKEMATCH: the reference holds the slot table address in the register
       that later holds the size, and the blend register address in the one
       that later holds the step; only variables assigned twice do that. */
    size = (s32)&Ram_WorkSlot[40];
    canvas = *(void **)size;
    work = *(struct BattleEffectWork **)(size - 4);
    work->effect = efx;
    BattleFx_BeginCanvasLayer(2);
    step = 0x04000052;
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)step = 0x1000;
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
    first.x += (last.x - first.x) / 2;
    *(s32 *)0x04000028 = (64 - first.x) << 8;
    Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(143);
    frame = 0;
    step = 1;
    size = 32;
    do {
        if (frame <= 8)
            *(u16 *)0x04000052 = (frame << 1) | 0x1000;
        if (frame > 53)
            *(u16 *)0x04000052 = (0x7c - (frame << 1)) | 0x1000;
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, step);
        ((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 41, size, size);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, step);
        ((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 41, size, size);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, step);
        ((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 72, size, size);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, step);
        ((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 72, size, size);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(143);
        for (i = 0; i != work->effect->count; i++) {
            if (frame == 10)
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = step;
        WaitFrames(1);
        frame++;
    } while (frame != 63);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

/* Battle effect: wipe the 128 by 128 canvas (16 by 16 tiles of 8 by 8
   bytes) with a ragged front. Each of the 128 lanes waits a random 0..63
   steps; the front then accelerates every frame, across the canvas when
   mode is 1 and down it otherwise, writing 1 - value into every pixel it
   passes. */
void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
    struct BattleEffectWork *work;
    u8 *canvas;
    u8 delay[128];
    s32 x;
    s32 y;
    s32 pos;

    canvas = (u8 *)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_CANVAS];
    work = (struct BattleEffectWork *)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    for (x = 0; x != 128; x++)
        delay[x] = Random16() & 0x3f;

    if (mode == 1) {
        s32 front;
        s32 speed;

        front = 0;
        speed = 1;
        x = 0;
        do {
            front += speed;
            speed++;
            for (; x != front; x++) {
                for (y = 0; y != 128; y++) {
                    pos = x - delay[y];
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] = 1 - value;
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        } while (front <= 256);
    } else {
        s32 front;
        s32 speed;

        front = 0;
        speed = 1;
        y = 0;
        do {
            front += speed / 2;
            speed += 4;
            for (; y != front; y++) {
                for (x = 0; x != 128; x++) {
                    pos = y - delay[x];
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] = 1 - value;
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        } while (front <= 191);
    }
}

/* Battle presentation: once per frame, flush the effect canvas to VRAM when
   an effect marked it pending, in the transfer mode the effect chose (plain
   copy, copy and clear, one of two blends, or a fade by amount); otherwise
   count the frames since the last flush. */
void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    void **heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    struct BattleEffectWork *work = heap_cache[0];
    void *source;
    s32 *counter;
    s32 next_counter;

    if (work->transfer_pending == 1) {
        source = heap_cache[1];
        switch ((u32)work->transfer_mode) {
        case 0:
            CopyWords((void *)0x06004000, source, 0x4000);
            break;
        case 1:
            CopyWords((void *)0x06004000, source, 0x4000);
            FillWords(source, 0x4000, work->transfer_value);
            break;
        case 2:
            if (work->transfer_value == 50) {
                ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
            } else {
                ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
            }
            break;
        case 3:
            ColorBuffer_BackupAndDarken(source, work->transfer_value,
                (void *)0x06004000, 0x4000);
            break;
        case 4:
            ColorBuffer_BackupAndBrighten(source, work->transfer_value,
                (void *)0x06004000, 0x4000);
            break;
        }
        work->transfer_pending = 0;
        counter = &work->frames_since_transfer;
        next_counter = 1;
    } else {
        counter = &work->frames_since_transfer;
        next_counter = *counter + 1;
    }
    *counter = next_counter;
}

/* Flush the battle compositor's pending display transfer. */
void BattleFx_FlushPendingGraphicsTransfer(void)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *source;
    s32 transfer_mode;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    work = heap_cache[0];
    source = Ram_MapCellBuffer;
    if (work->transfer_pending != 1)
        return;

    transfer_mode = work->transfer_mode;
    switch (transfer_mode) {
    case 0:
        CopyWords((void *)0x06008000, source, 0x7800);
        break;
    case 1:
        CopyWords((void *)0x06008000, source, 0x7800);
        FillWords(source, 0x7800, work->transfer_value);
        break;
    case 2:
        if (work->transfer_value == 50) {
            ColorBuffer_BackupAndHalve(source, (void *)0x06008000, 0x7800);
        } else {
            ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
        }
        break;
    case 3:
        ColorBuffer_BackupAndDarken(source, work->transfer_value,
            (void *)0x06008000, 0x7800);
        break;
    }

    work->transfer_pending = 0;
}
