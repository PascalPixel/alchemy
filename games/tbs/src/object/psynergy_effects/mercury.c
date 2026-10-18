// Part of psynergy_effects.c, compiled only through it.
// A Mercury Djinni joining the party.

#include "../psynergy_effects_internal.h"

#define MERCURY_PARTICLE_MAX 24  // droplets (one an effect slot)
#define MERCURY_AFTERIMAGE_MAX 8 // afterimages of the rising Djinni
// A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
// bounces twice and hops three times ever higher, then rises 120 pixels out
// of sight trailing eight afterimages. The leader turns to the camera and
// plays animation 28 while 24 droplets rain round it, one a frame
// (BattleFx_UpdateRandomTargetParticle); then the colours return.
void BattleFx_RunMercuryDjinnCapture(s32 djinniId) {
  struct FieldActor *djinni;
  struct FieldActor *copy;
  struct FieldActor *previous;
  struct AnimationObject *resource;
  struct BattleFxScene *effectScene;
  struct FieldActor **cursor;
  struct FieldActor **list;
  struct EffectSlot *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct FieldActor *copies[MERCURY_AFTERIMAGE_MAX];
  struct EffectVector position;

  djinni = Object_GetById(djinniId);
  if (djinni == NULL)
    return;

  // Tint the screen blue (red 4, green 4, blue 16).
  BattleFx_InitializeSlots();
  effectScene = gEffectWork;
  Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | RGB(4, 4, 16));
  WaitFrames(30);
  djinni->paused = 0;
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);

  // Three hops, at 3, 5 and 7 pixels a frame.
  AudioCommand_PlayFar(SE_152);
  ObjectMotion_Launch(djinniId, 3, 14);
  AudioCommand_PlayFar(SE_152);
  ObjectMotion_Launch(djinniId, 5, 16);
  AudioCommand_PlayFar(SE_152);
  ObjectMotion_Launch(djinniId, 7, 18);
  WaitFrames(20);

  // Eight afterimages of the same sprite, each closing half the distance to
  // the one before it every frame.
  kind = djinni->sprite->entries[0]->animId;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = MERCURY_AFTERIMAGE_MAX - 1; remaining >= 0; remaining--) {
    copy = (struct FieldActor *)Object_CreateFar(
        kind, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = BattleFx_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Animation_ApplyChildValuesFar(copy, 9);
      ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
      resource = Sprite_ShareVramCache(copy->sprite, resource);
      copy->linkedObject = previous;
      previous = copy;
    }
  }

  // Rise 8 pixels a frame for 15 frames, then remove the Djinni and its
  // afterimages (and their VRAM block, unless it is the shared block 96).
  resourceId = resource->slot;
  AudioCommand_PlayFar(SE_153);
  djinni->motionFlags = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y.fixed += 8 << 16;
    WaitFrames(1);
  }
  ObjectDispatch_ReleaseFar(djinni);
  list = copies;
  for (remaining = MERCURY_AFTERIMAGE_MAX - 1; remaining >= 0; remaining--)
    ObjectDispatch_ReleaseFar(*list++);
  if (resourceId != 96)
    Resource_ResetEntry(resourceId);
  WaitFrames(10);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(20);

  // The droplets (object 240), one a frame, from where the Djinni was on
  // screen (BattleFx_UpdateRandomTargetParticle moves them).
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  slot = effectScene->slots;
  remaining = MERCURY_PARTICLE_MAX - 1;
  do {
    EffectSlot_Initialize(slot, 240, position.x, position.z);
    EffectSlot_SetCallback(slot, BattleFx_UpdateRandomTargetParticle);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
    remaining--;
    WaitFrames(1);
    slot++;
  } while (remaining >= 0);

  // After 120 frames every droplet still in use goes on to state 2.
  // Walking whole slots instead of their state bytes grows native English
  // from 0x23c to 0x240 bytes.
  WaitFrames(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = (s8 *)effectScene;
    slotState += offsetof(struct BattleFxScene, slots) +
                 offsetof(struct EffectSlot, state);
    remaining = MERCURY_PARTICLE_MAX - 1;
    do {
      if (slotState[5] != 0) // active
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct EffectSlot);
    } while (remaining >= 0);
  }

  // Restore the colours.
  WaitFrames(50);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

#undef MERCURY_PARTICLE_MAX
#undef MERCURY_AFTERIMAGE_MAX
