/// @file
/// Part of psynergy_effects.c, compiled only through it.
/// A Venus Djinni joining the party.

#include "../psynergy_effects_internal.h"

/// A Venus Djinni joins the party: it hops twice, spins and leaps, then
/// shrinks away; the leader turns to the camera and plays animation 28 while
/// 24 particles rise round it (BattleFx_UpdateRadialSpread). Then 24 motes
/// spiral down onto the leader from 120 pixels above
/// (BattleFx_UpdateDescendingOrbitObject) and 8 sparks burst from the leader.
void BattleFx_RunVenusDjinnCapture(s32 djinniId) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  struct BattleFxScene *effectScene;
  struct EffectSlot *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  struct EffectVector position;

  leader = (struct CaptureObject *)Object_GetById(gGameState.selectedActor);
  djinni = (struct CaptureObject *)Object_GetById(djinniId);
  if (djinni == NULL)
    return;

  BattleFx_InitializeSlots();
  effectScene = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  WaitFrames(30);
  djinni->visible = 0;
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 15);
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 15);
  WaitFrames(30);
  djinni->callback = BattleFx_AdvanceSpinAngle;
  AudioCommand_PlayFar(SE_HIGH_JUMP);
  ObjectMotion_Launch(djinniId, 8, 22);
  AudioCommand_PlayFar(SE_140);
  Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = BattleFx_ShrinkObjectAndDestroyFast;
  ObjectDispatch_SelectAnimationFar(djinni, 3);
  WaitFrames(90);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_SelectAnimationFar(Object_GetById(gGameState.selectedActor),
                                    28);
  WaitFrames(30);
  Map_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position);

  slot = effectScene->slots;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 284, position.x, position.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRadialSpread);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 11);
    slot->scaleX = 0x8000;
    slot->scaleY = Random16() + 0x18000;
    remaining--;
    WaitFrames(1);
    slot++;
  } while (remaining >= 0);

  WaitFrames(140);
  // Walking whole slots instead of their state bytes grows native English
  // from 0x2dc to 0x2e0 bytes.
  {
    s32 nextState;

    nextState = 2;
    slotState = (s8 *)effectScene;
    slotState += offsetof(struct BattleFxScene, slots) +
                 offsetof(struct EffectSlot, state);
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  WaitFrames(20);
  Map_SetScreenShakeFar(1, 1, 1);
  WaitFrames(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position.x = leader->x;
    position.y = leader->y + 0x780000;
    position.z = leader->z;
    djinni = (struct CaptureObject *)Object_Spawn(284, position.x, position.y,
                                                  position.z);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = BattleFx_UpdateDescendingOrbitObject;
      djinni->mode = 0;
      ObjectDispatch_SelectAnimationFar(djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  WaitFrames(100);
  AudioCommand_PlayFar(SE_288);
  WaitFrames(1);
  AudioCommand_PlayFar(SE_151);
  position.x = leader->x;
  position.y = leader->y + 0x120000;
  position.z = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Object_Spawn(284, position.x, position.y,
                                                  position.z);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = 0x50000;
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                               0x200000, Random16());
    Animation_ApplyChildValuesFar(djinni, 11);
    djinni->lifetime = 8;
    ObjectDispatch_InitializeFar((struct DispatchObject *)djinni,
                                 gBattleFx_CommonParticleScript);
  }

  WaitFrames(15);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

/// Effect slot callback of a Mercury Djinni's droplets: each falls from 100
/// pixels above to a random point near the leader (state 0), then darts
/// between random points 12 pixels apart (states 1 and 2, a sound each time)
/// until the sequence sets state 3, which frees it.
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect) {
  struct ObjectRuntime *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    position.z = source->z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->x = position.x;
    effect->z = position.z - 0x640000;
    effect->acceleration = 0x30000;
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = 0x10000;
    effect->scaleY = 0x10000;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = 0x10000;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      AudioCommand_PlayFar(SE_143);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    BattleFx_ClearOwnedSlot(effect);
  }
}

/// Per-frame update of an afterimage: closes half the distance to the object
/// it is linked to.
void BattleFx_HalveDistanceToTarget(union FieldObject *fieldObject) {
  BattleFx_MoveAfterimageHalfway(fieldObject);
}
