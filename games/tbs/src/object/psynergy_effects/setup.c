/// @file
/// An effect's work: setting it up, its caster and target, cleaning up.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Scheduler callback: updates the 24 effect slots.
void BattleFx_UpdateAllEffectSlots(void) { FieldFx_UpdateSceneSlots(); }

/// Prepares the work of the Psynergy effect for ability (heap slot 56) and
/// loads its spark tiles. Called again while an effect resolves, it only
/// switches the ability.
///
/// Field_UsePsynergy and the item uses (object/items.c) call it before an
/// effect and BattleEffect_CleanupSceneObjects after it, which releases the
/// work, the spark tiles' resource entry and the scheduler callback started
/// here. Goma Cave Entrance calls it too, for Ivan's Whirlwind on the
/// stump's ivy.
/// The caster is the leader, with no target until BattleFx_SetupObjectPair
/// names one.
void BattleFx_LoadActionEffectResources(s32 abilityId, s32 mode) {
  struct FieldWork *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
  struct MapScrollWork *map = *(gWorkSlot + 8);
  struct BattleFxScene *work;
  s32 running;

  running = ev->resolvingAction;
  if (running == 0) {
    work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof *work);
    Dma_Fill32(running, work, sizeof *work);
  } else {
    work = *(gWorkSlot + HEAP_SLOT_FX);
  }
  work->action = abilityId;
  work->fieldKind = BattleActGet(abilityId)->type0c;
  running = ev->resolvingAction;
  if (running != 0)
    return;
  work->freeBlocks = 512 - ResourceTable_CountFreeBlocks();
  work->mode = mode;
  work->visible = 1;
  work->enabled = 1;
  work->active = 1;
  work->messageMode = 1;
  // Keep the map's shake and its decay, to restore after the effect.
  work->savedShakeX = map->shakeX;
  work->savedShakeY = map->shakeY;
  work->savedShakeDecay = map->shakeDecay;
  if (gGameState.map == (s32)&SceneId_MercuryLighthouseEntrance)
    work->lit = 1;
  if (gGameState.map == (s32)&SceneId_MercuryLighthouseMiddle)
    work->lit = 1;
  BattleFx_SetupObjectPair(gGameState.selectedActor, -1);
  // Every effect but Reveal's own pauses Reveal's countdown (running is 0
  // here) until BattleEffect_CleanupSceneObjects lets it run again.
  if (work->fieldKind != FIELD_KIND_REVEAL)
    ev->effectTimerRunning = running;
  work->tileSlot = Resource_FindFreeEntry();
  VramCache_Load((s16)work->tileSlot, 256, gBattleFx_SparkTiles);
  // 0xc80 is SCHEDULER_KEY_MAIN_LOOP: the slots update once a frame.
  Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}

/// Records the caster and the target of an effect. With no target, the effect
/// takes place 16 pixels in front of the caster (facing to the nearest
/// quarter turn). work->angle becomes that facing, the way the effects aim.
/// A target's update hook, script and first sprite's param are saved, for
/// the effects that take it over to restore.
void BattleFx_SetupObjectPair(s32 userId, s32 targetId) {
  struct ScriptMotionObject *user;
  struct ScriptMotionObject *target;
  s32 dir;
  struct BattleFxScene *work;

  work = gEffectWork;
  work->firstObjectId = userId;
  user = ObjectTable_Get((s16)userId);
  work->secondObjectId = targetId;
  work->mainObject = user;
  target = ObjectTable_Get((s16)targetId);
  dir = (user->facing + 0x2000) & 0xc000; // to the nearest quarter turn
  work->child = target;
  work->angle = dir;
  if (target != NULL) {
    // Keep the target's motion and colours, to restore them later.
    work->savedCallback = target->hook;
    work->savedScript = (const s32 *)target->script;
    {
      struct AnimationObject *anim =
          ((struct ObjectRuntime *)target)->animation;
      u8 pal = anim->entries[0]->param;

      work->savedPalette = pal;
    }
    work->x = target->x;
    work->z = target->z;
    work->y = target->y;
    return;
  }
  work->x = user->x;
  work->z = user->z;
  work->y = user->y;
  Vector_AddPolarOffset(0x100000, dir, &work->x);
}

/// Stops the caster's per-frame hook and restores its colours (sprite param
/// 0), then waits a frame.
void EffectRuntime_StopCurrentObject(void) {
  struct BattleFxScene *work = gEffectWork;
  struct ScriptMotionObject *user = work->mainObject;

  user->hook = NULL;
  Animation_ApplyChildValuesFar((struct DispatchObject *)user, 0);
  WaitFrames(1);
}

#define SETUP_OBJECT_SLOTS ((struct ScriptMotionObject *)gObjectSlots)

/// Ends a Psynergy effect: frees the effect slots still in use and, unless
/// another effect is being resolved, waits (at most 30 frames) for the
/// swelling arcs to end, stops the slot callback, releases the effect's
/// tiles, restores the map's shake, plays the queued sound and releases the
/// effect's work.
void BattleEffect_CleanupSceneObjects(void) {
#ifndef UBFIX
  struct BattleFxScene **sceneCell;
#endif
  struct BattleFxScene *scene;
  struct FieldWork *runtime;
  struct MapScrollWork *map;
  struct EffectSlot *sceneObject;
  s32 remaining;

#ifdef UBFIX
  scene = gEffectWork;
  runtime = (struct FieldWork *)gEventWork;
  map = gMapWork[0];
#else
  // Indexing beyond the scalar global is undefined in C. Named slots add
  // 8 bytes to this function in the forced native EN build (0xf8 -> 0x100).
  sceneCell = (struct BattleFxScene **)&gEffectWork;
  scene = *sceneCell;
  runtime = *(struct FieldWork **)((u8 *)sceneCell - 116);
  map = *(struct MapScrollWork **)((u8 *)sceneCell - 192);
#endif
  for (remaining = 0; remaining < 24; remaining++) {
    sceneObject = &scene->slots[remaining];
    if (sceneObject->active != 0)
      BattleFx_ClearOwnedSlot(sceneObject);
  }

  // While Field_UsePsynergy resolves an effect (resolvingAction), a cleanup
  // from inside it frees only the slots; the rest waits for its own.
  if (runtime->resolvingAction == 0) {
    s32 waited = 0;
    void (*firstActiveUpdate)(union FieldObject *) =
        FieldFx_UpdateScaledArcObjectB;
    void (*secondActiveUpdate)(union FieldObject *) =
        FieldFx_UpdateScaledArcObjectA;
    s32 active;

    do {
      struct ScriptMotionObject *slot = SETUP_OBJECT_SLOTS;

      active = 0;
      remaining = 0;
      while (remaining <= 63) {
        void (*update)(union FieldObject *) = slot->hook;

        if (update == firstActiveUpdate || update == secondActiveUpdate) {
          active = 1;
          break;
        }
        remaining++;
        slot++;
      }

      if (active != 0) {
        waited++;
        WaitFrames(1);
      }
    } while (active != 0 && waited <= 29);

    runtime->castStarted = 0;
    Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
    Resource_ResetEntry((s16)scene->tileSlot);
    map->shakeX = scene->savedShakeX;
    map->shakeY = scene->savedShakeY;
    map->shakeDecay = scene->savedShakeDecay;
    if (scene->fieldKind != FIELD_KIND_REVEAL)
      runtime->effectTimerRunning = 1;
    BattleFx_PlayQueuedSound();
    Runtime_ReleaseHeapBlock(HEAP_SLOT_FX);
  }
}

/// Starts casting: the caster takes its casting pose (animation 20) and
/// stops. Unless the effect is silent the caster flashes with a sound; unless
/// it is quiet, the ability's name is shown while every other object stands
/// still. With flag 0x140 the caster's colours keep cycling and it
/// takes pose 21; otherwise the caster's hook stops. Sets the field work's
/// castStarted, which BattleEffect_CleanupSceneObjects clears. Called by
/// Field_UsePsynergy and Goma Cave Entrance once the effect's caster and
/// target are set up.
void EventObject_Initialize(void) {
  s32 ability;
  s32 announcedAbility;
  struct FieldActor *caster;
  struct BattleFxScene *scene;
  void *renderState;

  scene = *(void **)Ram_EffectWork;
  renderState = *(void **)Ram_EventWork;
  caster = scene->mainObject;
  ability = (s16)scene->action;
  announcedAbility = ability;
  ObjectDispatch_SelectAnimationFar(caster, 0x14);
  caster->targetX = caster->x.fixed;
  caster->targetY = caster->y.fixed;
  caster->targetZ = caster->z.fixed;
  SET_VELOCITY(caster, 0, 0, 0);
  if (scene->visible != 0) {
    AudioCommand_PlayFar(SE_212);
    caster->update = BattleFx_FlickerObjectAndTick;
  }
  if (scene->active != 0) {
    ObjectGroup_SetActionForOthers((struct ObjectRuntime *)caster, 1, 0);
    UiWork_PushValueSlotFar(announcedAbility, 4);
    // The mode branches currently show the same message. Merging them shrinks
    // the native English function by 32 bytes, so keep this existing form.
    if ((s8)scene->mode != 0) {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             scene->messageMode);
    } else {
      UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce,
                                             scene->messageMode);
    }
    ObjectGroup_SetActionForOthers((struct ObjectRuntime *)caster, 0, 0x10);
  }
  if (GameFlag_TestFar(0x140) != 0) {
    if (scene->visible != 0) {
      caster->update = FieldFx_CycleObjectValueByCounter;
    }
    ObjectDispatch_SelectAnimationFar(caster, 0x15);
  } else {
    EffectRuntime_StopCurrentObject();
  }
  ((struct FieldWork *)renderState)->castStarted = 1;
}

/// Saves the palettes for the effect and fades them toward one of the eight
/// effect colours (gBattleFx_TargetColors), picked by the frame counter or
/// forced by flags 0x148 to 0x14f, over 8 frames. Most field Psynergy
/// effects open with it, and BattleFx_PrepareBufferInterpolation fades back.
void BattleEffect_InitializeSharedScene(void) {
  u8 *scene;
  struct BattleEffectBuffers *work;
  s32 no;

  // Save the colours the buffer moves toward into the field work, 224
  // colours of three channels (0x150 words) a half: the OBJ half at 0x776
  // and, unless Reveal is active, the BG half at 0x236. While Reveal lasts
  // the BG target is its grey (reveal.c), and the field's own colours saved
  // before it must stay. Then show the targets at once.
  scene = *(gWorkSlot + HEAP_SLOT_EVENT);
  work = *(gWorkSlot + HEAP_SLOT_PALETTE_BUFFER);
  Dma_Set(&work->target[0x2a0], scene + 0x776,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x150, REG_DMA3);
  if (((struct FieldWork *)scene)->revealActive == 0)
    Dma_Set(work->target, scene + 0x236, (DMA_ENABLE | DMA_32BIT) << 16 | 0x150,
            REG_DMA3);
  Dma_Set(work->target, work->current,
          (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof work->current / 4),
          REG_DMA3);

  // Of the forcing flags, the highest one set wins.
  no = gFrameCount & 7;
  if (GameFlag_TestFar(0x148))
    no = 0;
  if (GameFlag_TestFar(0x149))
    no = 1;
  if (GameFlag_TestFar(0x14a))
    no = 2;
  if (GameFlag_TestFar(0x14b))
    no = 3;
  if (GameFlag_TestFar(0x14c))
    no = 4;
  if (GameFlag_TestFar(0x14d))
    no = 5;
  if (GameFlag_TestFar(0x14e))
    no = 6;
  if (GameFlag_TestFar(0x14f))
    no = 7;
  BattleFx_ApplyColorToTargetBuffer(gBattleFx_TargetColors[no], 1);
  BattleFx_StartBufferInterpolation(8);
}

/// Fades the palettes back from an effect's colours to the saved ones over 8
/// frames; while Reveal is active the BG half fades to Reveal's grey
/// instead (PALETTE_TRANSFORM_GREY, 0x10001).
void BattleFx_PrepareBufferInterpolation(void) {
  s32 savedPalettes;
  u8 *field;

  field = (u8 *)gEventWork;
  savedPalettes = (s32)(field + 0x236);
  BattleFx_ApplyColorToTargetBuffer(savedPalettes, 2);
  if (((struct FieldWork *)field)->revealActive != 0) {
    BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  } else {
    BattleFx_ApplyColorToTargetBuffer(savedPalettes, 1);
  }
  BattleFx_StartBufferInterpolation(8);
}

/// Converts a world position (16.16 pixels) to the screen: x and z receive
/// the screen's x and y and y becomes 0. The world map uses its perspective
/// projection.
void Camera_WorldToScreen(struct EffectVector *position) {
#ifdef UBFIX
  struct FieldWork *event = (struct FieldWork *)gEventWork;

  if (event->mode == MODE_WORLD_MAP) {
#else
  struct FieldWork **eventCell = (struct FieldWork **)&gEventWork;

  if ((*eventCell)->mode == MODE_WORLD_MAP) {
#endif
    s32 result[3];

    Render_ProjectPoint(position, result);
    position->x = result[0] << 16;
    position->z = result[1] << 16;
    position->y = 0;
  } else {
#ifdef UBFIX
    struct MapScrollWork *state = gMapWork[0];
#else
    // The named map slot avoids stepping outside gEventWork. In forced native
    // EN it replaces the address copy/subtract with a separate literal load
    // and changes register allocation; the extent remains 0x68 bytes.
    struct MapScrollWork *state =
        *(struct MapScrollWork **)((u8 *)eventCell - 76);
#endif
    s32 x = state->viewX & 0xffff0000;
    s32 z = state->viewY & 0xffff0000;

    // Relative to the view's whole-pixel corner; height lifts a point up
    // the screen.
    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

// From here on positions are passed in each function's own structures.

#undef SETUP_OBJECT_SLOTS
