/// @file
/// Reveal: the world turns grey but for a rippling window round the leader,
/// which ends when the leader walks out of it or its time runs out.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Draws every live actor whose sprite shows a rock (SPRITE_ROCK) in the colour
/// mode value: Reveal greys the rocks with the backgrounds as it starts
/// (SPRITE_COLOR_GREY) and gives them back their colours as it ends
/// (SPRITE_COLOR_NORMAL).
void Actor_SetRevealSpriteColor(s32 value) {
  struct Actor *actor = gActors;
  s32 remainingActors = ACTOR_COUNT - 1;

  do {
    if (actor->script != NULL) {
      u8 kind = actor->spriteStorageKind;

      if (kind == ACTOR_SPRITE_ONE) {
        struct Sprite *animation = actor->sprite;
        struct SpriteAnimation *entry = animation->entries[0];

        if (entry->animationId == SPRITE_ROCK) {
          entry->colorMode = value;
          animation->dirty = kind;
        }
      }
    }
    remainingActors--;
    actor++;
  } while (remainingActors >= 0);
}

/// How far the leader may walk from where Reveal was cast, in screen pixels up
/// and down; sideways x counts at about 0.82 of itself, so the edge is an
/// ellipse about 73 pixels to either side.
#define REVEAL_RADIUS 60

/// Task of the timed Psynergy (FIELD_PSYNERGY_REVEAL): asks for it to end once
/// the leader is REVEAL_RADIUS from its centre or its time has run out.
void FieldPsynergy_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct Actor *actor;
  FIXED distanceX;
  s32 distanceZ;

  work = gFieldWork;
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  if (work->effectTimerRunning != 0) {
    if (work->revealFrames != 0)
      work->revealFrames--;
  }

  // In screen pixels: y is z less the height. x is scaled by 0.81649, about
  // the square root of 2/3.
  distanceX = Iwram_MulFX(work->revealCenterX - actor->x.fixed / FIXED_ONE,
                          toFIXED(0.81649));
  distanceZ =
      work->revealCenterY - (actor->z.fixed - actor->y.fixed) / FIXED_ONE;
  if (distanceX * distanceX + distanceZ * distanceZ >=
          REVEAL_RADIUS * REVEAL_RADIUS ||
      work->revealFrames == 0) {
    // Raise Reveal as a Psynergy request that pays no PP, which runs its
    // ending events.
    u16 *slot = (u16 *)&work->psynergy;
    u32 request = ACT_PACKED | ABILITY_REVEAL;

    *slot = request;
  }
}

/// Reveal: the backgrounds turn grey and show only within a rippling ellipse
/// round the leader; runs the scene's Reveal event if there is one, puts
/// markers round the leader and watches how far the leader walks.
void FieldPsynergy_Reveal(void) {
  struct FieldWork *work;
  struct ScreenTransitionState *state;
  struct Actor *target;
  s32 eventTarget;
  void *event;
  s32 frame;

  work = gFieldWork;
  Actor_SetRevealSpriteColor(SPRITE_COLOR_GREY);
  ScreenTransition_StartHeldWindow(WINDOW_TRANSITION_RIPPLE);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_SCREEN_TRANSITION);
  target = Actor_Lookup(gMainRoutineWork.leaderActorId);
  state->centreX = target->x.fixed;
  state->centreY = target->z.fixed - target->y.fixed;
  // The BG palettes turn grey in one frame.
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  PaletteFade_StartInterpolation(1);
  // Not in the English release.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
  work->revealActive = 0;
#endif
  wait(1);
  event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                    FIELD_PSYNERGY_REVEAL, &eventTarget);
  if (event != NULL)
    Field_RunPsynergyEvent(event, gMainRoutineWork.leaderActorId, eventTarget);
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);

  // Record the centre (in pixels) and the time in the field work, and put
  // markers nearby.
  work->revealActive = 1;
  work->revealCenterX = state->centreX / FIXED_ONE;
  work->revealCenterY = state->centreY / FIXED_ONE;
  work->revealFrames = REVEAL_FRAMES;
  work->effectTimerRunning = 1;
  FieldEffect_SpawnRevealMarkers();
  for (frame = 0; frame <= 18; frame++) {
    wait(1);
    state->value = frame;
  }
  Task_Add(FieldPsynergy_WatchLeaderDistance, TASK_PASS_FRAME);
}

/// Ends Reveal if it is running: stops its watch, brings the colours back from
/// black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES event.
void FieldPsynergy_EndReveal(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ScreenTransitionState *transition = gScreenTransitionWork;
  struct FieldWork *state = gFieldWork;
  s32 eventTarget;
  void *event;

  if (state->revealActive != 0) {
    Sound_PlayFar(SE_MAP_TRANSITION);
    Task_Remove(FieldPsynergy_WatchLeaderDistance);

    state->revealActive = 0;
    state->revealFrames = 0;
    Actor_SetRevealSpriteColor(SPRITE_COLOR_NORMAL);

    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    PaletteFade_SetSource(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(30);
    wait(1);

    event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES),
                                      FIELD_PSYNERGY_REVEAL, &eventTarget);
    if (event != NULL)
      Field_RunPsynergyEvent(event, gMainRoutineWork.leaderActorId,
                             eventTarget);

    if (effectWork->childOption == 0) {
      transition->nextMode = 0;
      transition->duration = 1;
      transition->step = 1;
      wait(10);
    }
  }
}
