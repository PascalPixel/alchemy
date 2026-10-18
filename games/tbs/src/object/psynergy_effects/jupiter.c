// Part of psynergy_effects.c, compiled only through it.
// A Jupiter Djinni joining the party.

#include "../psynergy_effects_internal.h"

// A Jupiter Djinni joins the party: it bounces three times, spins, leaps out
// of sight and bursts into 24 light particles that circle and close in on
// the leader, who turns to the camera and plays animation 28.
void Djinn_RunJupiterJoinEffect(s32 djinniId) {
  struct EffectVector position;
  struct MotionObject *djinni;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 scale;
  s32 remainingParticles;

  djinni = (struct MotionObject *)Object_GetById(djinniId);
  if (djinni == NULL)
    return;
  BattleFx_InitializeSlots();
  scene = gEffectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);

  // Spin, then leap high.
  AudioCommand_PlayFar(SE_140);
  djinni->update = BattleFx_AdvanceSpinAngle;
  WaitFrames(40);
  AudioCommand_PlayFar(SE_153);
  ObjectMotion_Launch(djinniId, 12, 22);

  // 24 light particles from where the Djinni is on screen.
  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position);
  ObjectDispatch_ReleaseFar(djinni);
  AudioCommand_PlayFar(SE_164);
  slot = scene->slots;
  remainingParticles = 23;
  do {
    EffectSlot_Initialize(slot, 284, position.x, position.z);
    EffectSlot_SetCallback(slot, BattleFx_RunAngledApproachPhases);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(slot->object,
                                              (Random16() * 7) >> 16);
    scale = Random16() / 3 + 0x10000; // 1.0 to 1.33
    slot->scaleY = scale;
    slot->scaleX = scale;
    remainingParticles--;
    WaitFrames(1);
    slot++;
  } while (remainingParticles >= 0);

  // The leader turns to face south (0x4000) and plays animation 28.
  WaitFrames(60);
  ObjectMotion_ArmCallback(gGameState.selectedActor, 0x4000, 0);
  WaitFrames(20);
  ObjectDispatch_ApplyArgumentToChildrenFar(
      Object_GetById(gGameState.selectedActor), 28);
  WaitFrames(40);
  AudioCommand_PlayFar(SE_164);
  WaitFrames(100);
  Shop_InitEffectFar();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// A spinning object: its angle, and at 0x64 the frames its spin has been
// speeding up for (BattleFx_AdvanceAngleWithRamp), read signed.
struct RampedSpinObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5c];
  s16 rampFrames;
};

// Per-frame update: makes an object pulse, switching its scale between the
// two values of gBattleFx_PulseScales every 4 frames.
void BattleFx_SetObjectAlternatingWords(union FieldObject *fieldObject) {
  u8 *object = (u8 *)fieldObject;
  u32 *table = gBattleFx_PulseScales;
  u32 index = ((u32)gFrameTick >> 2) & 1;
  u32 scale = table[index];
  ((struct MotionObject *)object)->scaleX = scale;
  ((struct MotionObject *)object)->scaleY = scale;
}

// Per-frame update: spins an object ever faster, from 1/16 turn a frame up to
// 1/8.
void BattleFx_AdvanceAngleWithRamp(union FieldObject *fieldObject) {
  void *obj = (void *)fieldObject;
  struct RampedSpinObject *spin = obj;
  u32 step;

  step = spin->rampFrames * 0x50;
  spin->angle = spin->angle + step + 0x1000;
  if (step < 0x1000U) {
    spin->rampFrames = spin->rampFrames + 1;
  }
}

// Spins and shrinks an object by 0x1c0 a frame, then releases it.
DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroySlow, 0xFFFFFE40)

// Effect slot callback of a Mars Djinni's sparks. States 0 and 1 repeat: the
// spark flies 120 pixels up the screen (give or take 3/32 turn) from its
// origin and starts over. Set to 2, it stops where it arrives; 40 frames
// later, states 3 and 4 swarm it round a point 20 pixels over the leader.
// Set to 5, it is freed once it arrives.
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect) {
  struct Object *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct Object *)Object_GetById(gGameState.selectedActor);
  state = effect->state;

  if (state == 0) {
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

    ((struct AnimationObject *)effect->object)->image.priority =
        owner->sprite->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gFrameTick & 1) != 0)
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
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
  } else if (state == 4) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}
