/// @file
/// Routines whose twins are in The Broken Seal's field/event_commands/mars.c.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

struct DjinniObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[68];
  u16 timer;
  u8 unk66[6];
  void *update;
};

struct Actor *Actor_GetById(u32 objectId);
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void Sound_PlayFar(s32 cue);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void wait(s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
/* VIEW(mars_join): routine Camera_WorldToScreen as void Camera_WorldToScreen(struct EffectVector *position); ground_particles has void Camera_WorldToScreen(union FieldCoordinate *position). */
extern void Camera_WorldToScreen__mars_join(struct EffectVector *position) __asm__("Camera_WorldToScreen");
/* VIEW(mars_join): routine FieldParticle_Initialize as void FieldParticle_Initialize(void *slot, s32 kind, s32 x, s32 z); motes has void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x, s32 z). */
extern void FieldParticle_Initialize__mars_join(void *slot, s32 kind, s32 x, s32 z) __asm__("FieldParticle_Initialize");
/* VIEW(mars_join): routine Animation_ApplyChildValuesToRecordFar as void Animation_ApplyChildValuesToRecordFar(s32 object, s32 value); motes has void Animation_ApplyChildValuesToRecordFar(void *object, u32 value). */
extern void Animation_ApplyChildValuesToRecordFar__mars_join(s32 object, s32 value) __asm__("Animation_ApplyChildValuesToRecordFar");
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x, s32 z);
u32 Random16(void);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);
/* VIEW(mars_join): routine Actor_Destroy as void Actor_Destroy(void *object); battle.h has void Actor_Destroy(struct Actor *object). */
extern void Actor_Destroy__mars_join(void *object) __asm__("Actor_Destroy");
void BattleEffect_SetObjectAlternatingWords(u8 *object);
void BattleEffect_SpinUpAngle(void *obj);
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj);
void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect);

/// A Mars Djinni joins the party: it turns to the camera, bounces three times,
/// spins ever faster and shrinks away in a fountain of 24 sparks
/// (BattleEffect_UpdatePhasedRadialParticle). The leader turns to the camera
/// and plays animation 28, the sparks swarm the leader, who pulses for 100
/// frames, and the sparks vanish.
void Djinn_RunMarsJoinEffect(s32 djinniId) {
  s8 *objectPointer;
  struct EffectVector position;
  s32 value;
  s32 remaining;
  struct DjinniObject *djinni;
  void *effectSlots;
  struct DjinniObject *leader;
  void *slot;
  s8 *slotState;

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x201090);
  wait(30);
  Actor_ArmCallback(djinniId, 0x4000, 0);
  wait(20);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(20);
  Sound_PlayFar(140);
  djinni->update = BattleEffect_SpinUpAngle;
  djinni->timer = 0;
  wait(80);
  djinni->update = BattleEffect_ShrinkObjectAndDestroySlow;
  Actor_SetMode(djinni, 3);

  // The sparks start where the Djinni is on screen.
  position.x = djinni->x;
  {
    void *p = &position;
    s32 y;

    y = djinni->y;
    remaining = 23;
    position.y = y;
    position.z = djinni->z;
    Camera_WorldToScreen__mars_join(p);
  }

  slot = (u8 *)effectSlots + 80;
  do {
    FieldParticle_Initialize__mars_join(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, BattleEffect_UpdatePhasedRadialParticle);
    FieldParticle_SetActorMode(slot, 7);
    objectPointer = slot;
    value = *(s32 *)objectPointer;
    Animation_ApplyChildValuesToRecordFar__mars_join(value, 10);
    value = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
    ((struct FieldParticle *)slot)->scaleY = value;
    ((struct FieldParticle *)slot)->scaleX = value;
    remaining--;
    wait(1);
    slot = (u8 *)slot + sizeof(struct FieldParticle);
  } while (remaining >= 0);

  wait(60);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);

  // Every spark still in use stops its fountain (state 2). The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    wait(20);
    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  wait(60);
  leader->update = BattleEffect_SetObjectAlternatingWords;
  wait(100);

  // Every spark still in use goes (state 5).
  {
    s32 nextState;

    slotState = effectSlots;
    nextState = 5;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  wait(10);
  slot = NULL;
  leader->update = slot;
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  wait(30);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Spins an object 1/8 turn and shrinks it by 0x400 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroyFast(void *obj) {
  s32 step = 0xFFFFFC00;
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + 0x2000;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Actor_Destroy__mars_join(obj);
}


struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar__mars_join)
#define Actor_ReleaseFar Actor_Destroy__mars_join
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];


void BattleEffect_ClearOwnedSlot(struct FieldParticle *effect);
/* VIEW(mars_join): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position); f0d489c has void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position). */
extern void Vector_AddPolarOffset__mars_join(s32 magnitude, s32 angle,
                           struct EffectVector *position) __asm__("Vector_AddPolarOffset");
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void BattleEffect_AdvanceSpinAngle();
void Map_SetScreenShakeFar(s32, s32, s32);
void Motion_SetTargetPositionFromMagnitudeAngle();




extern const s32 gBattleEffect_SharedParticleScript[];


void Actor_Jump(s32 id, s32 height, s32 frames);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
/* VIEW(mars_join): routine Animation_ApplyChildValuesFar as void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value); motes has void Animation_ApplyChildValuesFar(struct Actor *object, u32 value). */
extern void Animation_ApplyChildValuesFar__mars_join(struct CaptureObject *object, s32 value) __asm__("Animation_ApplyChildValuesFar");


/// Per-frame update of a Venus Djinni's falling motes: each circles the
/// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
/// pixel a frame, and is released 20 pixels above the leader's feet. Set by
/// Func_080d82e0, the Venus Djinni's effect.
void FieldEffect_UpdateDescendingOrbitActor(
    struct DescendingOrbitObject *actor) {
  struct OrbitPosition local;
  struct DescendingOrbitObject *other;
  s32 raw;
  s16 value;
  s32 y;

  other = (struct DescendingOrbitObject *)Actor_GetById(
      gMainRoutineWork.currentOwner);
  raw = actor->timer - 1;
  actor->timer = raw;
  value = actor->timer;
  local.x = other->x;
  local.z = other->z;
  Vector_AddPolarOffset__mars_join(value * 0x6666, (value << 11) + actor->angle, &local);
  actor->x = local.x;
  actor->z = local.z;
  y = actor->y + 0xFFFF0000;
  actor->y = y;
  if (y < other->y + 0x140000)
    Actor_ReleaseFar(actor);
}

void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);

/// Places a spark at random around the current owner and sends it 120 pixels
/// along angle 0xc000. Each time FieldParticle_HasPendingTarget returns 0,
/// state 1 starts it over and state 2 clears its slot.
void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect) {
  struct EffectVector position;
  struct Actor *source;
  s32 state;
  u32 random;
  s32 x;
  s32 magnitude;
  s32 angle;

  source = Actor_GetById(gMainRoutineWork.currentOwner);
  state = effect->state;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset__mars_join(random, Random16(), &position);
    Camera_WorldToScreen__mars_join(&position);

    x = position.x;
    magnitude = 240;
    effect->originX = x;
    angle = 192;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset__mars_join(magnitude << 15, angle << 8, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = Q16_ONE;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(0x90);
  } else if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      BattleEffect_ClearOwnedSlot(effect);
  }
}
