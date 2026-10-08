#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "heap_state.h"
#include "canvas.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "effect_step.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "btldisp.h"
#include "callback_scheduler.h"
#include "system.h"
#include "io_reg.h"

#define ColorBuffer_BackupAndHalve ((void (*)(void*,void*,s32))ColorBuffer_BackupAndHalve)
#define ColorBuffer_BackupAndScaleThreeQuarters ((void (*)(void*,void*,s32))ColorBuffer_BackupAndScaleThreeQuarters)
#define ColorBuffer_BackupAndDarken ((void (*)(void*,s32,void*,s32))ColorBuffer_BackupAndDarken)
#define ColorBuffer_BackupAndBrighten ((void (*)(void*,s32,void*,s32))ColorBuffer_BackupAndBrighten)



static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: needed to match: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from push {r5, r6, lr} to push {r5, r6, r7, lr} */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from mov r0, r5 to lsl r1, r1, #7 */
    Iwram_FillWords(destination, size, value);
}


void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
    struct EffectPosition first;
    struct EffectPosition last;
    void *canvas;
    struct BattleEffectWork *work;
    s32 size;
    s32 step;

    /* FAKEMATCH: the frame counter is pinned to r9, as the reference keeps it. */
    register s32 frame __asm__("r9");
    s32 i;

    /* FAKEMATCH: needed to match: size and step first hold the slot table and blend register addresses, sharing their registers */
    size = (s32)&Ram_WorkSlot[40];
    canvas = *(void **)size;
    work = *(struct BattleEffectWork **)(size - 4);
    work->effect = efx;
    BattleFx_BeginCanvasLayer(2);
    step = 0x04000052;
    REG_BG2PA = 0x100;
    *(u16 *)step = 0x1000;
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
    first.x += (last.x - first.x) / 2;
    REG_BG2X = (64 - first.x) << 8;
    Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    AudioCommand_PlayFar(143);
    frame = 0;
    step = 1;
    size = 32;
    do {
        if (frame <= 8)
            REG_BLDALPHA = (frame << 1) | 0x1000;
        if (frame > 53)
            REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
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
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
    struct BattleEffectWork *work;
    u8 *canvas;
    u8 delay[128];
    s32 x;
    s32 y;
    s32 pos;

    canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
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

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    void **heap_cache = (void **)gBattleFxWork;
    struct BattleEffectWork *work = heap_cache[0];
    void *source;
    s32 *counter;
    s32 next_counter;

    if (work->transfer_pending == 1) {
        source = heap_cache[1];
        switch (work->transfer_mode) {
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

void BattleFx_FlushPendingGraphicsTransfer(void)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *source;
    s32 transfer_mode;

    heap_cache = (void **)gBattleFxWork;
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
