#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MOTOBJ.H"
#include "MEM.H"
#include "BTLDISP.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "IO_REG.H"
#include "TRANSFORM.H"

#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)

#define gBattleFxWork ((void *(*))&gBattleFxWork)

void AudioCommand_PlayFar(s32 cue);

extern void *gWorkSlot[];

void BattleFx_RunBouncingSheep(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];

    struct EffectStep *one;
    struct EffectStep *sheep;
    s32 i;
    s32 frame;
    DrawRectangleFn draw_b;
    DrawRectangleFn draw_a;
    s32 member;

    sheep = (struct EffectStep *)Ram_MapCellBuffer;
    heap_cache = gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw_a = (DrawRectangleFn)heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 15, 1);
    draw_b = (DrawRectangleFn)heap_cache[8];
    for (i = 0; i != 32; i++) {
        one = &sheep[i];
        one->x = ((Random16() & 63) + 32) << 16;
        one->y = -0x200000;
        one->velocity_y = Random16() & 0;
        one->z = Random16() & 3;
        one->variant = Random16() & 255;
    }
    if (work->effect->side == 1)
        REG_BG2X = -0x7000;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    AudioCommand_PlayFar(142);
    for (frame = 0; frame != 148; frame++) {
        struct BattleCamera *camera;

        camera = gCameraWork;
        if (frame == 80)
            BattleEventRuntime_BeginPhaseFar(0);
        for (member = 0; member != work->effect->count; member++) {
            void *member_object;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
            record[0] = *(s32 *)((u8 *)member_object + 8);
            record[1] = 160 << 14;
            record[2] = *(s32 *)((u8 *)member_object + 16);
            SceneTransform_ApplyPosition(record);
            if (frame == member * 16 + 64)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
        }
        one = sheep;
        for (i = 0; i != 12; i++, one++) {
            if (frame > i * 4 && one->y <= 0x7fffff) {
                s32 cel;

                cel = (one->variant / 16) & 7;
                if (cel < 4)
                    draw_a(canvas, work->sheet + (cel << 10),
                        (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
                else
                    draw_b(canvas, work->sheet + (cel << 10) - 0x1000,
                        (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
                one->y += one->velocity_y;
                one->velocity_y += 0x2000;
                one->variant += one->z;
                if (one->y > 0x5c0000 && one->velocity_y == 0) {
                    s32 speed = one->velocity_y + 1;
                    one->y = 0x5c0000;
                    one->z += 4;
                    one->velocity_y = -speed / 2;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx)
{
    void **slot;
    struct BattleEffectWork *work;
    void *canvas;
    /* FAKEMATCH: needed to match: the family's blitter pair: this effect loads only one */
    DrawRectangle draw[2];
    struct EffectStep *marker;
    s32 i;
    s32 frame;
    s32 start;
    s32 cel;
    s32 y;
    struct EffectPosition pos;

    slot = &gWorkSlot[40];
    canvas = slot[0];
    work = slot[-1];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1, 1);
    if (work->effect->side == 1) {
        REG_BG2X = -0x7000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    draw[0] = (DrawRectangle)slot[6];
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != work->effect->count; i++) {
        work->particles[i].variant = Random16() & 63;
    }
    frame = 0;
    while (frame != (work->effect->count << 5) + 32) {
        if (frame == 32) {
            BattleEventRuntime_BeginPhaseFar(0);
        }
        i = 0;
        if (work->effect->count != 0) {
            marker = work->particles;
            do {
                start = i << 4;
                if (frame == start) {
                    AudioCommand_PlayFar(143);
                }
                if (frame < start) {
                    goto next;
                }
                if (frame >= start + 72) {
                    goto next;
                }
                EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], &pos);
                if (work->effect->side == 1) {
                    pos.x -= 112;
                }
                y = pos.y;
                pos.y = y - 16;
                draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
                if (frame < start) {
                    goto next;
                }
                cel = ((frame - start) + marker->variant) / 6 % 9;
                draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
            next:
                marker++;
                i++;
            } while (i != work->effect->count);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
