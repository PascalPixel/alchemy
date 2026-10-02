/* Draft, complete main:080e40a4 [080e40a4,080e46f0), 1612 bytes, written
   fresh from the listing in plain C. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "MAP_SCROLL.H"

extern u8 gBattleFxWork[];
extern DrawRectangle gWorkSlot[];
extern u16 BattleFx6_FlareCells[];

void BattlePres_SetupTransitionAtPairMidpointFar(s32 actor, s32 target, s32 mode);
void BattleFx_SetupCanvasTileMap(void);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 id);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object, s32 value);
void AudioCommand_PlayFar(s32 cue);
void Render_ResetTransformState(void);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
s32 Battle_GetObjectTableValueFar(s32 id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void BattlePresentation_SetPaletteLevelFar(s32 background, s32 level);
void BattleFx_SetTransitionFlagAndDisplay(void);

/* Battle presentation: a ring closes on the actor and bursts into sparks on
   the target. When the effect's kind is above 199, sixty-four motes first
   circle the actor on three random angles, each closing in by four a frame,
   while the actor stands still and the palette fades. Then four full-canvas
   pictures flash by two frames each, and sparks fly from the target and fall
   under gravity. The background's palette level steps back up at the end. */
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    struct EffectPosition anchor;
    struct EffectPosition point;
    struct EffectPosition spot;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 shake;
    u8 *sheet;
    struct BattleCamera *camera;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 acceleration;
    s32 strength;
    DrawRectangle draw[2];
    struct MotionObject *object;
    struct MotionObject *target;
    u16 *background;
    void *colors = (void *)0x05000000;
    s32 words = 0x4000;
    s32 big;
    s32 half;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    big = 1;
    if (effect->kind <= 199)
        big = 0;
    work->effect = effect;
    BattlePres_SetupTransitionAtPairMidpointFar(effect->actor, effect->unknown_000c, 130);
    WaitFrames(1);
    BattleFx_SetupCanvasTileMap();
    *(volatile u16 *)0x0400000a = 0x1f80;
    if (work->effect->side == 0) {
        BattleEffect_LoadWork(46, 7, 7, 3, 3);
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 7, 3);
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
    }
    draw[0] = gWorkSlot[46];
    draw[1] = gWorkSlot[47];
    BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor, work->effect->unknown_000c, 130);
    WaitFrames(1);
    Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet, work, 1, 0);
    BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor, work->effect->unknown_000c, 130);
    WaitFrames(1);
    Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet, Ram_MapCellBuffer, 1, 1);
    if (work->effect->actor > 7)
        Iwram_CopyWords(colors,
            Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor, work->effect->unknown_000c, 130);
    WaitFrames(1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
    BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor, work->effect->unknown_000c, 130);
    WaitFrames(1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    if (big == 1) {
        object = GetBattleObjectSlotFar(work->effect->actor)->object;
        for (i = 0; i != 64; i++) {
            struct EffectStep *mote = &work->particles[i];

            mote->x = (Random16() & 63) + 16;
            mote->y = 0;
            mote->z = 0;
            mote->velocity_x = Random16() & 0xffff;
            mote->velocity_y = Random16() & 0xffff;
            mote->velocity_z = Random16() & 0xffff;
        }
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
        velocity_x = object->velocity_x;
        velocity_y = object->velocity_y;
        velocity_z = object->velocity_z;
        strength = object->vertical_motion_strength;
        acceleration = object->acceleration;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->acceleration = 0;
        object->vertical_motion_strength = 0;
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
        shake = 64 - pos.x;
        gBgScroll[1].x = shake;
        gBgScroll[1].y = 80;
        work->fade_frames = 24;
        work->fade_step = 0;
        Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
        AudioCommand_PlayFar(212);
        for (frame = 0; frame != 32; frame++) {
            BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                work->effect->unknown_000c, 130);
            for (i = 0; i != 64; i++) {
                struct EffectStep *mote = &work->particles[i];

                if (mote->x >= 0 && frame >= i / 4) {
                    s32 size = (i & 1) + 5;

                    Render_ResetTransformState();
                    SceneTransform_ApplyRoll(mote->velocity_z);
                    SceneTransform_ApplyPitch(mote->velocity_x);
                    SceneTransform_ApplyYaw(mote->velocity_y);
                    EffectPosition_ApplyBaseAndYOffset(&mote->x, &point);
                    point.x += 64;
                    point.y = point.y + pos.y + 24;
                    if (point.depth < -60)
                        point.depth = -60;
                    if (point.depth > 60)
                        point.depth = 60;
                    point.depth += 60;
                    draw[0](canvas, sheet + BattleFx6_FlareCells[size - 1],
                        point.x - size, point.y - size, size * 2, size * 2);
                    mote->x -= 4;
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }
        Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
        ObjectDispatch_ApplyValueToChildrenFar(object, 16);
        object->velocity_x = velocity_x;
        object->velocity_y = velocity_y;
        object->velocity_z = velocity_z;
        object->acceleration = acceleration;
        object->vertical_motion_strength = strength;
    }

    Iwram_ClearWords(canvas, words);
    Iwram_ClearWords((void *)0x06004000, words);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    *(volatile u16 *)0x0400000a = 0x1f81;
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
    if (work->effect->side == 0)
        shake = 32 - anchor.x;
    else
        shake = 96 - anchor.x;
    if (shake > 0)
        shake = 0;
    if (shake < -128)
        shake = -128;
    anchor.x += shake;
    gBgScroll[1].y = 80;
    gBgScroll[1].x = shake;
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
    for (i = 0; i != 64; i++) {
        struct EffectStep *spark = &work->particles[i];

        spark->x = target->x;
        spark->y = target->y + half;
        spark->z = target->z;
        spark->velocity_x = (Random16() & 255) << 10;
        spark->velocity_y = (Random16() & 255) << 10;
        spark->velocity_z = ((Random16() & 255) - 127) << 10;
        if (spark->x > 0)
            spark->velocity_x = -spark->velocity_x;
        spark->variant = i + 16;
    }

    for (frame = 0; frame != 32; frame++) {
        if (frame == 5)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 4)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
        pos.y += 16;
        if (frame <= 1)
            draw[0](canvas, work, 0, 0, 120, 120);
        else if (frame <= 3)
            draw[0](canvas, (u8 *)work + 0x3840, 0, 0, 120, 120);
        else if (frame <= 5)
            draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
        else if (frame <= 7)
            draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame >= 4 && frame <= 31) {
            for (i = 0; i != 64; i++) {
                s32 pair = i / 2;
                struct EffectStep *spark = &work->particles[pair];
                s32 size = spark->variant;

                if (size > 0) {
                    EffectPosition_ApplyBaseAndYOffset(&spark->x, &spot);
                    spot.x += shake;
                    size >>= 3;
                    size += 2;
                    spot.y += 16;
                    draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1],
                        spot.x - size, spot.y - size, size * 2, size * 2);
                    EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
                    spark->variant--;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    gBgScroll[1].y = 32;
    for (frame = 0, background = &gBattleWork->background; frame != 7; frame++) {
        BattlePresentation_SetPaletteLevelFar(*background, 6 - frame);
        WaitFrames(1);
    }
    BattleFx_SetTransitionFlagAndDisplay();
}
