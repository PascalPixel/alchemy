/// @file
/// Reveal.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Gives every live actor of the 64 whose first sprite layer shows sprite 200
/// the sprite param value, and marks it to be redrawn. Reveal sets 6 as it
/// starts and 0 as it ends.
void Actor_SetRevealSpriteColor(s32 value) {
  struct Actor *actor = gActors;
  s32 remainingActors = 63;

  do {
    if (actor->script != NULL) {
      u8 kind = actor->spriteStorageKind;

      if (kind == ACTOR_SPRITE_ONE) {
        struct Sprite *animation = actor->sprite;
        struct SpriteAnimation *entry = animation->entries[0];

        if (entry->animationId == 200) {
          entry->colorMode = value;
          animation->dirty = kind;
        }
      }
    }
    remainingActors--;
    actor++;
  } while (remainingActors >= 0);
}

/// Task of the timed Psynergy (FIELD_PSYNERGY_REVEAL): asks for it
/// to end once the leader is 60 pixels from its centre or its time has run out.
/// The centre and the time are in the field work from 0xcb8 on
/// (FieldPsynergy_Reveal). The time counts only while effectTimerRunning is
/// set, so menus and other field Psynergy pause it. It asks every frame until
/// FieldPsynergy_EndReveal removes it.
void FieldPsynergy_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct Actor *actor;
  s16 *timer;
  s32 distanceX;
  s32 distanceZ;

  work = gFieldWork;
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  if (work->effectTimerRunning != 0) {
    timer = &work->revealFrames;
    if (*timer != 0)
      (*timer)--;
  }

  // On screen pixels (y is z less the height). x is scaled by 0xd105 (about
  // 0.82) for the comparison, so the edge is an ellipse 60 pixels up and
  // down from the centre and about 73 to either side.
  distanceX =
      Iwram_MulQ16(work->revealCenterX - actor->x.fixed / Q16_ONE, 0xd105);
  distanceZ = work->revealCenterY - (actor->z.fixed - actor->y.fixed) / Q16_ONE;
  if (distanceX * distanceX + distanceZ * distanceZ >= 3600 ||
      work->revealFrames == 0) {
    // Raise Reveal as a Psynergy request that pays no PP (ACT_PACKED 0x2000
    // | ABILITY_REVEAL 0x90), which runs its ending events.
    u16 *slot = (u16 *)&work->psynergy;
    u32 request = 0x2090;

    *slot = request;
  }
}

/// Starts a screen transition centred on the leader, runs the matching event if
/// there is one, and puts markers round the leader.
///
/// Abilities: Reveal (FIELD_PSYNERGY_REVEAL).
///
/// The transition is WINDOW_TRANSITION_RIPPLE, a wobbling circle round the
/// leader stepped over 19 frames, and the BG colours turn grey. Reveal lasts
/// 600 frames of its own time, or until the leader leaves its circle
/// (FieldPsynergy_WatchLeaderDistance), and FieldPsynergy_EndReveal ends it.
/// Called by FieldPsynergy_Run and FieldPsynergy_Dispatch.
void FieldPsynergy_Reveal(void) {
  struct FieldWork *work;
  struct ScreenTransitionState *state;
  struct Actor *target;
  s32 eventTarget;
  void *event;
  s32 frame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  Actor_SetRevealSpriteColor(6);
  ScreenTransition_InitializeBattleEffectState(8);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_SCREEN_TRANSITION);
  target = Actor_Lookup(gMainRoutineWork.leaderActorId);
  state->centreX = target->x.fixed;
  state->centreY = target->z.fixed - target->y.fixed;
  // From the plain colours (PALETTE_TINT_NONE, 0x10000) the BG palettes
  // turn grey (PALETTE_TINT_GRAYSCALE, 0x10001) in one frame.
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, 1);
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
  work->revealCenterX = state->centreX / Q16_ONE;
  work->revealCenterY = state->centreY / Q16_ONE;
  work->revealFrames = 600;
  work->effectTimerRunning = 1;
  FieldEffect_SpawnNearbyMarkers();
  frame = 0;
  do {
    wait(1);
    state->value = frame;
    frame++;
  } while (frame <= 18);
  Task_Add(FieldPsynergy_WatchLeaderDistance, TASK_PASS_FRAME);
}

/// Ends Reveal if it is running: stops its watch, brings the colours back
/// from black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES
/// event. Unless an event set childOption, the window transition is then
/// reset at once (no next mode, one frame). Called as Reveal ends
/// (Event_RunFieldPsynergy) and before Mind Read starts its own colours.
void FieldPsynergy_EndReveal(void) {
#ifdef UBFIX
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct ScreenTransitionState *transition = gScreenTransitionWork;
  struct FieldWork *state = gFieldWork;
#else
  // Named slots avoid stepping outside gFieldEffectWork. They add 8 bytes to
  // this function in the forced native EN build (0xe8 -> 0xf0).
  struct FieldEffectWork **cell = &gFieldEffectWork;
  struct FieldEffectWork *effectWork = *cell;
  struct ScreenTransitionState *transition =
      *(struct ScreenTransitionState **)((u8 *)cell - 0x64);
  struct FieldWork *state = *(struct FieldWork **)((u8 *)cell - 0x74);
#endif
  s16 zero;
  s32 eventTarget;
  void *event;

  if (state->revealActive != 0) {
    Sound_PlayFar(SE_MAP_TRANSITION);
    Task_Remove(FieldPsynergy_WatchLeaderDistance);

    zero = 0;
    state->revealActive = zero;
    state->revealFrames = zero;
    Actor_SetRevealSpriteColor(0);

    PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_StartInterpolation(1);
    PaletteFade_SetSource(0, 0);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
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
