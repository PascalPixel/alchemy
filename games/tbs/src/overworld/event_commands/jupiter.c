/// @file
/// A Jupiter Djinni joining the party.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// A Jupiter Djinni joins the party: it bounces three times, spins, leaps out
/// of sight and bursts into 24 light particles that circle and close in on the
/// leader, who turns to the camera and plays ANIM_LEADER_ARMS_RAISED. Called by
/// Djinn_JoinParty for a Jupiter Djinni, which then clears the Djinni's
/// actor slot; the Djinni's actor is released here. Uses all 24 field effect
/// sprites (FieldEffect_InitializeSlots), tints the field palette and fades it
/// back at the end.
void Djinn_RunJupiterJoinEffect(s32 djinniId) {
  struct Vector3 position;
  struct Actor *djinni;
  struct FieldEffectWork *effectWork;
  struct FieldParticle *effect;
  s32 scale;
  s32 remainingParticles;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;
  FieldEffect_InitializeSlots();
  effectWork = gFieldEffectWork;
  // Tint the screen yellow: PALETTE_TINT_CUSTOM | RGB(12, 12, 4).
  Shop_SaveAndTintFieldPaletteFar(0x20118c);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_MID);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(SE_SPIN);
  djinni->update = FieldEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 12, 22);

  // 24 light particles from where the Djinni is on screen.
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);
  Actor_ReleaseFar(djinni);
  Sound_PlayFar(SE_SCATTER);
  effect = effectWork->particles;
  remainingParticles = 23;
  // One a frame, each with a random sprite param of 0 to 6.
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, (Random16() * 7) >> 16);
    scale = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
    effect->scaleY = scale;
    effect->scaleX = scale;
    remainingParticles--;
    wait(1);
    effect++;
  } while (remainingParticles >= 0);

  wait(60);
  Actor_ArmCallback(gSequenceParentWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gSequenceParentWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(40);
  Sound_PlayFar(SE_SCATTER);
  wait(100);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Per-frame update: makes an actor pulse, switching its scale between the
/// two values of gFieldEffect_PulseScales every 4 frames. The Mars Djinni's
/// joining sets it on the leader, and clears it to end the pulse.
void FieldEffect_SetActorAlternatingWords(struct Actor *actor) {
  u8 *object = (u8 *)actor;
  u32 *table = gFieldEffect_PulseScales;
  u32 index = ((u32)gVBlankCount >> 2) & 1;
  u32 scale = table[index];
  ((struct Actor *)object)->scaleX = scale;
  ((struct Actor *)object)->scaleY = scale;
}

/// Per-frame update: spins an actor ever faster, from 1/16 turn a frame up to
/// just over 1/8 (0x2040), 0x50 more each frame for 52 frames. The caller
/// zeroes the frame count first (Actor.spinRampFrames), as the Mars Djinni's
/// joining does.
void FieldEffect_AdvanceAngleWithRamp(struct Actor *actor) {
  void *obj = (void *)actor;
  struct Actor *spin = obj;
  u32 step;

  step = spin->spinRampFrames * 0x50;
  spin->facing = spin->facing + step + 0x1000;
  if (step < 0x1000U) {
    spin->spinRampFrames = spin->spinRampFrames + 1;
  }
}

/// Spins and shrinks an actor by 0x1c0 a frame, then releases it: 119 frames
/// from full size. The Mars Djinni's joining shrinks the Djinni away with it.
DEFINE_SHRINK_AND_DESTROY(FieldEffect_ShrinkActorAndDestroySlow, 0xFFFFFE40)

/// Field effect sprite callback of a Mars Djinni's sparks (enum
/// MarsSparkState): the spark flies 120 pixels up the screen (give or take 3/32
/// turn) from its origin at up to 5 pixels a frame and starts over, until it
/// stops where it arrives, hidden; 40 frames later it swarms to points within 4
/// pixels of one 20 pixels over the leader, until it is freed once it arrives.
void FieldEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect) {
  struct Actor *owner;
  struct Vector3 position;
  s32 state;

  owner = Actor_GetById(gSequenceParentWork.leaderActorId);
  state = effect->state;

  if (state == MARS_SPARK_AIM) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(0x780000,
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + 0xc000,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x50000;
    effect->speedMax = 0x50000;
    effect->turnStepLimited = state;
    effect->state++;

    // At the leader's sprite priority, until MARS_SPARK_RISING brings it in
    // front
    // (priority 0, z lifting the sprite) after 3 frames.
    ((struct Sprite *)effect->actor)->image.priority =
        // Through the sprite union this load moves.
        (*(struct Sprite **)&owner->sprite)->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_HEAVY_IMPACT);
  } else if (state == MARS_SPARK_RISING) {
    if ((s16)effect->age == 3) {
      ((struct Sprite *)effect->actor)->image.priority = 0;
      effect->flags = 4;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_STOPPING) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct Sprite *)effect->actor)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == MARS_SPARK_AIM_LEADER) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->x.fixed;
    position.y = owner->y.fixed + 0x140000;
    position.z = owner->z.fixed;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_CRASH);
  } else if (state == MARS_SPARK_SWARMING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_LEAVING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}
