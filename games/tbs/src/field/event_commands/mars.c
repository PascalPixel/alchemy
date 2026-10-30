/// @file
/// A Mars Djinni joining the party.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// A Mars Djinni joins the party: it turns to the camera, bounces three times,
/// spins ever faster and shrinks away in a fountain of 24 sparks
/// (FieldEffect_UpdatePhasedRadialParticle). The leader turns to the camera and
/// plays ANIM_LEADER_ARMS_RAISED, the sparks swarm the leader, who pulses for
/// 100 frames, and the sparks vanish. Called by Djinn_JoinParty for a Mars
/// Djinni, which then clears the Djinni's actor slot. Uses all 24 field
/// effect sprites, tints the field palette red and fades it back at the end.
void Djinn_RunMarsJoinEffect(s32 djinniId) {
  struct Vector3 position;
  s32 scale;
  s32 remaining;
  struct Actor *djinni;
  struct FieldEffectWork *effectWork;
  struct Actor *leader;
  struct FieldParticle *effect;
  s8 *slotState;

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectWork = gFieldEffectWork;
  // Tint the screen red: PALETTE_TINT_CUSTOM | RGB(16, 4, 4), the form
  // mercury.c spells out for its blue.
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(16, 4, 4));
  wait(30);
  // Turn to face the camera (FACING_SOUTH), then bounce three times, a rising
  // note on each.
  Actor_ArmCallback(djinniId, FACING_SOUTH, 0);
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

  // One spark a frame into each of the 24 field effect sprites.
  effect = effectWork->particles;
  remaining = 23;
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdatePhasedRadialParticle);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, SPRITE_COLOR_RED_TINT);
    scale = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
    effect->scaleY = scale;
    effect->scaleX = scale;
    remaining--;
    wait(1);
    effect++;
  } while (remaining >= 0);

  wait(60);
  Actor_ArmCallback(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);

  // Every spark still in use stops its fountain. The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    wait(20);
    nextState = MARS_SPARK_STOPPING;
    slotState = &effectWork->particles[0].state;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  // The leader pulses while the sparks swarm it.
  wait(60);
  leader->update = FieldEffect_SetActorAlternatingWords;
  wait(100);

  // Every spark still in use goes. This loop's cursor starts as
  // the field effect plus 152, the first slot's state: written as above, the
  // cursor's two loads swap (differ 22).
  {
    s32 nextState;

    slotState = (s8 *)effectWork;
    nextState = MARS_SPARK_LEAVING;
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

  // The leader stops pulsing. Storing NULL through the cleared slot cursor,
  // as the bytes do: a plain NULL store differs by 9.
  wait(10);
  effect = NULL;
  leader->update = (void *)effect;
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  // Fade the palette back from the tint and free the field effect sprites.
  wait(30);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Spins and shrinks an actor by 1,024 a frame, then releases it: 53 frames
/// from full size.
DEFINE_SHRINK_AND_DESTROY(FieldEffect_ShrinkActorAndDestroyFast, -1024)

/// Per-frame update of a Venus Djinni's falling motes: each circles the
/// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
/// pixel a frame, and is released 20 pixels above the leader's feet. Set by
/// FieldEffect_RunVenusDjinnCapture in venus.c.
void FieldEffect_UpdateDescendingOrbitActor(struct Actor *actor) {
  struct Actor *mote = (struct Actor *)actor;
  struct Vector3 position;
  struct Actor *leader;
  s32 timer;
  s16 frames;
  s32 y;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  timer = mote->orbitFrames - 1;
  mote->orbitFrames = timer;
  frames = mote->orbitFrames;
  position.x = leader->x.fixed;
  position.z = leader->z.fixed;
  Vector_AddPolarOffset(frames * (Q16_ONE * 2 / 5),
                        (frames << 11) + mote->orbitAngle, &position);
  mote->x.fixed = position.x;
  mote->z.fixed = position.z;
  y = mote->y.fixed - PIXELS(1);
  mote->y.fixed = y;
  if (y < leader->y.fixed + PIXELS(20))
    Actor_ReleaseFar(mote);
}

/// Field effect sprite callback of a Venus Djinni's particles: each starts on
/// screen at a random point 10 to 20 pixels round the leader and flies 120
/// pixels up the screen, over and over, until it is set to
/// VENUS_PARTICLE_LAST_RISE and freed once it arrives. Set by
/// FieldEffect_RunVenusDjinnCapture in venus.c; a particle that starts on an
/// odd frame plays SE_CLATTER. It speeds up by 1 pixel a frame to at most 5,
/// straight at its target.
void FieldEffect_UpdateRadialSpread(struct FieldParticle *effect) {
  struct Vector3 position;
  struct Actor *source;
  s32 state;
  u32 random;

  source = Actor_GetById(gMainRoutineWork.leaderActorId);
  state = effect->state;

  if (state == VENUS_PARTICLE_AIM) {
    position.x = source->x.fixed;
    position.y = source->y.fixed;
    position.z = source->z.fixed;

    random = Random16() * 10 + PIXELS(10);
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    effect->originX = position.x;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(PIXELS(120), FACING_NORTH, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = Q16_ONE;
    effect->speedMax = PIXELS(5);
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
