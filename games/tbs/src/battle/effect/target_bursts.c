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
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "system.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "transform.h"

#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)




void BattleFx_RunTargetBursts(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *palette;
    s32 frame;
    void *rectangle_b;
    void *rectangle_a;
    struct EffectStep *step;
    s32 i;
    struct EffectPosition screen;
    s32 record[3];

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage, work->sheet + 0x1b00, 0, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
    rectangle_a = heap_cache[7];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
    rectangle_b = heap_cache[8];

    for (i = 0; i != 64; i++) {
        s32 angle;
        s32 magnitude;

        step = &work->particles[i];
        angle = Random16() & 0xffff;
        magnitude = Random16() & 0xff;
        step->x = 0;
        step->y = 0;
        step->z = 0;
        if (i % 6 == 5) {
            step->velocity_x = 0;
            step->velocity_y = 0;
        } else {
            step->velocity_x = (Trig_Sin(angle) * magnitude) >> 7;
            step->velocity_y = (Trig_Cos(angle) * magnitude) >> 9;
        }
        step->velocity_z = 0;
        step->variant = 0;
    }

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != 96; frame++) {
        s32 member;
        struct BattleCamera *camera;

        camera = gCameraWork;
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *member_object;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
            record[0] = member_object->x;
            record[1] = 160 << 13;
            record[2] = member_object->z;
            SceneTransform_ApplyPosition(record);
            if (frame >= member * 8 && frame < member * 8 + 40) {
                struct EffectStep *p;

                for (i = 0; i != 6; i++) {
                    s32 cel;

                    p = &work->particles[member * 6 + i];
                    cel = p->variant / 6;
                    if (cel > 5)
                        cel = 5;
                    EffectPosition_ApplyBaseAndYOffset((s32 *)p, &screen);
                    screen.x >>= 1;
                    if (i == 5) {
                        EffectStep_AdvanceWithGravity3D(p, 62, 0x800);
                        ((DrawRectangleFn)rectangle_a)(canvas,
                            work->sheet + cel * 0x480 + 0x1b00,
                            screen.x - 12, screen.y - 36, 24, 48);
                    } else {
                        EffectStep_AdvanceWithGravity3D(p, 60, 0x200);
                        ((DrawRectangleFn)rectangle_b)(canvas,
                            work->sheet + cel * 0x480,
                            screen.x - 12, screen.y - 36, 24, 48);
                    }
                    p->variant += 1;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
