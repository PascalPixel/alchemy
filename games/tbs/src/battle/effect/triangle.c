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
#include "motobj.h"
#include "canvas.h"
#include "resource.h"
#include "btldisp.h"
#include "types.h"
#include "iwram_call.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "io_reg.h"
#include "menu_list.h"
#include "transform.h"

#define SceneTransform_ApplyScale ((void (*)(struct Vector3*))SceneTransform_ApplyScale)

void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 a, s32 b, s32 c, s32 *x, s32 *y);

void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect)
{
    struct EffectPosition anchor;
    struct Vector3 vector;
    struct EffectPosition screen;
    struct Vector3 scale;
    s32 origin_x;
    s32 origin_y;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 member;
    struct BattleCamera *camera;
    s32 shift;
    u8 *graphics;
    u8 *palette;
    struct MotionObject *object;
    s32 tick;
    struct EffectStep *point;
    s32 size;
    s32 j;
    s32 k;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    if (work->effect->prelude == 1)
        BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side, 0, &origin_x, &origin_y);
    REG_BG2PA = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), work->sheet + 0x1000);
    palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x2000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
    shift = 64 - anchor.x;
    REG_BG2X = shift << 8;
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
        if (frame == 64)
            BattleEventRuntime_BeginPhaseFar(0);
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (work->effect->prelude == 1) {
            s32 x;
            s32 y;

            x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
            y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin_y - 24;
            BattleFx_FetchRectangleBlitters(work->effect->side, draw);
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[0](canvas, work->sheet + 0x2000, x, y, 40, 40);
            if (frame <= 3)
                draw[1](canvas, work->sheet + 0x2000, x, y, 40, 40);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
        draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        for (member = 0; member != 1; member++) {
            tick = frame - member * 8;
            object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            if (tick >= 0 && tick < 96) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
                vector.x = object->x;
                vector.y = object->y;
                vector.z = object->z;
                EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
                screen.x = anchor.x + shift;
                screen.y -= 24;
                if (tick <= 67) {
                    for (j = 0; j != 3; j++) {
                        point = &work->particles[member * 32 + j];
                        Render_ResetTransformState();
                        if (tick <= 63) {
                            scale.x = 0x2a000 - (tick * 3 << 9);
                            scale.y = 0x2a000 - (tick * 3 << 9);
                            scale.z = 0x2a000 - (tick * 3 << 9);
                            SceneTransform_ApplyScale(&scale);
                            SceneTransform_ApplyRoll((64 - tick) << 9);
                            SceneTransform_ApplyYaw((64 - tick) << 9);
                        }
                        SceneTransform_ApplyRoll(j * 0x5555);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)SpinningTriangle_Vertex, (struct EffectPosition *)&vector);
                        point->velocity_x = vector.x + screen.x;
                        point->velocity_y = vector.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++) {
                        struct EffectStep *from = &work->particles[j + member * 32];
                        struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

                        size = 5 - tick / 16;
                        for (k = 0; k != 24; k++) {
                            s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
                            s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

                            draw[0](canvas,
                                work->sheet + BattleFx6_FlareCells[size - 1] + 0x1000,
                                x - size, y - size, size * 2, size * 2);
                        }
                    }
                }
                if (tick > 63) {
                    draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
                    draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
                }
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect)
{
    struct EffectPosition anchor;
    struct Vector3 vector;
    struct EffectPosition screen;
    struct Vector3 scale;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 member;
    struct BattleCamera *camera;
    s32 shift;
    u8 *graphics;
    u8 *palette;
    struct MotionObject *object;
    s32 tick;
    struct EffectStep *point;
    s32 size;
    s32 j;
    s32 k;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
    Resource_DecodeType01(palette, work->sheet + 0x1000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
    shift = 64 - anchor.x;
    REG_BG2X = shift << 8;
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
        if (frame == 64)
            BattleEventRuntime_BeginPhaseFar(0);
        if (frame == 46)
            BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 16, 0);
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
        draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        for (member = 0; member != 1; member++) {
            tick = frame - member * 8;
            object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            if (tick >= 0 && tick < 96) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
                vector.x = object->x;
                vector.y = object->y;
                vector.z = object->z;
                EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
                screen.x = anchor.x + shift;
                screen.y -= 24;
                if (tick <= 67) {
                    for (j = 0; j != 3; j++) {
                        point = &work->particles[member * 32 + j];
                        Render_ResetTransformState();
                        if (tick <= 63) {
                            scale.x = 0x2a000 - (tick * 3 << 9);
                            scale.y = 0x2a000 - (tick * 3 << 9);
                            scale.z = 0x2a000 - (tick * 3 << 9);
                            SceneTransform_ApplyScale(&scale);
                            SceneTransform_ApplyRoll((64 - tick) << 9);
                            SceneTransform_ApplyYaw((64 - tick) << 9);
                        }
                        SceneTransform_ApplyRoll(j * 0x5555);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)TriangleStrike_Vertex, (struct EffectPosition *)&vector);
                        point->velocity_x = vector.x + screen.x;
                        point->velocity_y = vector.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++) {
                        struct EffectStep *from = &work->particles[j + member * 32];
                        struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

                        size = 5 - tick / 16;
                        for (k = 0; k != 24; k++) {
                            s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
                            s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

                            draw[0](canvas,
                                work->sheet + BattleFx6_FlareCells[size - 1] + 0x1000,
                                x - size, y - size, size * 2, size * 2);
                        }
                    }
                }
                if (tick > 63) {
                    draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
                    draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
                }
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
