/// @file
/// A field Psynergy's state: setting it up, its caster and target, cleaning up.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The frames FieldPsynergy_End waits at most for the swelling arcs to end.
#define END_ARC_WAIT_FRAMES 30

/// The caster's casting animations on the party's sprite sheets:
/// ANIM_CAST_PSYNERGY's first two pictures, held on the second, and from the
/// second to its last, held on the last.
#define ANIM_CAST_PSYNERGY_START 20
#define ANIM_CAST_PSYNERGY_END 21

/// A PaletteFade half's colours, the BG's and then the sprites', three
/// channels each.
#define FADE_HALF_COLORS 224
#define FADE_HALF_CHANNELS (FADE_HALF_COLORS * 3)

/// Task: updates the field Psynergy sprites.
void FieldEffect_UpdateAllEffectSlots(void) { FieldEffect_UpdateEffectSlots(); }

/// Prepares the state of the field Psynergy for abilityId
/// (HEAP_SLOT_FIELD_EFFECT) and loads its spark tiles; mode is where it runs,
/// an enum ActorSystemMode.
void FieldPsynergy_LoadResources(s32 abilityId, s32 mode) {
  struct FieldState *fieldState = gFieldState;
  struct MapGridScrollState *grid = gMapGridState[0];
  struct FieldEffectState *effectState;
  s32 running;

  running = fieldState->psynergyActing;
  if (running == 0) {
    effectState = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof *effectState);
    Dma_Fill32(running, effectState, sizeof *effectState);
  } else {
    effectState = gFieldEffectState;
  }
  effectState->ability = abilityId;
  effectState->fieldPsynergy = Ability_Get(abilityId)->fieldPsynergy;
  running = fieldState->psynergyActing;
  if (running != 0)
    return;
  effectState->freeBlocks = VRAM_BLOCK_COUNT - VramCache_CountFreeBlocks();
  effectState->mode = mode;
  effectState->flashCaster = 1;
  effectState->aimAhead = 1;
  effectState->announceName = 1;
  effectState->messageMode = 1;
  // Keep the map's shake and its decay, to restore after the Psynergy.
  effectState->savedShakeX = grid->shakeX;
  effectState->savedShakeY = grid->shakeY;
  effectState->savedShakeDecay = grid->shakeDecay;
  if (gMainRoutineState.mapId == SCENE_MERCURY_LIGHTHOUSE_ENTRANCE)
    effectState->lit = 1;
  if (gMainRoutineState.mapId == SCENE_MERCURY_LIGHTHOUSE_MIDDLE)
    effectState->lit = 1;
  FieldPsynergy_SetupActorPair(gMainRoutineState.leaderActorId, -1);
  // Every field Psynergy but Reveal itself pauses Reveal's countdown (running
  // is 0 here) until FieldPsynergy_End lets it run again.
  if (effectState->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
    fieldState->effectTimerRunning = running;
  effectState->tileSlot = VramCache_FindFreeEntry();
  VramCache_Load(effectState->tileSlot, 256, gFieldEffect_SparkTiles);
  Task_Add(FieldEffect_UpdateAllEffectSlots, TASK_PASS_FRAME);
}

/// Records the caster and the target of a field Psynergy, and where it takes
/// place: on the target, or 16 pixels in front of the caster with none.
void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId) {
  struct Actor *user;
  struct Actor *target;
  s32 facing;
  struct FieldEffectState *effectState;

  effectState = gFieldEffectState;
  effectState->casterId = userId;
  user = Actor_Lookup((s16)userId);
  effectState->targetId = targetId;
  effectState->caster = user;
  target = Actor_Lookup((s16)targetId);
  facing = FACING_NEAREST_CARDINAL(user->facing);
  effectState->target = target;
  effectState->angle = facing;
  if (target != NULL) {
    // Keep the target's motion and colours, to restore them later.
    effectState->savedCallback = target->update;
    effectState->savedScript = (const s32 *)target->script;
    {
      struct Sprite *sprite = target->sprite;
      u8 colorMode = sprite->entries[0]->colorMode;

      effectState->savedPalette = colorMode;
    }
    effectState->x = target->x.fixed;
    effectState->z = target->z.fixed;
    effectState->y = target->y.fixed;
    return;
  }
  effectState->x = user->x.fixed;
  effectState->z = user->z.fixed;
  effectState->y = user->y.fixed;
  Vector_AddPolarOffset(toFIXED(16), facing, &effectState->x);
}

/// Stops the caster's update routine and restores its colours, then waits a
/// frame.
void FieldPsynergy_StopCasterUpdate(void) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *user = effectState->caster;

  user->update = NULL;
  Actor_SetSpriteColorModeFar(user, SPRITE_COLOR_NORMAL);
  wait(1);
}

/// Ends a field Psynergy: frees the field effect sprites still in use and,
/// unless another is being resolved, waits (at most END_ARC_WAIT_FRAMES) for
/// the swelling arcs to end, stops the slot task, releases its tiles,
/// restores the map's shake, plays the queued sound and releases its
/// state.
void FieldPsynergy_End(void) {
  struct FieldEffectState *effectState;
  struct FieldState *fieldState;
  struct MapGridScrollState *grid;
  struct FieldParticle *effectSlot;
  s32 i;

  effectState = gFieldEffectState;
  fieldState = gFieldState;
  grid = gMapGridState[0];
  for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
    effectSlot = &effectState->particles[i];
    if (effectSlot->active != 0)
      FieldEffect_ClearOwnedSlot(effectSlot);
  }

  // While Field_UsePsynergy resolves a field Psynergy (psynergyActing), a
  // cleanup from inside it frees only the slots; the rest waits for its own.
  if (fieldState->psynergyActing == 0) {
    s32 waited = 0;
    void (*firstActiveUpdate)(struct Actor *) =
        FieldEffect_UpdateScaledArcActorB;
    void (*secondActiveUpdate)(struct Actor *) =
        FieldEffect_UpdateScaledArcActorA;
    s32 active;

    do {
      struct Actor *slot = gActors;

      active = 0;
      i = 0;
      while (i < ACTOR_COUNT) {
        void (*update)(struct Actor *) = slot->update;

        if (update == firstActiveUpdate || update == secondActiveUpdate) {
          active = 1;
          break;
        }
        i++;
        slot++;
      }

      if (active != 0) {
        waited++;
        wait(1);
      }
    } while (active != 0 && waited < END_ARC_WAIT_FRAMES);

    fieldState->castStarted = 0;
    Task_Remove(FieldEffect_UpdateAllEffectSlots);
    VramCache_FreeEntry(effectState->tileSlot);
    grid->shakeX = effectState->savedShakeX;
    grid->shakeY = effectState->savedShakeY;
    grid->shakeDecay = effectState->savedShakeDecay;
    if (effectState->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
      fieldState->effectTimerRunning = 1;
    Scene_PlayQueuedMusic();
    HeapSlot_Release(HEAP_SLOT_FIELD_EFFECT);
  }
}

/// Starts casting: the caster raises its hand (ANIM_CAST_PSYNERGY_START) and
/// stops; with FLAG_PLAY_PSYNERGY_CAST it goes on to ANIM_CAST_PSYNERGY_END.
void FieldPsynergy_StartCast(void) {
  s32 ability;
  struct Actor *caster;
  struct FieldEffectState *effectState;
  void *fieldState;

  effectState = *(void **)Ram_FieldEffectState;
  fieldState = *(void **)Ram_FieldState;
  caster = effectState->caster;
  ability = (s16)effectState->ability;
  Actor_SelectAnimationFar(caster, ANIM_CAST_PSYNERGY_START);
  caster->targetX = caster->x.fixed;
  caster->targetY = caster->y.fixed;
  caster->targetZ = caster->z.fixed;
  SET_VELOCITY(caster, 0, 0, 0);
  if (effectState->flashCaster != 0) {
    Sound_PlayFar(SE_POWER_UP);
    caster->update = FieldEffect_FlickerActorAndSpawnArcs;
  }
  if (effectState->announceName != 0) {
    Actor_SetOthersPaused(caster, 1, 0);
    MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
    // On the World Map and off it alike, the same announcement.
    if ((s8)effectState->mode != 0) {
      FieldMessage_ShowFar(MsgAbilityAnnounce, effectState->messageMode);
    } else {
      FieldMessage_ShowFar(MsgAbilityAnnounce, effectState->messageMode);
    }
    Actor_SetOthersPaused(caster, 0, ANIM_STEP_NORMAL);
  }
  if (GameFlag_GetFar(FLAG_PLAY_PSYNERGY_CAST) != 0) {
    if (effectState->flashCaster != 0) {
      caster->update = FieldEffect_CycleActorAndSpawnArcs;
    }
    Actor_SelectAnimationFar(caster, ANIM_CAST_PSYNERGY_END);
  } else {
    FieldPsynergy_StopCasterUpdate();
  }
  ((struct FieldState *)fieldState)->castStarted = 1;
}

/// Saves the palettes for the Psynergy and fades them toward one of the eight
/// cast colours (gFieldPsynergy_CastColors), picked by the frame counter or
/// forced by FLAG_PSYNERGY_TINT(0) to (7), over 8 frames.
void FieldPsynergy_FadeToCastColor(void) {
  struct FieldState *fieldState;
  struct PaletteFade *fade;
  s32 tint;

  // Save the colours the buffer moves toward into the fieldState fade: the
  // sprite half and, unless Reveal is active, the BG half.
  fieldState = gFieldState;
  fade = gPaletteFadeState;
  Dma_Set(&fade->target[FADE_HALF_CHANNELS],
          &fieldState->savedFadeTarget[FADE_HALF_CHANNELS],
          DMA_COPY_WORDS(FADE_HALF_CHANNELS / 2), REG_DMA3);
  if (fieldState->revealActive == 0)
    Dma_Set(fade->target, fieldState->savedFadeTarget,
            DMA_COPY_WORDS(FADE_HALF_CHANNELS / 2), REG_DMA3);
  Dma_Set(fade->target, fade->current, DMA_COPY_WORDS(sizeof fade->current / 4),
          REG_DMA3);

  // Of the forcing flags, the highest one set wins.
  tint = gFrameCount & 7;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(0)))
    tint = 0;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(1)))
    tint = 1;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(2)))
    tint = 2;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(3)))
    tint = 3;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(4)))
    tint = 4;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(5)))
    tint = 5;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(6)))
    tint = 6;
  if (GameFlag_GetFar(FLAG_PSYNERGY_TINT(7)))
    tint = 7;
  PaletteFade_SetTarget(gFieldPsynergy_CastColors[tint], PALETTES_BG);
  PaletteFade_StartInterpolation(8);
}

/// Fades the palettes back from a cast's colours to the saved ones over 8
/// frames; while Reveal is active the BG half fades to Reveal's grey
/// instead (PALETTE_TINT_GRAYSCALE).
void FieldPsynergy_FadeFromCastColor(void) {
  struct FieldState *state = gFieldState;
  s32 savedPalettes = (s32)state->savedFadeTarget;

  PaletteFade_SetTarget(savedPalettes, PALETTES_OBJECTS);
  if (state->revealActive != 0) {
    PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  } else {
    PaletteFade_SetTarget(savedPalettes, PALETTES_BG);
  }
  PaletteFade_StartInterpolation(8);
}

/// Converts a world position (16.16 pixels) to the screen: x and z receive the
/// screen's x and y and y becomes 0.
void Camera_WorldToScreen(struct Vector3 *position) {
#ifdef UBFIX
  struct FieldState *fieldState = gFieldState;

  if (fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
  struct FieldState **fieldStateSlot = (struct FieldState **)&gFieldState;

  if ((*fieldStateSlot)->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
    s32 result[3];

    Matrix_ProjectPoint(position, result);
    position->x = toFIXED(result[0]);
    position->z = toFIXED(result[1]);
    position->y = 0;
  } else {
#ifdef UBFIX
    struct MapGridScrollState *state = gMapGridState[0];
#else
    // UB: gMapGridState's heap slot, reached by stepping back from
    // gFieldState's.
    struct MapGridScrollState *state =
        *(struct MapGridScrollState **)((u8 *)fieldStateSlot -
                                        (HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID) *
                                            sizeof(void *));
#endif
    s32 x = state->view[0] & FIELD_PIXEL_MASK;
    s32 z = state->view[1] & FIELD_PIXEL_MASK;

    // Relative to the view's whole-pixel corner; height lifts a point up
    // the screen.
    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

#undef END_ARC_WAIT_FRAMES
#undef ANIM_CAST_PSYNERGY_START
#undef ANIM_CAST_PSYNERGY_END
#undef FADE_HALF_COLORS
#undef FADE_HALF_CHANNELS
