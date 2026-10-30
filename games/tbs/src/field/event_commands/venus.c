/// @file
/// A Venus Djinni joining the party.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// A Venus Djinni joins the party: it hops twice, spins and leaps, then shrinks
/// away; the leader turns to the camera and plays ANIM_LEADER_ARMS_RAISED while
/// 24 particles rise round it (FieldEffect_UpdateRadialSpread). Then 24 motes
/// spiral down onto the leader from 120 pixels above
/// (FieldEffect_UpdateDescendingOrbitActor) and 8 sparks burst from the
/// leader. Called by Djinn_JoinParty for a Venus Djinni, which then clears the
/// Djinni's actor slot. Uses all 24 field effect sprites, tints the
/// field palette green and fades it back at the end.
void FieldEffect_RunVenusDjinnCapture(s32 djinniId) {
  struct Actor *leader;
  struct Actor *djinni;
  struct FieldEffectWork *effectWork;
  struct FieldParticle *effect;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  struct Vector3 position;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectWork = gFieldEffectWork;
  // Tint the screen green: PALETTE_TINT_CUSTOM | RGB(4, 16, 4), the form
  // mercury.c spells out for its blue.
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 16, 4));
  wait(30);
  // Unpause the Djinni (its paused flag).
  djinni->paused = 0;
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  // Spin and leap, then shrink away while the screen shakes (1.3 pixels).
  wait(30);
  djinni->update = FieldEffect_AdvanceSpinAngle;
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 8, 22);
  Sound_PlayFar(SE_SPIN);
  MapGrid_SetScreenShakeFar(Q16_ONE * 13 / 10, Q16_ONE * 13 / 10, Q16_ONE);
  djinni->update = FieldEffect_ShrinkActorAndDestroyFast;
  Actor_SelectAnimationFar(djinni, 3);
  // The leader turns to face the camera (FACING_SOUTH).
  wait(90);
  Actor_ArmCallback(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(30);
  MapGrid_SetScreenShakeFar(Q16_ONE * 8 / 5, Q16_ONE * 8 / 5, Q16_ONE);

  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  // One rising particle a frame into each of the 24 field effect sprites.
  effect = effectWork->particles;
  remaining = 23;
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialSpread);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, SPRITE_COLOR_GREEN_TINT);
    effect->scaleX = Q16_ONE / 2;
    effect->scaleY = Random16() + Q16_ONE * 3 / 2;
    remaining--;
    wait(1);
    effect++;
  } while (remaining >= 0);

  // After 140 frames every particle still in use stops rising.
  wait(140);
  // Walking whole slots instead of their state bytes grows native English
  // from 732 to 736 bytes.
  {
    s32 nextState;

    nextState = VENUS_PARTICLE_LAST_RISE;
    slotState = (s8 *)effectWork;
    slotState += offsetof(struct FieldEffectWork, particles) +
                 offsetof(struct FieldParticle, state);
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  // A shake of 1/65536 pixel: in effect the shake stops.
  wait(20);
  MapGrid_SetScreenShakeFar(1, 1, 1);
  wait(30);

  // 24 motes, evenly spread round a circle, 120 pixels above the leader.
  for (remaining = 0; remaining <= 23; remaining++) {
    position.x = leader->x.fixed;
    position.y = leader->y.fixed + PIXELS(120);
    position.z = leader->z.fixed;
    djinni = Actor_Spawn(SPRITE_PARTICLE, position.x, position.y, position.z);
    if (djinni != NULL) {
      scale = Random16() / 3 + Q16_ONE;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->orbitFrames = 100;
      djinni->orbitAngle = (remaining << 16) / 24;
      djinni->update = FieldEffect_UpdateDescendingOrbitActor;
      djinni->motionFlags = 0;
      Actor_SelectAnimationFar(djinni, 7);
      Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    }
  }

  wait(100);
  Sound_PlayFar(SE_STOP_ACTION_SE);
  wait(1);
  Sound_PlayFar(SE_LIGHT_APPEAR);
  position.x = leader->x.fixed;
  position.y = leader->y.fixed + PIXELS(18);
  position.z = leader->z.fixed;
  // 8 sparks from 18 pixels above the leader, each thrown 32 pixels in a
  // random direction at 1.4 to 2.4 pixels a frame and 5 up (gravity is
  // Actor.velocityY), falling, and released after 8 frames; stop at the
  // first that cannot be spawned.
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = Actor_Spawn(SPRITE_PARTICLE, position.x, position.y, position.z);
    if (djinni == NULL)
      break;
    djinni->scaleY = Q16_ONE * 3 / 5;
    djinni->scaleX = Q16_ONE * 3 / 5;
    djinni->motionFlags = 2;
    djinni->velocityY = PIXELS(5);
    djinni->groundHeight = djinni->y.fixed;
    djinni->speedLimit = Random16() + Q16_ONE * 7 / 5;
    Actor_SetTargetPositionFromMagnitudeAngle(djinni, PIXELS(32), Random16());
    Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    djinni->wait = 8;
    Actor_StartScriptFar(djinni, gFieldEffect_SharedParticleScript);
  }

  wait(15);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Field effect sprite callback of a Mercury Djinni's droplets: each falls from
/// 100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
/// splashes 12 pixels in a random direction, at half size, with SE_CRACK. It
/// then falls again 6 frames later, until FieldEffect_RunMercuryDjinnCapture
/// sets MERCURY_DROPLET_LAST_FALL: its next splash is its last (enum
/// MercuryDropletState).
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect) {
  struct Actor *source;
  struct Vector3 position;
  s8 *statePointer;
  s32 state;

  source = Actor_GetById(gMainRoutineWork.leaderActorId);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == MERCURY_DROPLET_AIM) {
    position.x = source->x.fixed;
    position.y = source->y.fixed + Random16() * 5 + PIXELS(15);
    position.z = source->z.fixed;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(Random16() * 6 + PIXELS(2), Random16(), &position);

    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->x = position.x;
    effect->z = position.z - PIXELS(100);
    effect->acceleration = PIXELS(3);
    effect->speedMax = Random16() * 3 + PIXELS(3);
    effect->scaleX = Q16_ONE;
    effect->scaleY = Q16_ONE;
    effect->turnStepLimited = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - MERCURY_DROPLET_FALLING) <=
             MERCURY_DROPLET_LAST_FALL - MERCURY_DROPLET_FALLING) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(PIXELS(12), Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = Q16_ONE;
      effect->acceleration = 0;
      effect->speedMax = Random16() + Q16_ONE * 11 / 5;
      effect->scaleX = Q16_ONE / 2;
      effect->scaleY = Q16_ONE / 2;
      Sound_PlayFar(SE_CRACK);

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

/// Per-frame update of an afterimage: closes half the distance to the actor
/// it watches. Set on the Mercury Djinni's afterimages by
/// FieldEffect_RunMercuryDjinnCapture.
void FieldEffect_HalveDistanceToTarget(struct Actor *actor) {
  FieldEffect_MoveAfterimageHalfway(actor);
}
