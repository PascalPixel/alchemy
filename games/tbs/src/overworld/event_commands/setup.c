/// @file
/// A field Psynergy's work: setting it up, its caster and target, cleaning up.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Task: updates the 24 field effect sprites.
void FieldEffect_UpdateAllEffectSlots(void) { FieldEffect_UpdateEffectSlots(); }

/// Prepares the work of the field Psynergy for ability
/// (HEAP_SLOT_FIELD_EFFECT) and loads its spark tiles. Called again while one
/// resolves, it only switches the ability.
///
/// Field_UsePsynergy and the item uses (overworld/items.c) call it before a
/// field Psynergy and FieldPsynergy_End after it, which releases the
/// work, the spark tiles' resource entry and the task started
/// here. Goma Cave Entrance calls it too, for Ivan's Whirlwind on the
/// stump's ivy.
/// The caster is the leader, with no target until FieldPsynergy_SetupActorPair
/// names one.
void FieldPsynergy_LoadResources(s32 abilityId, s32 mode) {
  struct FieldWork *ev = *(gHeapSlots + HEAP_SLOT_FIELD);
  struct MapGridScrollWork *grid = *(gHeapSlots + 8);
  struct FieldEffectWork *effectWork;
  s32 running;

  running = ev->psynergyActing;
  if (running == 0) {
    effectWork = AllocHeapSlot(HEAP_SLOT_FIELD_EFFECT, sizeof *effectWork);
    Dma_Fill32(running, effectWork, sizeof *effectWork);
  } else {
    effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
  }
  effectWork->action = abilityId;
  effectWork->fieldPsynergy = Ability_Get(abilityId)->fieldPsynergy;
  running = ev->psynergyActing;
  if (running != 0)
    return;
  effectWork->freeBlocks = 512 - VramCache_CountFreeBlocks();
  effectWork->mode = mode;
  effectWork->visible = 1;
  effectWork->enabled = 1;
  effectWork->active = 1;
  effectWork->messageMode = 1;
  // Keep the map's shake and its decay, to restore after the Psynergy.
  effectWork->savedShakeX = grid->shakeX;
  effectWork->savedShakeY = grid->shakeY;
  effectWork->savedShakeDecay = grid->shakeDecay;
  if (gCommon.mapId == (s32)&SceneId_MercuryLighthouseEntrance)
    effectWork->lit = 1;
  if (gCommon.mapId == (s32)&SceneId_MercuryLighthouseMiddle)
    effectWork->lit = 1;
  FieldPsynergy_SetupActorPair(gCommon.leaderActorId, -1);
  // Every field Psynergy but Reveal itself pauses Reveal's countdown (running
  // is 0 here) until FieldPsynergy_End lets it run again.
  if (effectWork->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    ev->effectTimerRunning = running;
  effectWork->tileSlot = VramCache_FindFreeEntry();
  VramCache_Load((s16)effectWork->tileSlot, 256, gFieldEffect_SparkTiles);
  // 0xc80 is TASK_PASS_FRAME: the slots update once a frame.
  Task_Add(FieldEffect_UpdateAllEffectSlots, TASK_PASS_FRAME);
}

/// Records the caster and the target of a field Psynergy. With no target, it
/// takes place 16 pixels in front of the caster (facing to the nearest
/// quarter turn). work->angle becomes that facing, the way field Psynergy aims.
/// A target's update routine, script and first sprite's param are saved, for
/// the Psynergy that takes it over to restore.
void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId) {
  struct Actor *user;
  struct Actor *target;
  s32 dir;
  struct FieldEffectWork *effectWork;

  effectWork = gFieldEffectWork;
  effectWork->firstActorId = userId;
  user = Actor_Lookup((s16)userId);
  effectWork->secondActorId = targetId;
  effectWork->mainActor = user;
  target = Actor_Lookup((s16)targetId);
  dir = (user->facing + 0x2000) & 0xc000; // to the nearest quarter turn
  effectWork->child = target;
  effectWork->angle = dir;
  if (target != NULL) {
    // Keep the target's motion and colours, to restore them later.
    effectWork->savedCallback = target->update;
    effectWork->savedScript = (const s32 *)target->script;
    {
      struct Sprite *animation = target->sprite;
      u8 pal = animation->entries[0]->colorMode;

      effectWork->savedPalette = pal;
    }
    effectWork->x = target->x.fixed;
    effectWork->z = target->z.fixed;
    effectWork->y = target->y.fixed;
    return;
  }
  effectWork->x = user->x.fixed;
  effectWork->z = user->z.fixed;
  effectWork->y = user->y.fixed;
  Vector_AddPolarOffset(0x100000, dir, &effectWork->x);
}

/// Stops the caster's update routine and restores its colours (sprite param
/// 0), then waits a frame.
void FieldPsynergy_StopCasterUpdate(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *user = effectWork->mainActor;

  user->update = NULL;
  Anim_ApplyChildValuesFar(user, SPRITE_COLOR_NORMAL);
  WaitFrames(1);
}

#define SETUP_OBJECT_SLOTS (gActors)

/// Ends a field Psynergy: frees the field effect sprites still in use and,
/// unless another is being resolved, waits (at most 30 frames) for the
/// swelling arcs to end, stops the slot task, releases its tiles,
/// restores the map's shake, plays the queued sound and releases its
/// work.
void FieldPsynergy_End(void) {
#ifndef UBFIX
  struct FieldEffectWork **effectWorkCell;
#endif
  struct FieldEffectWork *effectWork;
  struct FieldWork *runtime;
  struct MapGridScrollWork *grid;
  struct FieldParticle *effectSlot;
  s32 remaining;

#ifdef UBFIX
  effectWork = gFieldEffectWork;
  runtime = gFieldWork;
  grid = gMapGridWork[0];
#else
  // Indexing beyond the scalar global is undefined in C. Named slots add
  // 8 bytes to this function in the forced native EN build (0xf8 -> 0x100).
  effectWorkCell = (struct FieldEffectWork **)&gFieldEffectWork;
  effectWork = *effectWorkCell;
  runtime = *(struct FieldWork **)((u8 *)effectWorkCell - 116);
  grid = *(struct MapGridScrollWork **)((u8 *)effectWorkCell - 192);
#endif
  for (remaining = 0; remaining < 24; remaining++) {
    effectSlot = &effectWork->particles[remaining];
    if (effectSlot->active != 0)
      FieldEffect_ClearOwnedSlot(effectSlot);
  }

  // While Field_UsePsynergy resolves a field Psynergy (psynergyActing), a
  // cleanup from inside it frees only the slots; the rest waits for its own.
  if (runtime->psynergyActing == 0) {
    s32 waited = 0;
    void (*firstActiveUpdate)(struct Actor *) =
        FieldEffect_UpdateScaledArcActorB;
    void (*secondActiveUpdate)(struct Actor *) =
        FieldEffect_UpdateScaledArcActorA;
    s32 active;

    do {
      struct Actor *slot = SETUP_OBJECT_SLOTS;

      active = 0;
      remaining = 0;
      while (remaining <= 63) {
        void (*update)(struct Actor *) = slot->update;

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
    Task_Remove(FieldEffect_UpdateAllEffectSlots);
    VramCache_FreeEntry((s16)effectWork->tileSlot);
    grid->shakeX = effectWork->savedShakeX;
    grid->shakeY = effectWork->savedShakeY;
    grid->shakeDecay = effectWork->savedShakeDecay;
    if (effectWork->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
      runtime->effectTimerRunning = 1;
    Scene_PlayQueuedMusic();
    FreeHeapSlot(HEAP_SLOT_FIELD_EFFECT);
  }
}

/// Starts casting: the caster takes its casting pose (animation 20) and
/// stops. Unless the Psynergy is silent the caster flashes with a sound; unless
/// it is quiet, the ability's name is shown while every other actor stands
/// still. With flag 0x140 the caster's colours keep cycling and it
/// takes pose 21; otherwise the caster's update routine stops. Sets the field
/// work's castStarted, which FieldPsynergy_End clears. Called by
/// Field_UsePsynergy and Goma Cave Entrance once the Psynergy's caster and
/// target are set up.
void FieldPsynergy_StartCast(void) {
  s32 ability;
  s32 announcedAbility;
  struct Actor *caster;
  struct FieldEffectWork *effectWork;
  void *renderState;

  effectWork = *(void **)Ram_FieldEffectWork;
  renderState = *(void **)Ram_FieldWork;
  caster = effectWork->mainActor;
  ability = (s16)effectWork->action;
  announcedAbility = ability;
  Actor_SelectAnimationFar(caster, 0x14);
  caster->targetX = caster->x.fixed;
  caster->targetY = caster->y.fixed;
  caster->targetZ = caster->z.fixed;
  SET_VELOCITY(caster, 0, 0, 0);
  if (effectWork->visible != 0) {
    Sound_PlaySongFar(SE_POWER_UP);
    caster->update = FieldEffect_FlickerActorAndSpawnArcs;
  }
  if (effectWork->active != 0) {
    Actor_SetOthersPaused(caster, 1, 0);
    MessageArgument_PushFar(announcedAbility, MESSAGE_ARGUMENT_ABILITY);
    // The mode branches currently show the same message. Merging them shrinks
    // the native English function by 32 bytes, so keep this existing form.
    if ((s8)effectWork->mode != 0) {
      FieldMessage_ShowFar(MsgAbilityAnnounce, effectWork->messageMode);
    } else {
      FieldMessage_ShowFar(MsgAbilityAnnounce, effectWork->messageMode);
    }
    Actor_SetOthersPaused(caster, 0, 0x10);
  }
  if (GameFlag_GetFar(FLAG_PLAY_PSYNERGY_CAST) != 0) {
    if (effectWork->visible != 0) {
      caster->update = FieldEffect_CycleActorValueByCounter;
    }
    Actor_SelectAnimationFar(caster, 0x15);
  } else {
    FieldPsynergy_StopCasterUpdate();
  }
  ((struct FieldWork *)renderState)->castStarted = 1;
}

/// Saves the palettes for the Psynergy and fades them toward one of the eight
/// cast colours (gFieldEffect_TargetColors), picked by the frame counter or
/// forced by flags 0x148 to 0x14f, over 8 frames. Most field Psynergy
/// opens with it, and FieldPsynergy_FadeFromCastColor fades back.
void FieldPsynergy_FadeToCastColor(void) {
  u8 *field;
  struct PaletteFade *work;
  s32 no;

  // Save the colours the buffer moves toward into the field work, 224
  // colours of three channels (0x150 words) a half: the OBJ half at 0x776
  // and, unless Reveal is active, the BG half at 0x236. While Reveal lasts
  // the BG target is its grey (reveal.c), and the field's own colours saved
  // before it must stay. Then show the targets at once.
  // Both blocks are read through gHeapSlots, the spelling this site matched
  // with; gFieldWork and gPaletteFadeWork are not measured here.
  field = *(gHeapSlots + HEAP_SLOT_FIELD);
  work = *(gHeapSlots + HEAP_SLOT_PALETTE_FADE);
  Dma_Set(&work->target[0x2a0], field + 0x776,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x150, REG_DMA3);
  if (((struct FieldWork *)field)->revealActive == 0)
    Dma_Set(work->target, field + 0x236, (DMA_ENABLE | DMA_32BIT) << 16 | 0x150,
            REG_DMA3);
  Dma_Set(work->target, work->current,
          (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof work->current / 4),
          REG_DMA3);

  // Of the forcing flags, the highest one set wins.
  no = gFrameCount & 7;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(0)))
    no = 0;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(1)))
    no = 1;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(2)))
    no = 2;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(3)))
    no = 3;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(4)))
    no = 4;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(5)))
    no = 5;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(6)))
    no = 6;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(7)))
    no = 7;
  PaletteFade_SetTarget(gFieldEffect_TargetColors[no], 1);
  PaletteFade_StartInterpolation(8);
}

/// Fades the palettes back from a cast's colours to the saved ones over 8
/// frames; while Reveal is active the BG half fades to Reveal's grey
/// instead (PALETTE_TINT_GRAYSCALE, 0x10001).
void FieldPsynergy_FadeFromCastColor(void) {
  s32 savedPalettes;
  u8 *field;

  field = (u8 *)gFieldWork;
  savedPalettes = (s32)(field + 0x236);
  PaletteFade_SetTarget(savedPalettes, 2);
  if (((struct FieldWork *)field)->revealActive != 0) {
    PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, 1);
  } else {
    PaletteFade_SetTarget(savedPalettes, 1);
  }
  PaletteFade_StartInterpolation(8);
}

/// Converts a world position (16.16 pixels) to the screen: x and z receive
/// the screen's x and y and y becomes 0. The World Map uses its perspective
/// projection.
void Camera_WorldToScreen(struct Vector3 *position) {
#ifdef UBFIX
  struct FieldWork *event = gFieldWork;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
  struct FieldWork **eventCell = (struct FieldWork **)&gFieldWork;

  if ((*eventCell)->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
    s32 result[3];

    Transform_ProjectPoint(position, result);
    position->x = result[0] << 16;
    position->z = result[1] << 16;
    position->y = 0;
  } else {
#ifdef UBFIX
    struct MapGridScrollWork *state = gMapGridWork[0];
#else
    // The named map slot avoids stepping outside gFieldWork. In forced native
    // EN it replaces the address copy/subtract with a separate literal load
    // and changes register allocation; the extent remains 0x68 bytes.
    struct MapGridScrollWork *state =
        *(struct MapGridScrollWork **)((u8 *)eventCell - 76);
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
