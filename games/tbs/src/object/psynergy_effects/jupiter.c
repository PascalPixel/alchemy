/// @file
/// A Jupiter Djinni joining the party.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// A Jupiter Djinni joins the party: it bounces three times, spins, leaps out
/// of sight and bursts into 24 light particles that circle and close in on
/// the leader, who turns to the camera and plays animation 28. Called by
/// Djinn_JoinParty for a Jupiter Djinni, which then clears the Djinni's actor
/// slot; the Djinni's object is released here. Uses all 24 effect slots
/// (FieldEffect_InitializeSlots), tints the field palette and fades it back at
/// the end.
void Djinn_RunJupiterJoinEffect(s32 djinniId) {
  struct EffectVector position;
  struct MotionObject *djinni;
  struct FieldFxScene *scene;
  struct EffectSlot *effect;
  s32 scale;
  s32 remainingParticles;

  djinni = (struct MotionObject *)Object_GetById(djinniId);
  if (djinni == NULL)
    return;
  FieldEffect_InitializeSlots();
  scene = gEffectWork;
  // Tint the screen yellow: PALETTE_TRANSFORM_TINT | RGB(12, 12, 4).
  Shop_SaveAndTintFieldPaletteFar(0x20118c);
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);

  // Spin, then leap high.
  AudioCommand_PlayFar(SE_140);
  djinni->update = FieldEffect_AdvanceSpinAngle;
  WaitFrames(40);
  AudioCommand_PlayFar(SE_HIGH_JUMP);
  ObjectMotion_Launch(djinniId, 12, 22);

  // 24 light particles from where the Djinni is on screen.
  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position);
  ObjectDispatch_ReleaseFar((struct DispatchObject *)djinni);
  AudioCommand_PlayFar(SE_164);
  effect = scene->effects;
  remainingParticles = 23;
  // One a frame, each with a random sprite param of 0 to 6.
  do {
    EffectSlot_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    EffectSlot_SetCallback(effect, FieldEffect_RunAngledApproachPhases);
    EffectSlot_SetObjectMode(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              (Random16() * 7) >> 16);
    scale = Random16() / 3 + 0x10000; // 1.0 to 1.33
    effect->scaleY = scale;
    effect->scaleX = scale;
    remainingParticles--;
    WaitFrames(1);
    effect++;
  } while (remainingParticles >= 0);

  // The leader turns to face south (0x4000) and plays animation 28.
  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_SelectAnimationFar(Object_GetById(gGameState.selectedActor),
                                    28);
  WaitFrames(40);
  AudioCommand_PlayFar(SE_164);
  WaitFrames(100);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// A spinning object: its angle, and at 0x64 the frames its spin has been
/// speeding up for (FieldEffect_AdvanceAngleWithRamp), read signed.
struct RampedSpinObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5c];
  s16 rampFrames;
};

/// Per-frame update: makes an object pulse, switching its scale between the
/// two values of gFieldEffect_PulseScales every 4 frames. The Mars Djinni's
/// joining sets it on the leader, and clears it to end the pulse.
void FieldEffect_SetObjectAlternatingWords(union FieldObject *fieldObject) {
  u8 *object = (u8 *)fieldObject;
  u32 *table = gFieldEffect_PulseScales;
  u32 index = ((u32)gFrameTick >> 2) & 1;
  u32 scale = table[index];
  ((struct MotionObject *)object)->scaleX = scale;
  ((struct MotionObject *)object)->scaleY = scale;
}

/// Per-frame update: spins an object ever faster, from 1/16 turn a frame up to
/// just over 1/8 (0x2040), 0x50 more each frame for 52 frames. The caller
/// zeroes the frame count first (FieldActor.timer), as the Mars Djinni's
/// joining does.
void FieldEffect_AdvanceAngleWithRamp(union FieldObject *fieldObject) {
  void *obj = (void *)fieldObject;
  struct RampedSpinObject *spin = obj;
  u32 step;

  step = spin->rampFrames * 0x50;
  spin->angle = spin->angle + step + 0x1000;
  if (step < 0x1000U) {
    spin->rampFrames = spin->rampFrames + 1;
  }
}

/// Spins and shrinks an object by 0x1c0 a frame, then releases it: 119 frames
/// from full size. The Mars Djinni's joining shrinks the Djinni away with it.
DEFINE_SHRINK_AND_DESTROY(FieldEffect_ShrinkObjectAndDestroySlow, 0xFFFFFE40)

/// Effect slot callback of a Mars Djinni's sparks (enum MarsSparkState): the
/// spark flies 120 pixels up the screen (give or take 3/32 turn) from its
/// origin at up to 5 pixels a frame and starts over, until it stops where it
/// arrives, hidden; 40 frames later it swarms to points within 4 pixels of
/// one 20 pixels over the leader, until it is freed once it arrives.
void FieldEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect) {
  struct Object *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct Object *)Object_GetById(gGameState.selectedActor);
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
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    // At the leader's sprite priority, until MARS_SPARK_RISING brings it in
    // front
    // (priority 0, z lifting the sprite) after 3 frames.
    ((struct AnimationObject *)effect->object)->image.priority =
        owner->sprite->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
  } else if (state == MARS_SPARK_RISING) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_STOPPING) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == MARS_SPARK_AIM_LEADER) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->pos.x;
    position.y = owner->pos.y + 0x140000;
    position.z = owner->pos.z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(SE_145);
  } else if (state == MARS_SPARK_SWARMING) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_LEAVING) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}
