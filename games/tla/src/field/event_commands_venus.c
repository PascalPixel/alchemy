/// @file
/// A Venus Djinni joining the party, and a Mercury Djinni's droplets.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field.h"

struct FieldEffectState;

/// The motes that circle down onto the leader as a Venus Djinni joins.
enum { VENUS_JOIN_MOTES = 24 };

/// A Venus Djinni joins the party: it hops twice, spins and leaps, then shrinks
/// away; the leader turns to the camera and plays ANIM_LEADER_ARMS_RAISED while
/// 24 particles rise round it (FieldEffect_UpdateRadialSpread).
void Djinn_RunVenusJoinEffect(s32 djinniId) {
  struct Actor *leader;
  struct Actor *djinni;
  struct FieldEffectState *effectState;
  struct FieldParticle *effect;
  s8 *slotState;
  s32 remaining;
  FIXED scale;
  struct Vector3 position;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectState = Ram_HeapSlots->fieldEffectState;
  // Tint the screen green: PALETTE_TINT_CUSTOM | RGB(4, 16, 4), the form
  // event_commands_mercury.c spells out for its blue.
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 16, 4));
  wait(30);
  djinni->paused = 0;
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  // Spin and leap, then shrink away while the screen shakes (1.3 pixels).
  wait(30);
  djinni->update = (ObjectUpdate)Djinn_AdvanceSpinAngle;
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 8, 22);
  Sound_PlayFar(SE_SPIN);
  MapGrid_SetScreenShakeFar(toFIXED(13) / 10, toFIXED(13) / 10, FIXED_ONE);
  djinni->update = (ObjectUpdate)FieldEffect_ShrinkActorAndDestroyFast;
  Actor_SelectAnimationFar(djinni, 3);
  // The leader turns to face the camera (FACING_SOUTH).
  wait(90);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineState.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(30);
  MapGrid_SetScreenShakeFar(toFIXED(8) / 5, toFIXED(8) / 5, FIXED_ONE);

  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  // One rising particle a frame into each of the 24 field effect sprites.
  effect = effectState->particles;
  remaining = ARRAY_COUNT(effectState->particles) - 1;
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialSpread);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, SPRITE_COLOR_GREEN_TINT);
    effect->scaleX = FIXED_ONE / 2;
    effect->scaleY = Random16() + FIXED_ONE * 3 / 2;
    remaining--;
    wait(1);
    effect++;
  } while (remaining >= 0);

  // After 140 frames every particle still in use stops rising.
  wait(140);
  FIELD_PARTICLES_SET_ACTIVE_STATE(slotState, remaining, effectState, particles,
                                   VENUS_PARTICLE_LAST_RISE);

  // A shake of 1/65536 pixel: in effect the shake stops.
  wait(20);
  MapGrid_SetScreenShakeFar(1, 1, 1);
  wait(30);

  // The motes, evenly spread round a circle, 120 pixels above the leader.
  for (remaining = 0; remaining < VENUS_JOIN_MOTES; remaining++) {
    position.x = leader->x.fixed;
    position.y = leader->y.fixed + toFIXED(120);
    position.z = leader->z.fixed;
    djinni = Actor_Spawn(SPRITE_PARTICLE, position.x, position.y, position.z);
    if (djinni != NULL) {
      scale = Random16() / 3 + FIXED_ONE;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->orbitFrames = 100;
      djinni->orbitAngle = remaining * TRIG_TURN / VENUS_JOIN_MOTES;
      djinni->update = (ObjectUpdate)FieldEffect_UpdateDescendingOrbitActor;
      djinni->motionFlags = 0;
      Actor_SelectAnimationFar(djinni, 7);
      Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    }
  }

  wait(100);
  Sound_PlayFar(SE_STOP_EXIT_SE);
  wait(1);
  Sound_PlayFar(SE_LIGHT_APPEAR);
  position.x = leader->x.fixed;
  position.y = leader->y.fixed + toFIXED(18);
  position.z = leader->z.fixed;
  // 8 sparks from 18 pixels above the leader, each thrown 32 pixels in a
  // random direction at 1.4 to 2.4 pixels a frame and 5 up (gravity is
  // Actor.velocityY), falling, and released after 8 frames; stop at the
  // first that cannot be spawned.
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = Actor_Spawn(SPRITE_PARTICLE, position.x, position.y, position.z);
    if (djinni == NULL)
      break;
    djinni->scaleY = FIXED_ONE * 3 / 5;
    djinni->scaleX = FIXED_ONE * 3 / 5;
    djinni->motionFlags = ACTOR_FALLS;
    djinni->velocityY = toFIXED(5);
    djinni->groundHeight = djinni->y.fixed;
    djinni->speedLimit = Random16() + toFIXED(7) / 5;
    Actor_SetTargetPositionFromMagnitudeAngle(djinni, toFIXED(32), Random16());
    Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    djinni->wait = 8;
    Actor_StartScriptFar(djinni, gFieldEffect_ReleaseScript);
  }

  wait(15);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

/// Field effect sprite callback of a Mercury Djinni's droplets: each falls from
/// 100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
/// splashes 12 pixels in a random direction, at half size, with SE_CRACK. It
/// then falls again 6 frames later, until Djinn_RunMercuryJoinEffect sets its
/// state to MERCURY_DROPLET_LAST_FALL: its next splash is its last.
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect) {
  struct Actor *source;
  struct Vector3 position;
  s32 state;

  source = Actor_GetById(gMainRoutineState.leaderActorId);
  state = effect->state;

  if (state == MERCURY_DROPLET_AIM) {
    position.x = source->x.fixed;
    position.y = source->y.fixed + Random16() * 5 + toFIXED(15);
    position.z = source->z.fixed;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(Random16() * 6 + toFIXED(2), Random16(), &position);

    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->x = position.x;
    effect->z = position.z - toFIXED(100);
    effect->acceleration = toFIXED(3);
    effect->speedMax = Random16() * 3 + toFIXED(3);
    effect->scaleX = FIXED_ONE;
    effect->scaleY = FIXED_ONE;
    effect->turnStepLimited = state;
    effect->stopAtTarget = 1;
    effect->state++;
  } else if ((u8)(effect->state - MERCURY_DROPLET_FALLING) <=
             MERCURY_DROPLET_LAST_FALL - MERCURY_DROPLET_FALLING) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(toFIXED(12), Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = FIXED_ONE;
      effect->acceleration = 0;
      effect->speedMax = Random16() + toFIXED(11) / 5;
      effect->scaleX = FIXED_ONE / 2;
      effect->scaleY = FIXED_ONE / 2;
      Sound_PlayFar(SE_CRACK);

      if (effect->state == MERCURY_DROPLET_FALLING)
        effect->state--;
      else
        effect->state++;
      effect->callbackDelay = 6;
    }
  } else if (state == MERCURY_DROPLET_DONE) {
    FieldEffect_ClearOwnedSlot(effect);
  }
}
