/// @file
/// A Mars Djinni joining the party.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// A Mars Djinni joins the party: it turns to the camera, bounces three times,
/// spins ever faster and shrinks away in a fountain of 24 sparks
/// (BattleEffect_UpdatePhasedRadialParticle). The leader turns to the camera
/// and plays animation 28, the sparks swarm the leader, who pulses for 100
/// frames, and the sparks vanish. Called by Djinn_JoinParty for a Mars Djinni,
/// which then clears the Djinni's actor slot. Uses all 24 effect slots, tints
/// the field palette red and fades it back at the end.
void Djinn_RunMarsJoinEffect(s32 djinniId) {
  struct EffectVector position;
  s32 scale;
  s32 remaining;
  struct FieldActor *djinni;
  struct BattleFxScene *effectScene;
  struct FieldActor *leader;
  struct EffectSlot *slot;
  s8 *slotState;

  djinni = Object_GetById(djinniId);
  leader = Object_GetById(gGameState.selectedActor);
  if (djinni == NULL)
    return;

  BattleFx_InitializeSlots();
  effectScene = gEffectWork;
  // Tint the screen red: PALETTE_TRANSFORM_TINT | RGB(16, 4, 4), the form
  // mercury.c spells out for its blue.
  Shop_SaveAndTintFieldPaletteFar(0x201090);
  WaitFrames(30);
  // Turn to face the camera (angle 0x4000), then bounce three times, a rising
  // note on each.
  ObjectMotion_ArmCallback(djinniId, 0x4000, 0);
  WaitFrames(20);
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  // Spin up for 80 frames, then spin and shrink away.
  WaitFrames(20);
  AudioCommand_PlayFar(SE_140);
  djinni->update = BattleFx_AdvanceAngleWithRamp;
  djinni->timer = 0;
  WaitFrames(80);
  djinni->update = BattleFx_ShrinkObjectAndDestroySlow;
  ObjectDispatch_SelectAnimationFar(djinni, 3);

  // The sparks start where the Djinni is on screen.
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  // One spark a frame into each of the 24 effect slots.
  slot = effectScene->slots;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, SPRITE_PARTICLE, position.x, position.z);
    EffectSlot_SetCallback(slot, BattleEffect_UpdatePhasedRadialParticle);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              SPRITE_COLOR_RED_TINT);
    scale = Random16() / 3 + 0x10000; // 1.0 to 1.33
    slot->scaleY = scale;
    slot->scaleX = scale;
    remaining--;
    WaitFrames(1);
    slot++;
  } while (remaining >= 0);

  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_SelectAnimationFar(Object_GetById(gGameState.selectedActor),
                                    28);

  // Every spark still in use stops its fountain. The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    WaitFrames(20);
    nextState = MARS_SPARK_STOPPING;
    slotState = &effectScene->slots[0].state;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  // The leader pulses while the sparks swarm it.
  WaitFrames(60);
  leader->update = BattleFx_SetObjectAlternatingWords;
  WaitFrames(100);

  // Every spark still in use goes. This loop's cursor starts as
  // the scene plus 152, the first slot's state: written as above, the
  // cursor's two loads swap (differ 22).
  {
    s32 nextState;

    slotState = (s8 *)effectScene;
    nextState = MARS_SPARK_LEAVING;
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

  // The leader stops pulsing. Storing NULL through the cleared slot cursor,
  // as the bytes do: a plain NULL store differs by 9.
  WaitFrames(10);
  slot = NULL;
  leader->update = (void *)slot;
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  // Fade the palette back from the tint and free the effect slots.
  WaitFrames(30);
  Shop_RestoreFieldPaletteFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

/// Spins and shrinks an object by 0x400 a frame, then releases it: 53 frames
/// from full size.
DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroyFast, 0xFFFFFC00)

/// Per-frame update of a Venus Djinni's falling motes: each circles the
/// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
/// pixel a frame, and is released 20 pixels above the leader's feet. Set by
/// BattleFx_RunVenusDjinnCapture in venus.c.
void BattleFx_UpdateDescendingOrbitObject(union FieldObject *fieldObject) {
  struct DescendingOrbitObject *mote =
      (struct DescendingOrbitObject *)fieldObject;
  struct EffectVector position;
  struct DescendingOrbitObject *leader;
  s32 timer;
  s16 frames;
  s32 y;

  leader =
      (struct DescendingOrbitObject *)Object_GetById(gGameState.selectedActor);
  timer = mote->timer - 1;
  mote->timer = timer;
  frames = mote->timer;
  position.x = leader->x;
  position.z = leader->z;
  Vector_AddPolarOffset(frames * 0x6666, (frames << 11) + mote->angle,
                        &position);
  mote->x = position.x;
  mote->z = position.z;
  y = mote->y + 0xFFFF0000;
  mote->y = y;
  if (y < leader->y + 0x140000)
    ObjectDispatch_ReleaseFar((struct DispatchObject *)mote);
}

/// Effect slot callback of a Venus Djinni's particles: each starts on screen
/// at a random point 10 to 20 pixels round the leader and flies 120 pixels up
/// the screen, over and over, until it is set to VENUS_PARTICLE_LAST_RISE and
/// freed once it arrives. Set by BattleFx_RunVenusDjinnCapture in venus.c; a
/// particle that starts on an odd frame plays SE_144. It speeds up by 1 pixel a
/// frame to at most 5, straight at its target.
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect) {
  struct EffectVector position;
  struct ObjectRuntime *source;
  s32 state;
  u32 random;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == VENUS_PARTICLE_AIM) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    effect->originX = position.x;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(0x780000, 0xc000, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x10000;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(SE_144);
  } else if (state == VENUS_PARTICLE_RISING) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == VENUS_PARTICLE_LAST_RISE) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}
