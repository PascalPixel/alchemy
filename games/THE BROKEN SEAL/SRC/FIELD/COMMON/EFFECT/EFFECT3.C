#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "MAP_SCROLL.H"

s32 Render_ProjectPoint(s32 *, s32 *);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);

extern u8 *gCameraWork;
s32 **GetBattleObjectSlotFar(s32 unit);
u8 *GetMotionRecordFar(s32 *object, s32 mode);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(u8 *source, u8 *destination);
s32 Render_ProjectPoint(s32 *point, s32 *screen);

extern u8 gBattleFxWork[];
extern u8 Data_03001ae8[];

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
void *BattleFx_RunCastingImpact(s32 *);

s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->z = (s32)((u32)step->z + (u32)step->velocity_z);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
    step->velocity_z = (s32)((u32)step->velocity_z * (u32)damping) / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position)
{
    s32 result = Render_ProjectPoint(point, &position->x);
    position->y = (s32)((u32)position->y - 0x10);
    return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleUnit_ProjectToScreen(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectScaledPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* Projects a battle unit's position to the screen and lifts it by its
   scaled height. */
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen)
{
    u8 *camera;
    s32 *object;
    u8 *info;
    s32 scale;
    /* FAKEMATCH: an unused vector reproduces the reference's 12-byte frame. */
    s32 unused[3];

    camera = gCameraWork;
    object = *GetBattleObjectSlotFar(unit);
    info = GetMotionRecordFar(object, 0);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera, camera + 12);
    scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen), *(s32 *)(info + 24));
    screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
    return 0;
}

void ObjectGroup_ProbeKeysWhenField24High(void)
{
    u8 *state = *(u8 **)((u32)&gBattleFxWork);
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7f)
        (void)*(volatile s32 *)((u32)&Data_03001ae8);
}

void BattleFx_DispatchByIdRange(s32 *arg0)
{
  s32 no;
  s32 tmp;
  tmp = (tmp = 0x60E);
  Runtime_AllocateBlock(0x29, tmp);
  Runtime_AllocateHeapBlock(0x27, 0x782C);
  Runtime_AllocateHeapBlock(0x28, 0x4000);
  tmp = *arg0;
  no = tmp;
  tmp = no - 0x64;
  if (((u32)tmp) <= 0x23U)
  {
    BattleFx_RunCastingImpact(arg0);
  } else
    if (no > 0xC7)
  {
    BattlePres_RunRingAndSparkScene((struct BattleEffectArgument *)arg0);
  } else
  {
    BattlePres_RunBeamSequence((struct BattleEffectArgument *)arg0);
  }
  Runtime_ReleaseHeapBlock(0x28);
  Runtime_ReleaseHeapBlock(0x27);
  Runtime_ReleaseHeapBlock(0x29);
}

extern DrawRectangle gWorkSlot[];
extern u16 BattleFx6_FlareCells[];
/* The third kind of beam, cut into six cells: how wide and tall each is,
   where it starts in the sheet and how far it reaches from the actor. */
extern u8 BeamSequence_CellWidths[];
extern u8 BeamSequence_CellHeights[];
extern u16 BeamSequence_CellSheetOffsets[];
extern u8 BeamSequence_CellReaches[];

void BattlePres_ConfigureEffectDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 index);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattlePres_SetupTransitionAtPairMidpointFar(s32 actor, s32 target, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object, s32 value);
void AudioCommand_PlayFar(s32 cue);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void BattlePresentation_SetPaletteLevelFar(s32 background, s32 level);

/* Battle presentation: a beam strikes the target. The beam's picture runs
   beside the actor, a flash plays on the target, and sparks fly from it and
   fall under gravity. The effect's kind picks the beam sheet and how it is
   cut into frames. */
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    struct EffectPosition anchor;
    struct EffectPosition point;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 shake;
    u8 *sheet;
    struct BattleCamera *camera;
    s32 kind;
    struct BattleUnit *unit;
    DrawRectangle draw[2];
    struct MotionObject *target;
    void *colors = (void *)0x05000000;
    s32 half;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    kind = effect->kind;
    work->effect = effect;
    unit = Owner_GetStateFar(effect->actor);
    WaitFrames(1);
    BattlePres_ConfigureEffectDisplay();
    BattleFx_SetupCanvasTileMap();
    *(volatile u16 *)0x0400000a = 0x1f80;
    WaitFrames(1);
    if (kind == 5) {
        if (work->effect->side == 0) {
            BattleEffect_LoadWork(46, 7, 7, 11, 3);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
        } else {
            BattleEffect_LoadWork(46, 7, 7, 15, 3);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
        }
    } else {
        if (work->effect->side == 0) {
            BattleEffect_LoadWork(46, 7, 7, 3, 3);
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
        } else {
            BattleEffect_LoadWork(46, 7, 7, 7, 3);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
        }
    }
    draw[0] = gWorkSlot[46];
    draw[1] = gWorkSlot[47];
    WaitFrames(1);
    if (kind == 4) {
        Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work, 1, 1);
    } else if (kind == 3) {
        Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work, 0, 0);
    } else {
        switch (kind) {
        case 0:
        case 1:
        case 5:
            Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
            break;
        case 2:
            Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
            break;
        }
    }
    if (work->effect->actor > 7)
        Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    else
        Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
    WaitFrames(1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    *(volatile u16 *)0x0400000a = 0x1f81;
    EffectPosition_ApplyAnimationAndYOffset(work->effect->actors[0], &anchor);
    if (work->effect->side == 0)
        shake = 96 - anchor.x;
    else
        shake = 32 - anchor.x;
    if (shake > 0)
        shake = 0;
    if (shake < -128)
        shake = -128;
    anchor.x += shake;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    WaitFrames(1);
    target = (struct MotionObject *)*GetBattleObjectSlotFar(work->effect->actors[0]);
    half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
    for (i = 0; i != 64; i++) {
        struct EffectStep *spark = &work->particles[i];

        spark->x = target->x;
        spark->y = target->y + half;
        spark->z = target->z;
        spark->velocity_x = (Random16() & 255) << 10;
        spark->velocity_y = ((Random16() & 255) - 32) << 10;
        spark->velocity_z = ((Random16() & 255) - 127) << 10;
        if (spark->x > 0)
            spark->velocity_x = -spark->velocity_x;
        spark->velocity_x = -spark->velocity_x;
        spark->variant = i + 16;
    }

    for (frame = 0; frame != 32; frame++) {
        if (frame == 5) {
            if (Summon_IsEntrySecondaryFlaggedFar(unit->class_id))
                BattleEventRuntime_BeginPhaseFar(134);
            else
                BattleEventRuntime_BeginPhaseFar(133);
        }
        if (frame == 4)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
        pos.y += 16;
        if (kind == 4) {
            if (frame <= 11) {
                if (work->effect->side == 0)
                    draw[1](canvas, (u8 *)work + (5 - frame / 2) * 0x300,
                        pos.x + shake - 48, pos.y - 8, 48, 16);
                else
                    draw[1](canvas, (u8 *)work + (5 - frame / 2) * 0x300,
                        pos.x + shake, pos.y - 8, 48, 16);
            }
        } else if ((u32)kind <= 2 || kind == 5) {
            if (frame <= 11) {
                if (work->effect->side == 0)
                    draw[1](canvas, (u8 *)work + frame / 2 * 0xd80,
                        pos.x + shake - 48, pos.y - 40, 48, 72);
                else
                    draw[1](canvas, (u8 *)work + frame / 2 * 0xd80,
                        pos.x + shake, pos.y - 40, 48, 72);
            }
        } else {
            if (frame <= 17) {
                s32 cell = frame / 3;

                if (work->effect->side == 0)
                    draw[1](canvas, (u8 *)work + BeamSequence_CellSheetOffsets[cell],
                        pos.x + BeamSequence_CellReaches[cell] + shake - 58,
                        pos.y - BeamSequence_CellHeights[cell] / 2,
                        BeamSequence_CellWidths[cell], BeamSequence_CellHeights[cell]);
                else
                    draw[1](canvas, (u8 *)work + BeamSequence_CellSheetOffsets[cell],
                        pos.x - BeamSequence_CellReaches[cell] + shake - BeamSequence_CellWidths[cell] + 58,
                        pos.y - BeamSequence_CellHeights[cell] / 2,
                        BeamSequence_CellWidths[cell], BeamSequence_CellHeights[cell]);
            }
        }
        if (frame >= 4 && frame <= 15)
            draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
                anchor.x - 16, anchor.y - 24, 40, 48);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((u8 *)camera, (u8 *)camera->pos);
        if (frame >= 4 && frame <= 31) {
            for (i = 0; i != 64; i++) {
                s32 pair = i / 2;
                struct EffectStep *spark = &work->particles[pair];
                s32 size = spark->variant;

                if (size > 0) {
                    Render_ProjectPoint(&spark->x, &point.x);
                    point.x += shake;
                    size >>= 3;
                    size += 2;
                    draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1],
                        point.x - size, point.y - size, size * 2, size * 2);
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
    BattleFx_SetTransitionFlagAndDisplay();
}

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
        object = (struct MotionObject *)*GetBattleObjectSlotFar(work->effect->actor);
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
    target = (struct MotionObject *)*GetBattleObjectSlotFar(work->effect->actors[0]);
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
        Graphics_PrepareTransferInIwramWork((u8 *)camera, (u8 *)camera->pos);
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
