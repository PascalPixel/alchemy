#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"

extern void *gBattleFxWork[];
extern s32 gCameraWork;

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
void SceneTransform_ApplyPosition(s32 *position);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);

/* Sleep: thirty-two sheep drop onto the scene from above and bounce once
   they reach the floor, the first twelve drawn from frame i * 4; each
   affected unit is lifted on frame i * 16 + 64. The flock lives in the
   shared map cell buffer, which the code addresses as a constant. */
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
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work, 1, 1);
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
        *(volatile s32 *)0x04000028 = -0x7000;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(142);
    for (frame = 0; frame != 148; frame++) {
        s32 facing;

        facing = gCameraWork;
        if (frame == 80)
            BattleEventRuntime_BeginPhaseFar(0);
        for (member = 0; member != work->effect->count; member++) {
            void *member_object;

            member_object = *GetBattleObjectSlotFar(work->effect->actors[member]);
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
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
                    draw_a(canvas, (u8 *)work + (cel << 10),
                        (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
                else
                    draw_b(canvas, (u8 *)work + (cel << 10) - 0x1000,
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
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
