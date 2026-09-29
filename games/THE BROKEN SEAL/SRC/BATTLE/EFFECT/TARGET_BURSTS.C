#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
extern u8 gBattleFxWork[];
extern u8 gCameraWork[];

/*
 * Battle effect: a burst of six particles over each affected unit in turn.
 * Every particle starts with a random polar velocity except each group's
 * sixth, which only falls. For 96 frames, unit i's group is drawn from
 * frame i * 8 for 40 frames above the unit: five 24x48 cels from the first
 * sheet under light gravity and the sixth from the second sheet under
 * heavier gravity, each cel advancing every six frames.
 */

/* Resource ids the reference loads from its literal pool. */
extern u8 Value_0000009e;
extern u8 Value_0000006c;
extern u8 Value_000000bb;

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
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
    Resource_LoadAndDecompress((s32)&Value_0000009e, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_0000006c, (u8 *)work + 0x1b00, 0, 0);
    palette = Resource_GetTableEntry((s32)&Value_000000bb);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    rectangle_a = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 3, 2);
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
        s32 facing;

        facing = *(s32 *)gCameraWork;
        for (member = 0; member != work->effect->count; member++) {
            void *member_object;

            member_object = *GetBattleObjectSlotFar(work->effect->actors[member]);
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            record[0] = *(s32 *)((u8 *)member_object + 8);
            record[1] = 160 << 13;
            record[2] = *(s32 *)((u8 *)member_object + 16);
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
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
