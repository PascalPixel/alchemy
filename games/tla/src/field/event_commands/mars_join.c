#include "types.h"
#include "field_particle.h"
#include "motobj.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "fixed_math.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

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
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void wait(s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
#include "actor.h"
void Camera_WorldToScreen(struct EffectVector *position);
void FieldParticle_Initialize(void *slot, s32 kind, s32 x, s32 z);
void Animation_ApplyChildValuesToRecordFar(s32 object, s32 value);
u32 Random16(void);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);
void Actor_Destroy(void *object);
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
    Camera_WorldToScreen(p);
  }

  slot = (u8 *)effectSlots + 80;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, BattleEffect_UpdatePhasedRadialParticle);
    FieldParticle_SetActorMode(slot, 7);
    objectPointer = slot;
    value = *(s32 *)objectPointer;
    Animation_ApplyChildValuesToRecordFar(value, 10);
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
    Actor_Destroy(obj);
}

#include "system.h"
#include "sprite.h"
#include "objdisp.h"

struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

void BattleEffect_ClearOwnedSlot(struct FieldParticle *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);
void BattleEffect_AdvanceSpinAngle();
void Map_SetScreenShakeFar(s32, s32, s32);
void Motion_SetTargetPositionFromMagnitudeAngle();

struct OrbitPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

struct SpreadPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gBattleEffect_SharedParticleScript[];

struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  u8 unk68[4];
  void *callback;
};

void Actor_Jump(s32 id, s32 height, s32 frames);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

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
  Vector_AddPolarOffset(value * 0x6666, (value << 11) + actor->angle, &local);
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
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    x = position.x;
    magnitude = 240;
    effect->originX = x;
    angle = 192;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(magnitude << 15, angle << 8, &position);
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

struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

void Func_080d82e0(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Actor_GetById(gMainRoutineWork.currentOwner);
  djinni = (struct CaptureObject *)Actor_GetById(arg);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(152);
  Actor_Jump(arg, 4, 15);
  Sound_PlayFar(152);
  Actor_Jump(arg, 4, 15);
  wait(30);
  djinni->callback = (void *)BattleEffect_AdvanceSpinAngle;
  Sound_PlayFar(153);
  Actor_Jump(arg, 8, 22);
  Sound_PlayFar(140);
  Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)BattleEffect_ShrinkObjectAndDestroyFast;
  Actor_SetMode((struct Actor *)djinni, 3);
  wait(90);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(30);
  Map_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen(position);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, 672, position[0], position[2]);
    FieldParticle_SetCallback(slot, (void *)BattleEffect_UpdateRadialSpread);
    FieldParticle_SetActorMode(slot, 7);
    Actor_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
    ((struct FieldParticle *)slot)->scaleX = 0x8000;
    ((struct FieldParticle *)slot)->scaleY = Random16() + 0x18000;
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(140);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  wait(20);
  Map_SetScreenShakeFar(1, 1, 1);
  wait(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)FieldEffect_UpdateDescendingOrbitActor;
      djinni->mode = 0;
      Actor_SetMode((struct Actor *)djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  wait(100);
  Sound_PlayFar(298);
  wait(1);
  Sound_PlayFar(151);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = 0x50000;
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
    Animation_ApplyChildValuesFar(djinni, 11);
    djinni->lifetime = 8;
    Actor_StartScriptFar((struct ActorView *)djinni,
                         gBattleEffect_SharedParticleScript);
  }

  wait(15);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

/// Field effect sprite callback of a Mercury Djinni's droplets: each falls from
/// 100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
/// splashes 12 pixels in a random direction, at half size, with sound 143. It
/// then falls again 6 frames later, until BattleEffect_RunMercuryDjinnCapture
/// sets its state to 2: its next splash is its last.
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect) {
  struct Actor *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = Actor_GetById(gMainRoutineWork.currentOwner);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    {
      struct EffectVector *p = &position;

      position.z = source->z;
      Camera_WorldToScreen(p);
    }
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    {
      s32 accel = 0x30000;

      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->x = position.x;
      effect->z = position.z - 0x640000;
      effect->acceleration = accel;
    }
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = Q16_ONE;
    effect->scaleY = Q16_ONE;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = Q16_ONE;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      Sound_PlayFar(143);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    BattleEffect_ClearOwnedSlot(effect);
  }
}

void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object) {
  struct Actor *target = object->watchedObject;

  object->x += (target->x - object->x) / 2;
  object->y += (target->y - object->y) / 2;
  object->z += (target->z - object->z) / 2;
}
