#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "MOTION_OBJECT.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"

extern u8 gBattleFxWork[];


void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);

/* Three sparks orbit each affected unit for 64 frames from frame i * 16,
   drawn through the first blitter of the pair. */
void BattleFx_RunOrbitingSparks(void)
{
    /* FAKEMATCH: the existing relative heap-cell transport preserves load and literal ordering; independent typed slot loads change those instructions. */
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition center;
    struct EffectPosition pos;
    s32 frame;
    DrawRectangleFn draw[2]; /* FAKEMATCH: only draw[0] is used; the pair is the reference's frame layout */
    s32 member;
    struct BattleCamera *camera;
    u8 *palette;
    s32 x_offset;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)(gWorkSlot + HEAP_SLOT_CAMERA * sizeof(void *));
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000052 = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    Resource_DecodeType01(palette + 128, work);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    draw[0] = (DrawRectangleFn)heap_cache[7];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 0)
        x_offset = 0;
    else
        x_offset = -112;
    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer */
    do {
        *(volatile s32 *)0x04000028 = x_offset << 8;
    } while (0);
    for (i = 0; i != 64; i++) {
        struct EffectStep *step = &work->particles[i];

        step->x = 0;
        step->y = 0;
        step->velocity_x = 0;
        step->z = 4;
    }
    for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *member_object;
            u32 window;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &pos);
            window = frame - member * 16;
            pos.x += x_offset;
            if (window < 64) {
                /* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp */
                register s32 *rec asm("r0");

                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                rec = record;
                rec[0] = member_object->x;
                rec[1] = member_object->y;
                rec[2] = member_object->z;
                EffectPosition_ApplyBaseAndYOffset(rec, &center);
                /* FAKEMATCH: sched2 barrier, so this store precedes the spark counter as in the reference */
                do {
                    center.x += x_offset;
                } while (0);
                for (i = 0; i != 3; i++) {
                    struct EffectStep *spark = &work->particles[member * 3 + i];
                    s32 x;
                    s32 y;

                    x = pos.x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    y = pos.y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    spark->velocity_x += 0x200;
                    draw[0](canvas, (u8 *)work + i * 0x240, x - 12, y - 28, 24, 24);
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

/*
 * Battle effect: a burst of six particles over each affected unit in turn.
 * Every particle starts with a random polar velocity except each group's
 * sixth, which only falls. For 96 frames, unit i's group is drawn from
 * frame i * 8 for 40 frames above the unit: five 24x48 cels from the first
 * sheet under light gravity and the sixth from the second sheet under
 * heavier gravity, each cel advancing every six frames.
 */

/* Resource ids the reference loads from its literal pool. */

void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);

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
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage, (u8 *)work + 0x1b00, 0, 0);
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
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != 96; frame++) {
        s32 member;
        struct BattleCamera *camera;

        camera = gCameraWork;
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *member_object;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
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
                            (u8 *)work + cel * 0x480 + 0x1b00,
                            screen.x - 12, screen.y - 36, 24, 48);
                    } else {
                        EffectStep_AdvanceWithGravity3D(p, 60, 0x200);
                        ((DrawRectangleFn)rectangle_b)(canvas,
                            (u8 *)work + cel * 0x480,
                            screen.x - 12, screen.y - 36, 24, 48);
                    }
                    p->variant += 1;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
