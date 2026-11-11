#include "types.h"
#include "system.h"
#include "sprite.h"
#include "motobj.h"
#include "actor.h"
#include "objdisp.h"
#include "main_routine_work.h"
#include "field_particle.h"
#include "ram_buffer.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                              \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];
void Animation_ApplyChildValuesToRecordFar();
void Actor_Destroy();

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

struct Actor *Actor_GetById(u32 actor);
void BattleEffect_ClearOwnedSlot(struct FieldParticle *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);
void Sound_PlayFar(s32 cue);
void Motion_SetVarCbAndRefresh();
void Camera_WorldToScreen();
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32);
void Shop_InitializeEffectFar(void);
void FieldParticle_Initialize();
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

void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_SetMode(struct Actor *object, s32 mode);
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
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

void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect);

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

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

  other =
      (struct DescendingOrbitObject *)Actor_GetById(gMainRoutineWork.currentOwner);
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
