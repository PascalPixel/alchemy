#include "TRANSFORM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

struct BattleObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x14];
    s32 unknown_28;
    u8 unknown_2c[4];
    s32 unknown_30;
    s32 unknown_34;
    u8 unknown_38[0x10];
    s32 unknown_48;
    u8 unknown_4c[0xe];
    u8 unknown_5a;
};

struct CameraWork {
    u8 unknown_00[0x36];
    u16 scroll;
};

struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

extern u8 gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];
/* A ring's outer and inner point, by vertex parity. */
extern struct Vector3 RingBolts_Points[];
/* Outer and inner radius of a star outline, by vertex parity. */
extern u8 SpinningStars_Radii[];
extern u8 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyPosition(void *position);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale(struct Vector3 *scale);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 BattleFx_EndCanvasLayer(void);
struct BattleObject **GetBattleObjectSlotFar(s32 id);
void Object_SetMode(struct BattleObject *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct BattleObject *object, s32 value);
void Object_ResetMotion(struct BattleObject *object);
void Object_SetMoveTargetFar(struct BattleObject *object, s32 x, s32 y, s32 z);

/*
 * Three rings leave the acting unit twelve frames apart and fly to the first
 * target in twelve steps, growing as they go, while the camera scrolls for
 * the first 48 frames. Each ring is ten points spun about its centre and
 * joined by sixteen dots a side; on arrival it knocks the target back.
 * `base` is read before anything writes it: the sums it feeds are stored and
 * at once replaced, as the game's code has them.
 */
void BattleFx_RunRingBolts(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    struct EffectPosition base;
    struct Vector3 scale;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    s32 facing;
    struct BattleObject *caster;
    struct BattleObject *source;
    struct BattleObject *target;
    struct EffectStep *point;
    struct EffectStep *step;
    s32 start;
    s32 peak;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    facing = *(s32 *)(gWorkSlot + 12 * 4);
    caster = *GetBattleObjectSlotFar(effect->actor);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    Object_SetMode(caster, 2);
    ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw[0] = (DrawRectangle)heap_cache[7];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = *GetBattleObjectSlotFar(work->effect->actor);
    target = *GetBattleObjectSlotFar(work->effect->actors[0]);
    for (i = 0, point = work->particles; i != 3; i++) {
        point->x = source->x;
        point->y = source->y + 0x280000;
        point->z = source->z;
        if (i == 0)
            point->velocity_x = (target->x - point->x) / 12;
        else
            point->velocity_x = (target->x * 2 - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x280000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = 0;
        point++;
    }

    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 60; frame++) {
        if (frame <= 47) {
            struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + 12 * 4);
            s32 speed;

            if (frame <= 39)
                speed = 128;
            else
                speed = 0x300 - frame * 16;
            if (work->effect->side == 0)
                camera->scroll -= speed;
            else
                camera->scroll += speed;
        }
        for (i = 0; i != 3; i++) {
            start = i * 12;
            if (frame >= start) {
                step = &work->particles[i];
                size = (frame - start) / 4 + 2;
                if (size > 10)
                    size = 10;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                SceneTransform_ApplyPosition(step);
                peak = 0;
                for (j = 0; j != 10; j++) {
                    s32 depth;

                    Graphics_SaveTransferWorkOnce();
                    SceneTransform_ApplyRoll((frame - start) << 10);
                    SceneTransform_ApplyYaw(0x4000);
                    scale.x = ((frame - start) << 12) + 0x1000;
                    if (scale.x > 0x10000)
                        scale.x = 0x10000;
                    scale.y = scale.x;
                    scale.z = scale.x;
                    SceneTransform_ApplyScale(&scale);
                    SceneTransform_ApplyRoll(j * 0x199a);
                    depth = EffectPosition_ApplyBaseAndYOffset((s32 *)&RingBolts_Points[j & 1], &pos);
                    if (peak < depth)
                        peak = depth;
                    pos.x >>= 1;
                    trails[i * 10 + j].velocity_x = pos.x + base.x;
                    trails[i * 10 + j].velocity_y = pos.y + base.y;
                    trails[i * 10 + j].velocity_x = pos.x;
                    trails[i * 10 + j].velocity_y = pos.y;
                    Graphics_RestoreTransferWork();
                }
                if (peak <= 399999) {
                    for (j = 0; j != 10; j++) {
                        struct EffectStep *from = &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
                        struct EffectStep *to = &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

                        for (k = 0; k != 16; k++) {
                            s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 16;
                            s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 16;

                            draw[0](canvas,
                                graphics + ParticleStreams_CellOffsets[size - 1],
                                x - size / 2, y - size, size, size * 2);
                        }
                    }
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
                if (frame == start + i + 10) {
                    target->unknown_34 = 0x20000;
                    target->unknown_30 = 0x80000;
                    target->unknown_28 = 0x50000;
                    target->unknown_48 = 0xab85;
                    target->unknown_5a = 0;
                    Object_ResetMotion(target);
                    if (target->x < 0)
                        Object_SetMoveTargetFar(target, target->x - 0x280000, 0, target->z);
                    else
                        Object_SetMoveTargetFar(target, target->x + 0x280000, 0, target->z);
                    if (i == 2) {
                        BattleEventRuntime_BeginPhaseFar(134);
                    } else {
                        AudioCommand_PlayFar(134);
                        ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
                    }
                    work->shake_frames = 4;
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/*
 * Eight star outlines leave the acting unit eight frames apart, fly to the
 * first target in twelve steps and bounce off the ground there, shaking the
 * screen and knocking every affected unit. Each outline is ten vertices
 * spun about its point and joined by twelve dots a side.
 */
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    s32 facing;
    struct BattleObject *source;
    struct BattleObject *target;
    struct EffectStep *point;
    struct EffectStep *step;
    struct EffectPosition pos;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    facing = *(s32 *)(gWorkSlot + 12 * 4);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    draw[0] = (DrawRectangle)heap_cache[7];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = *GetBattleObjectSlotFar(work->effect->actor);
    target = *GetBattleObjectSlotFar(work->effect->actors[0]);
    for (i = 0, point = work->particles; i != 8; i++) {
        s32 delay = i * 8;

        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->velocity_x = (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x140000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = (s32)(Random16() & 15) + delay;
        point++;
    }

    size = 2;
    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 128; frame++) {
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(134);
        for (i = 0; i != 8; i++) {
            step = &work->particles[i];
            if (frame >= step->variant) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                EffectPosition_ApplyBaseAndYOffset((s32 *)step, &pos);
                pos.x >>= 1;
                if (pos.x >= -8 && pos.x <= 127) {
                    if (pos.y <= 127) {
                        if (pos.y >= -8) {
                            for (j = 0; j != 10; j++) {
                                trails[i * 10 + j].velocity_x = pos.x
                                    + ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1]) / 2 >> 16);
                                trails[i * 10 + j].velocity_y = pos.y
                                    - (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1] >> 16);
                            }
                            for (j = 0; j != 10; j++) {
                                struct EffectStep *from = &trails[j + i * 10];
                                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                                for (k = 0; k != 12; k++) {
                                    s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 12;
                                    s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 12;

                                    draw[0](canvas,
                                        graphics + ParticleStreams_CellOffsets[size - 1],
                                        x - size / 2, y - size, size, size * 2);
                                }
                            }
                        }
                    }
                }
                if (step->y <= 0x1dffff) {
                    step->velocity_y = -step->velocity_y;
                    step->velocity_x /= 2;
                    step->velocity_z /= 2;
                    work->shake_frames = 4;
                    AudioCommand_PlayFar(134);
                    for (j = 0; j != work->effect->count; j++)
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
