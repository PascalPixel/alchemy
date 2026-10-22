/// @file
/// Reveal.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Gives every live object of the 64 whose first sprite layer shows sprite 200
/// the sprite param value, and marks it to be redrawn. Reveal sets 6 as it
/// starts and 0 as it ends.
void ObjectDispatch_ApplyValueToKind200Children(s32 value) {
  struct ObjectRuntime *object = gObjectSlots;
  s32 remainingActors = 63;

  do {
    if (object->script != NULL) {
      u8 kind = object->animationKind;

      if (kind == OBJECT_SPRITE_ONE) {
        struct AnimationObject *animation = object->animation;
        struct AnimationEntry *entry = animation->entries[0];

        if (entry->animId == 200) {
          entry->param = value;
          animation->dirty = kind;
        }
      }
    }
    remainingActors--;
    object++;
  } while (remainingActors >= 0);
}

/// Scheduler callback of the timed effect (FIELD_KIND_REVEAL): asks for it to
/// end once the leader is 60 pixels from its centre or its time has run out.
/// The centre and the time are in the event work from 0xcb8 on
/// (FieldEffect_RunReveal). The time counts only while effectTimerRunning is
/// set, so menus and other effects pause it. It asks every frame until
/// ResetSceneTransitionEffect removes it.
void FieldEffect_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct ObjectRuntime *actor;
  s16 *timer;
  s32 distanceX;
  s32 distanceZ;

  work = (struct FieldWork *)gEventWork;
  actor = ObjectTable_Get(gGameState.selectedActor);
  if (work->effectTimerRunning != 0) {
    timer = &work->revealFrames;
    if (*timer != 0)
      (*timer)--;
  }

  // On screen pixels (y is z less the height). x is scaled by 0xd105 (about
  // 0.82) for the comparison, so the edge is an ellipse 60 pixels up and
  // down from the centre and about 73 to either side.
  distanceX = Iwram_MulQ16(work->revealCenterX - actor->x / 0x10000, 0xd105);
  distanceZ = work->revealCenterY - (actor->z - actor->y) / 0x10000;
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
/// Abilities: Reveal (FIELD_KIND_REVEAL).
///
/// The transition is WINDOW_TRANSITION_RIPPLE, a wobbling circle round the
/// leader stepped over 19 frames, and the BG colours turn grey. Reveal lasts
/// 600 frames of its own time, or until the leader leaves its circle
/// (FieldEffect_WatchLeaderDistance), and ResetSceneTransitionEffect ends it.
/// Called by FieldEffect_Run and FieldEffect_DispatchRequestKind.
void FieldEffect_RunReveal(void) {
  struct FieldWork *field;
  struct DisplayTransitionState *transition;
  struct FieldActor *target;
  s32 eventTarget;
  void *event;
  s32 frame;

  field = WORK_SLOT(HEAP_SLOT_EVENT);
  ObjectDispatch_ApplyValueToKind200Children(6);
  DisplayTransition_InitializeBattleEffectState(8);
  transition = WORK_SLOT(HEAP_SLOT_TRANSITION);
  target = ObjectTable_Get(gGameState.selectedActor);
  transition->centreX = target->x.fixed;
  transition->centreY = target->z.fixed - target->y.fixed;
  // From the plain colours (PALETTE_TRANSFORM_PLAIN, 0x10000) the BG half
  // turns grey (PALETTE_TRANSFORM_GREY, 0x10001) in one frame.
  ColorBuffer_SetSource(0x10000, 0);
  ColorBuffer_SetTarget(0x10001, 1);
  ColorBuffer_StartInterpolation(1);
  // Not in the English release.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
  field->revealActive = 0;
#endif
  WaitFrames(1);
  event = FieldEffect_FindMatchingEvent(PSYNERGY_EV(PSYNERGY_PHASE_EFFECT_ACTS),
                                        FIELD_KIND_REVEAL, &eventTarget);
  if (event != NULL)
    Field_RunEventAction(event, gGameState.selectedActor, eventTarget);
  AudioCommand_PlayFar(SE_131);

  // Record the centre (in pixels) and the time in the event work, and put
  // markers nearby.
  field->revealActive = 1;
  field->revealCenterX = transition->centreX / 0x10000;
  field->revealCenterY = transition->centreY / 0x10000;
  field->revealFrames = 600;
  field->effectTimerRunning = 1;
  FieldEffect_SpawnNearbyMarkers();
  frame = 0;
  do {
    WaitFrames(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

/// Ends Reveal if it is running: stops its watch, brings the colours back
/// from black over 30 frames and runs the scene's PSYNERGY_PHASE_EFFECT_ENDS
/// event. Unless a scene set childOption, the window transition is then reset
/// at once (no next mode, one frame). Called as Reveal ends
/// (FieldEvent_RunTypeHandler) and before Mind Read starts its own colours.
void ResetSceneTransitionEffect(void) {
#ifdef UBFIX
  struct FieldFxScene *work = gEffectWork;
  struct DisplayTransitionState *scene = gDisplayTransition;
  struct FieldWork *state = (struct FieldWork *)gEventWork;
#else
  // Named slots avoid stepping outside gEffectWork. They add 8 bytes to this
  // function in the forced native EN build (0xe8 -> 0xf0).
  struct FieldFxScene **cell = &gEffectWork;
  struct FieldFxScene *work = *cell;
  struct DisplayTransitionState *scene =
      *(struct DisplayTransitionState **)((u8 *)cell - 0x64);
  struct FieldWork *state = *(struct FieldWork **)((u8 *)cell - 0x74);
#endif
  s16 zero;
  s32 eventTarget;
  void *event;

  if (state->revealActive != 0) {
    AudioCommand_PlayFar(SE_SCENE_TRANSITION);
    Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

    zero = 0;
    state->revealActive = zero;
    state->revealFrames = zero;
    ObjectDispatch_ApplyValueToKind200Children(0);

    ColorBuffer_SetTarget(PALETTE_TRANSFORM_PLAIN, 1);
    ColorBuffer_StartInterpolation(1);
    ColorBuffer_SetSource(0, 0);
    ColorBuffer_SetTarget(PALETTE_TRANSFORM_PLAIN, 0);
    ColorBuffer_StartInterpolation(30);
    WaitFrames(1);

    event =
        FieldEffect_FindMatchingEvent(PSYNERGY_EV(PSYNERGY_PHASE_EFFECT_ENDS),
                                      FIELD_KIND_REVEAL, &eventTarget);
    if (event != NULL)
      Field_RunEventAction(event, gGameState.selectedActor, eventTarget);

    if (work->childOption == 0) {
      scene->nextMode = 0;
      scene->duration = 1;
      scene->step = 1;
      WaitFrames(10);
    }
  }
}
