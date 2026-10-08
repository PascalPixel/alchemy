#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "canvas.h"
#include "mem.h"
#include "scene.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "heap_state.h"
#include "io_reg.h"


void BattleFx_RunSevenModeMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 4);
}

void BattleFx_RunSevenModeMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 5);
}

void BattleFx_RunSevenModeMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 0);
}

void BattleFx_RunSevenModeMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 1);
}

void BattleFx_RunSevenModeMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 6);
}

void BattleFx_RunSevenModeMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 2);
}

void BattleFx_RunSevenModeMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunSevenMode(effect, 3);
}

extern s8 SevenMode_Records[];
extern u8 SevenMode_ShardWidths[];
extern u8 SevenMode_ShardHeights[];
extern u16 SevenMode_ShardOffsets[];

#define gShots ((struct EffectStep *)Ram_MapCellBuffer)
#define gBursts ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

typedef s32 (*WordCopy)(void *, const void *, s32);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: needed to match: the inlined copy shares the routine address and rebuilds the palette address at each call */
    copy(destination, source, size);
}

void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    s32 total;
    struct MotionObject *source;
    struct BattleCamera *camera;
    u8 *palette;
    s32 i;
    s32 height;

    heap_cache = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x3e80);
    if (mode == 0)
        palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
    else if (mode == 1)
        palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
    else if (mode == 2)
        palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    else if (mode == 3)
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    else if (mode == 4)
        palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    else if (mode == 6)
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);

    for (i = 0; i != 1024; i++)
        gShots[i].variant = -1;
    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    height = 0x40000;
    for (k = 0; k != work->effect->count; k++) {
        struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

        for (i = 0; i != 16; i++) {
            struct EffectStep *shot = &gShots[k * 16 + i];

            shot->x = source->x;
            shot->y = height;
            shot->z = source->z;
            shot->velocity_x = (target->x - source->x) >> 4;
            shot->velocity_y = 0x40000;
            shot->velocity_z = (target->z - source->z) >> 4;
            shot->variant = 0;
        }
    }
    for (i = 0; i != 256; i++)
        gBursts[i].variant = -1;

    if (effect->side == 0) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    } else {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (SevenMode_Records[mode * 2] == 0)
        total = work->effect->count * 8 + 72;
    else
        total = work->effect->count * 8 + 56;
    AudioCommand_PlayFar(103);

    for (frame = 0; frame != total; frame++) {
        struct EffectStep *burst;

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        k = 0;
        if (work->effect->count != 0) {
            do {
                if (frame >= k * 8) {
                    struct EffectStep *shot = &gShots[k * 16];

                    if (frame == k * 8 + 17) {
                        ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 16);
                        BattleEventRuntime_BeginPhaseFar(133);
                    }
                    /* FAKEMATCH: needed to match: restores the CSE block boundary after the hit block (k << 3 is recomputed) */
                    do { } while (0);
                    if (shot->variant >= 0) {
                        s32 step = (frame - k * 8) / 3;

                        if (step > 9)
                            step = 9;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)shot, &position);
                        position.x >>= 1;
                        if (step > 4)
                            draw[0](canvas, work->sheet + step * 768, position.x - 16, position.y - 12, 32, 24);
                        else
                            draw[0](canvas, work->sheet + step * 768, position.x - 12, position.y - 16, 24, 32);
                        if (shot->variant == 0)
                            EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
                        if (shot->y < 0) {
                            s32 count;
                            s8 *records = SevenMode_Records;
                            s32 row;

                            shot->y = 0;
                            shot->variant = 1;
                            /* FAKEMATCH: needed to match: native loads the table address before the row (r0, r1) */
                            row = mode * 2;
                            if (records[row] == 0)
                                count = 4;
                            else
                                count = 16;
                            for (i = 0; i != count; i++) {
                                struct EffectStep *burst;
                                s32 group = k * 4;

                                /* FAKEMATCH: needed to match: restores the CSE block boundary between group's set and group * 8 */
                                do { } while (0);
                                burst = &gBursts[group * 8 + i];

                                burst->x = shot->x;
                                burst->y = shot->y;
                                burst->z = shot->z;
                                if (SevenMode_Records[mode * 2] == 0) {
                                    burst->velocity_x = ((Random16() & 63) - 32) << 11;
                                    burst->velocity_y = 0;
                                    burst->velocity_z = ((Random16() & 63) - 32) << 11;
                                } else {
                                    burst->velocity_x = ((Random16() & 63) - 32) << 13;
                                    burst->velocity_y = ((Random16() & 31) + 32) << 12;
                                    burst->velocity_z = ((Random16() & 63) - 32) << 13;
                                }
                                burst->variant = 0;
                            }
                        }
                    }
                }
                k++;
            } while (k != work->effect->count);
        }
        for (i = 0, burst = gBursts; i != 256; burst++, i++) {

            if ((u32)burst->variant <= 44 && burst->y >= 0) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)burst, &position);
                position.x >>= 1;
                if (SevenMode_Records[mode * 2] == 0) {
                    draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + 0x3e80,
                        position.x - 12, position.y - 24, 24, 48);
                } else {
                    s32 picture = burst->variant / 5;
                    s32 flip;

                    if (i & 1)
                        picture += 9;
                    flip = effect->side;
                    draw[burst->velocity_x > 0 ? flip ^ 1 : flip](canvas,
                        work->sheet + SevenMode_ShardOffsets[picture] + 0x1e00,
                        position.x - SevenMode_ShardWidths[picture] / 2,
                        position.y - SevenMode_ShardHeights[picture] / 2,
                        SevenMode_ShardWidths[picture], SevenMode_ShardHeights[picture]);
                }
                if (SevenMode_Records[mode * 2] == 0)
                    EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
                else
                    EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
                burst->variant++;
            }
        }
        Camera_ApplyShake(2, 2);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
