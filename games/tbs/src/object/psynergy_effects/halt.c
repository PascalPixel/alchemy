/// @file
/// Halt.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Effect slot callback: state 0 heads for a point 40 to 70 pixels from where
/// the particle appeared; state 1 waits to arrive, then goes on to 2; state 2
/// heads for a point 4 pixels round 8 pixels above the effect's position on
/// screen; state 3 frees the slot on arrival.
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect) {
  // A for/continue retry grows the native English callback 0x100 -> 0x104.
  // Retain the immediate state-1-to-state-2 transition on arrival.
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *statePointer;
  s16 angle;
  s32 state;

  scene = gEffectWork;
  statePointer = &effect->state;
top:
  state = *statePointer;
  if (state == 0) {
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
  if (state == 1) {
    if (EffectSlot_HasPendingTarget(effect) != 0)
      return;
    *statePointer = *statePointer + 1;
    goto top;
  }
  if (state == 2) {
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
  if (state == 3 && EffectSlot_HasPendingTarget(effect) == 0)
    BattleFx_ClearOwnedSlot(effect);
}

/// Pauses the target (scene->child) and runs
/// BattleEffect_RunFallbackObjectTransition.
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused) {
  struct FieldActor *target = ((struct BattleFxScene *)gEffectWork)->child;

  target->paused = 1;
  BattleEffect_RunFallbackObjectTransition();
}

/// Throws an object from the caster to the effect's position over 10 frames
/// and scatters it into 16 particles.
void BattleEffect_RunFallbackObjectTransition(void) {
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
  orb = Object_Spawn(250, 0, 0, 0);
  step = 0;
  ObjectDispatch_SelectAnimationFar(orb, 0);
  if (orb == 0)
    return;

  // From 16 pixels above the caster to 8 pixels above the effect's position.
  BattleEffect_InitializeSharedScene();
  from.x = caster->x;
  from.y = caster->y + (16 << 16);
  from.z = caster->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
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
    Camera_WorldToScreen(&pos.x);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
    EffectSlot_Initialize(slot, 285, pos.x, pos.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
    slot++;
  }

  pos.x = orb->x;
  pos.y = orb->y + (8 << 16);
  pos.z = orb->z;
  WaitFrames(8);
  ObjectDispatch_ReleaseFar(orb);
  WaitFrames(4);
  WaitFrames(30);
  BattleFx_PrepareBufferInterpolation();
}

/// Per-frame update of a paused object: gives its sprite one of the table's
/// eight params at random.
void BattleEffect_SetRandomTableValueOnObject(union FieldObject *fieldObject) {
  void *object = (void *)fieldObject;
  s8 *table = gBattleFx_RandomChildValues;
  s32 random = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(random * 8) >> 16]);
}

/// Pauses object id, keeping its per-frame update and, if it is animated, its
/// first animation's param in gGameState.
void BattleEffect_PauseObject(s32 actorId) {
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
    object->update = BattleEffect_SetRandomTableValueOnObject;
    object->paused = 1;
    ObjectDispatch_SetAnimationSpeedFar(object, 0);
  }
}

/// Resumes object id with the update and param kept by
/// BattleEffect_PauseObject.
void BattleFx_ResumeObject(s32 actorId) {
  struct FieldActor *object = ObjectTable_Get(actorId);
  if (object != 0) {
    if (object->update == BattleEffect_SetRandomTableValueOnObject) {
      struct GameState *state = &gGameState;
      object->update = (void (*)(union FieldObject *))state->pausedUpdate;
      state->pausedUpdate = 0;
      Animation_ApplyChildValuesFar(object, state->pausedPose);
    }
    object->paused = 0;
    ObjectDispatch_SetAnimationSpeedFar(object, 16);
  }
}

/// Returns id, or -1 (no target) when the current effect's definition has
/// flags 0xff.
s32 BattleFx_FilterObjectIdByFlags(s32 id) {
  if (BattleFx_GetFlags(id) == 0xff)
    return -1;
  return id;
}
