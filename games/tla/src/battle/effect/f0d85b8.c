#include "types.h"
#include "system.h"
#include "animspr.h"
#include "motobj.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "adventure.h"
#include "effect_slot.h"
#include "ram_buffer.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

struct BattleFxScene;

#define ObjectGroup_SetChildValueUnlessFifteenFar                              \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define ObjectDispatch_ReleaseFar Object_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];
void Animation_ApplyChildValuesToRecordFar();
void Object_Destroy();

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

struct ObjectRuntime *Object_GetById(u32 actor);
void BattleEffect_ClearOwnedSlot(struct EffectSlot *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);
void Sound_PlayFar(s32 cue);
void Motion_SetVarCbAndRefresh();
void Camera_WorldToScreen();
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32);
void Shop_InitializeEffectFar(void);
void EffectSlot_Initialize();
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

void Sound_PlayFar(s32);

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

void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Object_SetMode(struct ObjectRuntime *object, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

void BattleEffect_SetObjectAlternatingWords(u8 *object);

void BattleEffect_SpinUpAngle(void *obj);

void BattleEffect_ShrinkObjectAndDestroySlow(void *obj);

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

void BattleEffect_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);

void BattleEffect_UpdateRadialSpread(struct EffectSlot *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void Func_080d85b8(struct EffectSlot *effect) {
  struct ObjectRuntime *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = Object_GetById(gAdventure.currentOwner);
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
    effect->scaleX = 0x10000;
    effect->scaleY = 0x10000;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = 0x10000;
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
