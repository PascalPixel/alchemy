/// @file
/// Halt.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Effect slot callback (enum OutAndBackState): the particle heads for a point
/// 40 to 70 pixels from where it appeared, then for a point 4 pixels round 8
/// pixels above the effect's position on screen, and is freed on arrival. Set
/// on the 16 particles of FieldFx_RunHalt. They fly
/// out straight at up to 4 pixels a frame and come back turning at most 1/16
/// turn a frame.
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect) {
  // A for/continue retry grows the native English callback 0x100 -> 0x104.
  // Retain the immediate OUT_AND_BACK_FLYING_OUT to OUT_AND_BACK_AIM_BACK
  // transition on arrival.
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *statePointer;
  s16 angle;
  s32 state;

  scene = gEffectWork;
  statePointer = &effect->state;
top:
  state = *statePointer;
  if (state == OUT_AND_BACK_AIM_OUT) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->limitTurnStep = state;
    *statePointer = *statePointer + 1;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (EffectSlot_HasPendingTarget(effect) != 0)
      return;
    *statePointer = *statePointer + 1;
    goto top;
  }
  if (state == OUT_AND_BACK_AIM_BACK) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *statePointer = *statePointer + 1;
    return;
  }
  if (state == OUT_AND_BACK_FLYING_BACK &&
      EffectSlot_HasPendingTarget(effect) == 0)
    BattleFx_ClearOwnedSlot(effect);
}

/// Pauses the target (scene->child) and runs
/// FieldFx_RunHalt. Called by BattleFx_Run and
/// BattleFx_DispatchRequestKind for Halt on a target; sets the target's
/// paused field.
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused) {
  struct FieldActor *target = ((struct BattleFxScene *)gEffectWork)->child;

  target->paused = 1;
  FieldFx_RunHalt();
}

/// Throws an object from the caster to the effect's position over 10 frames
/// and scatters it into 16 particles. Halt's visible effect: called by
/// BattleFx_Run when no target with a talk event is near, and through
/// BattleFx_MarkChildAndRunFallbackTransition otherwise. Fills effect slots 0
/// to 15 and sets scene->y to the caster's height.
///
/// Abilities: Halt (FIELD_KIND_HALT).
void FieldFx_RunHalt(void) {
  struct BattleFxScene *scene = gEffectWork;
  struct MotionObject *caster = scene->mainObject;
  struct MotionObject *orb;
  struct EffectSlot *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = caster->y;
  orb = (struct MotionObject *)Object_Spawn(250, 0, 0, 0);
  step = 0;
  ObjectDispatch_SelectAnimationFar(orb, 0);
  if (orb == 0)
    return;

  // From 16 pixels above the caster to 8 pixels above the effect's position.
  FieldFx_FadeToEffectColor();
  from.x = caster->x;
  from.y = caster->y + (16 << 16);
  from.z = caster->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  // Steps 0 to 10: the orb arrives on the 11th frame, grown to full size.
  for (;;) {
    s32 steps = 11;
    s32 scale;

    SET_POSITION(orb, from.x + (to.x - from.x) * step / 10,
                 from.y + (to.y - from.y) * step / 10,
                 from.z + (to.z - from.z) * step / 10);
    scale = BattleFx_Interpolate(0x10000, 0x4000, step); // from 0.25 to full
    orb->scaleX = scale;
    orb->scaleY = scale;
    WaitFrames(1);
    step++;
    if (step >= steps)
      break;
  }

  // Three low tones and a high one, 10 frames apart, as the orb changes to
  // its second animation.
  WaitFrames(5);
  ObjectDispatch_SelectAnimationFar(orb, 1);
  AudioCommand_PlayFar(SE_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SE_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SE_TRIPLE_TONE_LOW);
  WaitFrames(10);
  AudioCommand_PlayFar(SE_TRIPLE_TONE_HIGH);

  // Particles 4 pixels round it fly out, then come back to the effect's
  // position.
  slot = &scene->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = orb->x;
    pos.y = orb->y + (8 << 16);
    pos.z = orb->z;
    Camera_WorldToScreen((struct EffectVector *)&pos.x);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
    EffectSlot_Initialize(slot, SPRITE_SPARKLE, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, SPRITE_COLOR_WHITE);
    slot++;
  }

  pos.x = orb->x;
  pos.y = orb->y + (8 << 16);
  pos.z = orb->z;
  WaitFrames(8);
  ObjectDispatch_ReleaseFar((struct DispatchObject *)orb);
  WaitFrames(4);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

/// Per-frame update of a paused object: gives its sprite one of the table's
/// eight params at random. FieldFx_PauseObject sets it, and
/// BattleFx_ResumeObject looks for it to know the object was paused this way.
void FieldFx_UpdatePausedObject(union FieldObject *fieldObject) {
  void *object = (void *)fieldObject;
  s8 *table = gBattleFx_RandomChildValues;
  s32 random = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(random * 8) >> 16]);
}

/// Pauses object id, keeping its per-frame update and, if it is animated, its
/// first animation's param in gGameState. Called by BattleFx_Run and
/// BattleFx_DispatchRequestKind for Halt, which then record the id in
/// gGameState.pausedObject, and by the Colosso scenes to stop their operators.
/// Sets gGameState.pausedUpdate and pausedPose, the object's update and paused
/// fields, and stops its animation. gGameState keeps one object's state, so
/// Halt resumes the one it stopped before it stops another; the paused
/// object flickers through random sprite params meanwhile.
void FieldFx_PauseObject(s32 actorId) {
  struct FieldActor *object;
  struct AnimationEntry *entry;

  object = ObjectTable_Get(actorId);
  if (object != NULL) {
    gGameState.pausedUpdate = (u32)object->update;
    gGameState.pausedPose = 0;
    if (object->spriteStorageKind == 1) {
      entry = object->sprite->entries[0];
      if (entry != NULL) {
        gGameState.pausedPose = entry->param;
      }
    }
    object->update = FieldFx_UpdatePausedObject;
    object->paused = 1;
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 0);
  }
}

/// Resumes object id with the update and param kept by
/// FieldFx_PauseObject. Called for gGameState.pausedObject by BattleFx_Run
/// and BattleFx_DispatchRequestKind before Halt stops another object, and by
/// Scene_TalkToActor after talking to the paused actor. Clears
/// gGameState.pausedUpdate and the object's paused field, and restarts its
/// animation at speed 16.
void BattleFx_ResumeObject(s32 actorId) {
  struct FieldActor *object = ObjectTable_Get(actorId);
  if (object != 0) {
    if (object->update == FieldFx_UpdatePausedObject) {
      struct GameState *state = &gGameState;
      object->update = (void (*)(union FieldObject *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Animation_ApplyChildValuesFar((struct DispatchObject *)object,
                                    state->pausedPose);
    }
    object->paused = 0;
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 16);
  }
}

/// Returns id, or -1 (no target) when actor id's sprite has flags 0xff in
/// gBattleFx_DefinitionTable (BattleFx_GetFlags): the 20 rows before the
/// default. Called by BattleFx_Run on the nearest Halt target, so Halt does
/// not stop those actors; an unlisted sprite takes the default row's flags 0.
s32 BattleFx_FilterObjectIdByFlags(s32 id) {
  if (BattleFx_GetFlags(id) == 0xff)
    return -1;
  return id;
}
