/// @file
/// A Jupiter Djinni joining the party.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];

void Shop_RestoreSceneTilesFar(s32 value);
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
void Shop_InitializeEffectFar(void);

/// Ends a heavy impact: object id spins, leaps and vanishes in 24 sparks that
/// circle and then close on the current owner, which turns and takes mode 28.
void Djinn_RunJupiterJoinEffect(s32 id) {
  struct EffectVector position;
  struct MotionObject *object;
  struct BattleFxScene__motes *scene;
  struct FieldParticle *slot;
  s32 scale;
  s32 count;

  object = Actor_GetById(id);
  if (object == NULL)
    return;
  BattleEffect_InitializeSlots();
  scene = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(140);
  object->update = BattleEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(153);
  Actor_Jump(id, 12, 22);

  // 24 sparks from the object's place on screen, one a frame.
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Camera_WorldToScreen__motes(&position);
  Actor_Destroy(object);
  Sound_PlayFar(164);
  slot = scene->slots;
  count = 23;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = Random16() / 3 + Q16_ONE;
    slot->scaleY = scale;
    slot->scaleX = scale;
    count--;
    wait(1);
    slot++;
  } while (count >= 0);

  // The current owner turns to 0x4000 and takes mode 28.
  wait(60);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(40);
  Sound_PlayFar(164);
  wait(100);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

void BattleEffect_SetObjectAlternatingWords(u8 *object) {
  u32 *vblankCount = &gVBlankCount;
  u32 *table;
  u32 index;
  u32 value;

  table = gData_080f0bfc;
  index = (*vblankCount >> 2) & 1;
  value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

void BattleEffect_SpinUpAngle(void *obj) {
  u32 step;

  step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
  FIELD_AT_OFFSET(obj, u16 *, 6) =
      (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
  if (step < 0x1000U) {
    FIELD_AT_OFFSET(obj, s16 *, 0x64) =
        (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
  }
}

/// Spins an object 1/8 turn and shrinks it by 0x1c0 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj) {
  s32 step = 0xFFFFFE40;
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


struct BattleFxScene__motes;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

void Map_SetScreenShakeFar(s32, s32, s32);

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

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect) {
  struct EffectOwner *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct EffectOwner *)Actor_GetById(gMainRoutineWork.currentOwner);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset__motes(0x780000,
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
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(134);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
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
    position.x = owner->position.x;
    position.y = owner->position.y + 0x140000;
    position.z = owner->position.z;
    Camera_WorldToScreen__motes(&position);
    Vector_AddPolarOffset__motes(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(145);
  } else if (state == 4) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      BattleEffect_ClearOwnedSlot(effect);
  }
}

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);
#undef Actor_SetChildValueUnlessFifteenFar
#undef Actor_ReleaseFar
#undef gBattleEffect_PulseScales
#undef gEffectWork
