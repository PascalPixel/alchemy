/// @file
/// A Mars Djinni joining the party, and the first of the Venus Djinni's
/// routines, linked after it: its shrinking, its falling motes and its rising
/// particles.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// A Mars Djinni joins the party: it turns to the camera, bounces three times,
/// spins ever faster and shrinks away in a fountain of 24 sparks
/// (FieldEffect_UpdateMarsSpark). The leader turns to the camera and raises
/// its arms, the sparks swarm it while it pulses for 100 frames, and the
/// sparks vanish.
void Djinn_RunMarsJoinEffect(s32 djinniId) {
  struct Vector3 position;
  FIXED scale;
  s32 remaining;
  struct Actor *djinni;
  struct FieldEffectState *effectState;
  struct Actor *leader;
  struct FieldParticle *effect;
  s8 *slotState;

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectState = gFieldEffectState;
  // Tint the screen red.
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(16, 4, 4));
  wait(30);
  // Turn to face the camera (FACING_SOUTH), then bounce three times, a rising
  // note on each.
  Actor_FaceDirectionById(djinniId, FACING_SOUTH, 0);
  wait(20);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_MID);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  // Spin up for 80 frames, then spin and shrink away.
  wait(20);
  Sound_PlayFar(SE_SPIN);
  djinni->update = FieldEffect_AdvanceAngleWithRamp;
  djinni->timer = 0;
  wait(80);
  djinni->update = FieldEffect_ShrinkActorAndDestroySlow;
  Actor_SelectAnimationFar(djinni, 3);

  // The sparks start where the Djinni is on screen.
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  // One spark a frame into each of the field effect sprites.
  effect = effectState->particles;
  remaining = PSYNERGY_EFFECT_SLOT_COUNT - 1;
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateMarsSpark);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, SPRITE_COLOR_RED_TINT);
    scale = Random16() / 3 + FIXED_ONE; // 1.0 to 1.33
    effect->scaleY = scale;
    effect->scaleX = scale;
    remaining--;
    wait(1);
    effect++;
  } while (remaining >= 0);

  wait(60);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineState.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);

  // Every spark still in use stops its fountain.
  wait(20);
  FIELD_PARTICLES_SET_ACTIVE_STATE(slotState, remaining, effectState, particles,
                                   MARS_SPARK_STOPPING);

  // The leader pulses while the sparks swarm it.
  wait(60);
  leader->update = FieldEffect_PulseActorScale;
  wait(100);

  // Every spark still in use goes.
  FIELD_PARTICLES_SET_ACTIVE_STATE(slotState, remaining, effectState, particles,
                                   MARS_SPARK_LEAVING);

  // The leader stops pulsing.
  wait(10);
  /* FAKEMATCH(temporary): effect, done with the sparks, stands for NULL. */
  effect = NULL;
  leader->update = (void *)effect;
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  // Fade the palette back from the tint and free the field effect sprites.
  wait(30);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

/// Spins an actor an eighth of a turn and shrinks it by 1/64 of its size a
/// frame, then releases it: 53 frames from full size. The Venus Djinni's
/// joining shrinks the Djinni away with it.
DEFINE_SHRINK_AND_DESTROY(FieldEffect_ShrinkActorAndDestroyFast,
                          -(FIXED_ONE / 64))

/// Per-frame update of a Venus Djinni's falling motes: each circles the leader
/// a 32nd of a turn a frame on a shrinking spiral (0.4 pixels of radius for
/// each frame left) while it sinks a pixel a frame, and is released 20 pixels
/// above the leader's feet.
void FieldEffect_UpdateDescendingOrbitActor(struct Actor *mote) {
  struct Vector3 position;
  struct Actor *leader;
  s32 timer;
  s16 frames;
  FIXED y;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  timer = mote->orbitFrames - 1;
  mote->orbitFrames = timer;
  frames = mote->orbitFrames;
  position.x = leader->x.fixed;
  position.z = leader->z.fixed;
  Vector_AddPolarOffset(frames * (toFIXED(2) / 5),
                        frames * (TRIG_TURN / 32) + mote->orbitAngle,
                        &position);
  mote->x.fixed = position.x;
  mote->z.fixed = position.z;
  y = mote->y.fixed - toFIXED(1);
  mote->y.fixed = y;
  if (y < leader->y.fixed + toFIXED(20))
    Actor_ReleaseFar(mote);
}

/// Field effect sprite callback of a Venus Djinni's particles: each starts on
/// screen at a random point 10 to 20 pixels round the leader and flies 120
/// pixels up the screen, over and over, until it is set to
/// VENUS_PARTICLE_LAST_RISE and freed once it arrives.
void FieldEffect_UpdateRadialSpread(struct FieldParticle *effect) {
  struct Vector3 position;
  struct Actor *leader;
  s32 state;
  u32 random;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  state = effect->state;

  if (state == VENUS_PARTICLE_AIM) {
    position.x = leader->x.fixed;
    position.y = leader->y.fixed;
    position.z = leader->z.fixed;

    random = Random16() * 10 + toFIXED(10);
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    effect->originX = position.x;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(toFIXED(120), FACING_NORTH, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = FIXED_ONE;
    effect->speedMax = toFIXED(5);
    effect->turnStepLimited = state;
    effect->state++;

    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_CLATTER);
  } else if (state == VENUS_PARTICLE_RISING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == VENUS_PARTICLE_LAST_RISE) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}
