#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern s32 gCameraWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);

extern u16 ParticleStreams_CellOffsets[];
extern u16 BattleFx_PuffCells[];
extern u8 BattleFx_PuffSizes[];

/* Battle effect: sparks burst from the acting unit and are pulled back to
   it while a djinni travels to the first target; then a cloud of 64 sparks
   closes in on every affected unit, eight frames apart. */
void BattleFx_RunSparkTravel(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 member;
    void *sheet;
    DrawRectangle *draw;
    struct MotionObject *actor;
    struct MotionObject *goal;
    s32 frame;
    s32 i;
    s32 position[3];
    s32 target[3];
    s32 delta[3];
    s32 rider[3];
    struct EffectPosition screen;
    s32 point[3];
    struct EffectPosition view;
    DrawRectangle callbacks[2];

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw = callbacks);
    actor = GetBattleObjectSlotFar(work->effect->actor)->object;
    goal = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 64; i++) {
        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 heading = Random16() & 0xffff;
        s32 speed = (Random16() & 0xff) + 128;

        spark->x = 0;
        spark->y = ((Random16() & 31) + 20) << 16;
        spark->z = 0;
        spark->velocity_x = (Trig_Sin(heading) * speed) >> 5;
        spark->velocity_y = 0;
        spark->velocity_z = (Trig_Cos(heading) * speed) >> 5;
        spark->variant = 0;
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    position[0] = actor->x;
    position[1] = 0;
    position[2] = actor->z;
    target[0] = goal->x;
    target[1] = 180 << 15;
    target[2] = 0;
    delta[0] = (target[0] - position[0]) / 40;
    delta[1] = (target[1] - position[1]) / 40;
    delta[2] = (target[2] - position[2]) / 40;
    for (frame = 0; frame != 98; frame++) {
        s32 facing = gCameraWork[0];
        if (frame == 8)
            Audio_PlayCue(212);
        if (frame == 80)
            Audio_PlayCue(142);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame >= 30 && frame < 70) {
            position[0] += delta[0];
            position[1] += delta[1];
            position[2] += delta[2];
        }
        SceneTransform_ApplyPosition(position);
        if (frame == 0)
            ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
        if (frame == 24)
            ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 32; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            if (frame > i && spark->variant == 0) {
                s32 size;
                Graphics_SaveTransferWorkOnce();
                switch (i & 3) {
                case 0:
                    SceneTransform_ApplyYaw(frame * (i * 32 + 256));
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
                    break;
                case 2:
                    SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
                    SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
                    break;
                }
                EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &screen);
                screen.x >>= 1;
                Graphics_RestoreTransferWork();
                if (screen.depth < 250)
                    screen.depth = 250;
                if (screen.depth > 634)
                    screen.depth = 634;
                size = 9 - (screen.depth - 250) / 64;
                callbacks[0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    screen.x - size / 2, screen.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(spark, 60, 0);
                if (frame > i + 30) {
                    s32 dx = -spark->x >> 8;
                    s32 dy = -spark->y >> 8;
                    s32 dz = -spark->z >> 8;

                    spark->velocity_x += dx;
                    spark->velocity_y += dy;
                    spark->velocity_z += dz;
                }
            }
        }
        if (frame > 82) {
            rider[0] = 0;
            rider[1] = Trig_Sin(frame << 10) << 2;
            rider[2] = 0;
            EffectPosition_ApplyBaseAndYOffset(rider, &screen);
            screen.x >>= 1;
            draw[1](canvas, work, screen.x - 10, screen.y - 17, 20, 34);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    for (i = 0; i != 512; i++) {
        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        spark->x = ((Random16() & 0xff) - 127) << 15;
        spark->y = ((Random16() & 0x7f) + 64) << 15;
        spark->z = ((Random16() & 0xff) - 127) << 15;
        spark->variant = 0;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
    for (frame = 0; frame != work->effect->count * 8 + 72; frame++) {
        s32 facing = gCameraWork[0];
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame >= work->effect->count * 8 + 40)
            target[1] += 0x40000;
        point[0] = target[0];
        point[1] = target[1];
        point[2] = target[2] + Trig_Sin(frame << 11) * 40;
        EffectPosition_ApplyBaseAndYOffset(point, &view);
        view.x >>= 1;
        callbacks[0](canvas, work, view.x - 10, view.y - 17, 20, 34);
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            s32 start = member * 8;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            position[0] = object->x;
            position[1] = 160 << 14;
            position[2] = object->z;
            SceneTransform_ApplyPosition(position);
            if (frame == start + 30)
                Audio_PlayCue(126);
            if (frame == start + 40)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, -1, 0);
            if (frame == start + 64)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 0, -1, -1, 0);
            if (frame > start) {
                SceneTransform_ApplyYaw((frame - start) << 9);
                for (i = 0; i != 64; i++) {
                    struct EffectStep *spark =
                        &((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];

                    if (frame > start + i / 2) {
                        s32 xx = (spark->x >> 8) * (spark->x >> 8);
                        s32 yy = (spark->y >> 8) * (spark->y >> 8);
                        s32 zz = (spark->z >> 8) * (spark->z >> 8);
                        s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
                        if (distance != 0) {
                            s32 cel;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &view);
                            view.x >>= 1;
                            if (view.depth < 314)
                                view.depth = 314;
                            if (view.depth > 634)
                                view.depth = 634;
                            /* FAKEMATCH: the size series A draws its sparks at, kept
                               from that effect and never used here; without it
                               the loop is short enough for the view's address
                               to leave it, and 268 instructions differ. */
                            cel = 3 - (view.depth - 314) / 128;
                            cel = (i * 4 + frame) % 9;
                            {
                                u8 *cell = (u8 *)sheet + BattleFx_PuffCells[cel];
                                u32 edge = BattleFx_PuffSizes[cel];
                                u32 half = edge >> 1;

                                draw[1](canvas, cell, view.x - half, view.y - half,
                                    edge, edge);
                            }
                            spark->x -= spark->x / distance;
                            spark->y -= spark->y / distance;
                            spark->z -= spark->z / distance;
                        }
                    }
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
