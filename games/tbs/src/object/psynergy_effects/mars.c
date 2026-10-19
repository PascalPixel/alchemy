// Part of psynergy_effects.c, compiled only through it.
// A Mars Djinni joining the party.

#include "../psynergy_effects_internal.h"

// A Mars Djinni joins the party: it turns to the camera, bounces three times,
// spins ever faster and shrinks away in a fountain of 24 sparks
// (BattleEffect_UpdatePhasedRadialParticle). The leader turns to the camera
// and plays animation 28, the sparks swarm the leader, who pulses for 100
// frames, and the sparks vanish.
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
  Shop_RestoreSceneTilesFar(0x201090);
  WaitFrames(30);
  ObjectMotion_ArmCallback(djinniId, 0x4000, 0);
  WaitFrames(20);
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
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

  slot = effectScene->slots;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 284, position.x, position.z);
    EffectSlot_SetCallback(slot, BattleEffect_UpdatePhasedRadialParticle);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 10);
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

  // Every spark still in use stops its fountain (state 2). The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    WaitFrames(20);
    nextState = 2;
    slotState = &effectScene->slots[0].state;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  WaitFrames(60);
  leader->update = BattleFx_SetObjectAlternatingWords;
  WaitFrames(100);

  // Every spark still in use goes (state 5). This loop's cursor starts as
  // the scene plus 152, the first slot's state: written as above, the
  // cursor's two loads swap (differ 22).
  {
    s32 nextState;

    slotState = (s8 *)effectScene;
    nextState = 5;
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
  WaitFrames(30);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// Spins and shrinks an object by 0x400 a frame, then releases it.
DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroyFast, 0xFFFFFC00)

// Per-frame update of a Venus Djinni's falling motes: each circles the
// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
// pixel a frame, and is released 20 pixels above the leader's feet.
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
    ObjectDispatch_ReleaseFar(mote);
}

// Effect slot callback of a Venus Djinni's particles: each starts on screen
// at a random point 10 to 20 pixels round the leader and flies 120 pixels up
// the screen, over and over (states 0 and 1); set to 2, it is freed once it
// arrives.
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect) {
  struct EffectVector position;
  struct ObjectRuntime *source;
  s32 state;
  u32 random;

  source = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
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
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}
