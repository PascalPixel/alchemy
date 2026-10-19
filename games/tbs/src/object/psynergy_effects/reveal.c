/// @file
/// Reveal.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Passes value (as animation timer steps) to every one of the 64 objects
/// whose first sprite part shows sprite 200.
void ObjectDispatch_ApplyValueToKind200Children(s32 value) {
  struct ObjectRuntime *object = gObjectSlots;
  s32 remainingActors = 63;

  do {
    if (object->script != NULL) {
      u8 kind = object->animationKind;

      if (kind == 1) {
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

/// Scheduler callback of the timed effect (kind 8): asks for it to end once
/// the leader is 60 pixels from its centre or its time has run out. The
/// centre and the time are in the event work from 0xcb8 on
/// (FieldFx_RunReveal).
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

  // x is scaled by 0xd105 (about 0.82) for the comparison.
  distanceX = Iwram_MulQ16(work->revealCenterX - actor->x / 0x10000, 0xd105);
  distanceZ = work->revealCenterY - (actor->z - actor->y) / 0x10000;
  if (distanceX * distanceX + distanceZ * distanceZ >= 3600 ||
      work->revealFrames == 0) {
    u16 *slot = (u16 *)&work->command; // psynergyRequest
    u32 request = 0x2090;

    *slot = request;
  }
}

/// Abilities: Reveal (field kind 8).
///
/// Starts a screen transition centred on the leader, runs the matching event if
/// there is one, and puts markers round the leader.
void FieldFx_RunReveal(void) {
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
  BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
  BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  BattleFx_StartBufferInterpolation(1);
  // Not in the English release.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
  field->revealActive = 0;
#endif
  WaitFrames(1);
  event = BattleFx_FindMatchingEvent(0x50000005, 8, &eventTarget);
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

#define REVEAL_END_EVENT 0x40000005 ///< Psynergy event, stage 4

/// Ends Reveal if it is running: stops its watch, brings the colours back
/// from black over 30 frames and runs stage 4 of the Psynergy event.
void ResetSceneTransitionEffect(void) {
#ifdef UBFIX
  struct BattleFxScene *work = gEffectWork;
  struct DisplayTransitionState *scene = gDisplayTransition;
  struct FieldWork *state = (struct FieldWork *)gEventWork;
#else
  // Named slots avoid stepping outside gEffectWork. They add 8 bytes to this
  // function in the forced native EN build (0xe8 -> 0xf0).
  struct BattleFxScene **cell = &gEffectWork;
  struct BattleFxScene *work = *cell;
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

    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
    BattleFx_StartBufferInterpolation(1);
    BattleFx_ApplyColorToSourceBuffer(0, 0);
    BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
    BattleFx_StartBufferInterpolation(30);
    WaitFrames(1);

    event = BattleFx_FindMatchingEvent(REVEAL_END_EVENT, FIELD_KIND_REVEAL,
                                       &eventTarget);
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

#undef REVEAL_END_EVENT
