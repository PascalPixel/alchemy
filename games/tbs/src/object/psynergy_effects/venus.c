/// @file
/// A Venus Djinni joining the party.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// A Venus Djinni joins the party: it hops twice, spins and leaps, then
/// shrinks away; the leader turns to the camera and plays animation 28 while
/// 24 particles rise round it (FieldEffect_UpdateRadialSpread). Then 24 motes
/// spiral down onto the leader from 120 pixels above
/// (FieldEffect_UpdateDescendingOrbitObject) and 8 sparks burst from the
/// leader. Called by Djinn_JoinParty for a Venus Djinni, which then clears the
/// Djinni's actor slot. Uses all 24 effect slots, tints the field palette green
/// and fades it back at the end.
void FieldEffect_RunVenusDjinnCapture(s32 djinniId) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  struct FieldEffectWork *effectWork;
  struct EffectSlot *effect;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  struct EffectVector position;

  leader = (struct CaptureObject *)ObjectEvent_GetById(
      gGameState.selectedObjectEvent);
  djinni = (struct CaptureObject *)ObjectEvent_GetById(djinniId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectWork = gEffectWork;
  // Tint the screen green: PALETTE_TRANSFORM_TINT | RGB(4, 16, 4), the form
  // mercury.c spells out for its blue.
  Shop_SaveAndTintFieldPaletteFar(0x201204);
  WaitFrames(30);
  // Unpause the Djinni (CaptureObject.visible is ObjectEvent.paused).
  djinni->visible = 0;
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 15);
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 15);
  // Spin and leap, then shrink away while the screen shakes (1.3 pixels).
  WaitFrames(30);
  djinni->callback = FieldEffect_AdvanceSpinAngle;
  AudioCommand_PlayFar(SE_HIGH_JUMP);
  ObjectMotion_Launch(djinniId, 8, 22);
  AudioCommand_PlayFar(SE_140);
  MapGrid_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = FieldEffect_ShrinkObjectAndDestroyFast;
  ObjectDispatch_SelectAnimationFar(djinni, 3);
  // The leader turns to face the camera (angle 0x4000).
  WaitFrames(90);
  ObjectMotion_ArmCallback(gGameState.selectedObjectEvent, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_SelectAnimationFar(
      ObjectEvent_GetById(gGameState.selectedObjectEvent), 28);
  WaitFrames(30);
  MapGrid_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position);

  // One rising particle a frame into each of the 24 effect slots.
  effect = effectWork->effects;
  remaining = 23;
  do {
    EffectSlot_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    EffectSlot_SetCallback(effect, FieldEffect_UpdateRadialSpread);
    EffectSlot_SetObjectMode(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              SPRITE_COLOR_GREEN_TINT);
    effect->scaleX = 0x8000;
    effect->scaleY = Random16() + 0x18000;
    remaining--;
    WaitFrames(1);
    effect++;
  } while (remaining >= 0);

  // After 140 frames every particle still in use stops rising.
  WaitFrames(140);
  // Walking whole slots instead of their state bytes grows native English
  // from 0x2dc to 0x2e0 bytes.
  {
    s32 nextState;

    nextState = VENUS_PARTICLE_LAST_RISE;
    slotState = (s8 *)effectWork;
    slotState += offsetof(struct FieldEffectWork, effects) +
                 offsetof(struct EffectSlot, state);
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  // A shake of 1/65536 pixel: in effect the shake stops.
  WaitFrames(20);
  MapGrid_SetScreenShakeFar(1, 1, 1);
  WaitFrames(30);

  // 24 motes, evenly spread round a circle, 120 pixels above the leader.
  for (remaining = 0; remaining <= 23; remaining++) {
    position.x = leader->x;
    position.y = leader->y + 0x780000;
    position.z = leader->z;
    djinni = (struct CaptureObject *)Object_Spawn(SPRITE_PARTICLE, position.x,
                                                  position.y, position.z);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = FieldEffect_UpdateDescendingOrbitObject;
      djinni->mode = 0;
      ObjectDispatch_SelectAnimationFar(djinni, 7);
      Animation_ApplyChildValuesFar((struct DispatchObject *)djinni,
                                    SPRITE_COLOR_GREEN_TINT);
    }
  }

  WaitFrames(100);
  AudioCommand_PlayFar(SE_STOP_SLOT3);
  WaitFrames(1);
  AudioCommand_PlayFar(SE_151);
  position.x = leader->x;
  position.y = leader->y + 0x120000;
  position.z = leader->z;
  // 8 sparks from 18 pixels above the leader, each thrown 32 pixels in a
  // random direction at 1.4 to 2.4 pixels a frame and 5 up (gravity is
  // ObjectEvent.velocityY), falling, and freed after 8 frames; stop at the
  // first that cannot be spawned.
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Object_Spawn(SPRITE_PARTICLE, position.x,
                                                  position.y, position.z);
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
    Animation_ApplyChildValuesFar((struct DispatchObject *)djinni,
                                  SPRITE_COLOR_GREEN_TINT);
    djinni->lifetime = 8;
    ObjectDispatch_InitializeFar((struct DispatchObject *)djinni,
                                 gFieldEffect_CommonParticleScript);
  }

  WaitFrames(15);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Effect slot callback of a Mercury Djinni's droplets: each falls from 100
/// pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and
/// where it lands splashes 12 pixels in a random direction, at half size,
/// with SE_143. It then falls again 6 frames later, until
/// FieldEffect_RunMercuryDjinnCapture sets MERCURY_DROPLET_LAST_FALL: its next
/// splash is its last (enum MercuryDropletState).
void FieldEffect_UpdateRandomTargetParticle(struct EffectSlot *effect) {
  struct ObjectRuntime *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = (struct ObjectRuntime *)ObjectEvent_GetById(
      gGameState.selectedObjectEvent);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == MERCURY_DROPLET_AIM) {
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
    effect->speedMax = Random16() * 3 + 0x30000;
    effect->scaleX = 0x10000;
    effect->scaleY = 0x10000;
    effect->turnStepLimited = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - MERCURY_DROPLET_FALLING) <=
             MERCURY_DROPLET_LAST_FALL - MERCURY_DROPLET_FALLING) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = 0x10000;
      effect->acceleration = 0;
      effect->speedMax = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      AudioCommand_PlayFar(SE_143);

      if (*statePointer == MERCURY_DROPLET_FALLING)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == MERCURY_DROPLET_DONE) {
    FieldEffect_ClearOwnedSlot(effect);
  }
}

/// Per-frame update of an afterimage: closes half the distance to the object
/// it is linked to. Set on the Mercury Djinni's afterimages by
/// FieldEffect_RunMercuryDjinnCapture.
void FieldEffect_HalveDistanceToTarget(union FieldObject *fieldObject) {
  FieldEffect_MoveAfterimageHalfway(fieldObject);
}
