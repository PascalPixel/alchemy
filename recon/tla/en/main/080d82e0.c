// Near miss, differ 12 of 292: ported from ☀️ (BattleEffect_RunVenusDjinnCapture),
// with ⚓️'s data: the effect scene keeps its slots at 0x50, the sparks and
// the spawned objects are kind 672 and the cue is 298. What remains is order:
// the native builds the slot pointer and the spark count between the
// position's loads, starts 240 << 15 before the second y load, and clears r7
// earlier; barriers on the position's address and the count, as in
// Djinn_RunMarsJoinEffect, measured 36 to 38.
#include "types.h"
#include "system.h"
#include "animspr.h"
#include "motobj.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "party_state.h"
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

extern u32 gFrameTick;
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
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh();
void Camera_WorldToScreen();
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32);
void Shop_InitializeEffectFar(void);
void EffectSlot_Initialize();
void BattleEffect_AdvanceSpinAngle();
void Map_SetScreenShakeFar(s32, s32, s32);
void Motion_SetTargetPositionFromMagnitudeAngle();

struct PhasedRadialSequenceObject {
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
  void *callback;
};

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

void Audio_PlayCue(s32);

extern const s32 gBattleEffect_CommonParticleScript[];

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

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg);

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

void BattleEffect_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);

void BattleEffect_UpdateRadialSpread(struct EffectSlot *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Object_GetById(gPartyState.currentOwner);
  djinni = (struct CaptureObject *)Object_GetById(arg);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  WaitFrames(30);
  djinni->visible = 0;
  Audio_PlayCue(152);
  ObjectMotion_Launch(arg, 4, 15);
  Audio_PlayCue(152);
  ObjectMotion_Launch(arg, 4, 15);
  WaitFrames(30);
  djinni->callback = (void *)BattleEffect_AdvanceSpinAngle;
  Audio_PlayCue(153);
  ObjectMotion_Launch(arg, 8, 22);
  Audio_PlayCue(140);
  Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)BattleEffect_ShrinkObjectAndDestroyFast;
  Object_SetMode((struct ObjectRuntime *)djinni, 3);
  WaitFrames(90);
  ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(Object_GetById(gPartyState.currentOwner), 28);
  WaitFrames(30);
  Map_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen(position);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    EffectSlot_Initialize(slot, 672, position[0], position[2]);
    EffectSlot_SetCallback(slot, (void *)BattleEffect_UpdateRadialSpread);
    EffectSlot_SetObjectMode(slot, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
    ((struct EffectSlot *)slot)->scaleX = 0x8000;
    ((struct EffectSlot *)slot)->scaleY = Random16() + 0x18000;
    remaining--;
    WaitFrames(1);
    slot += 72;
  } while (remaining >= 0);

  WaitFrames(140);
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

  WaitFrames(20);
  Map_SetScreenShakeFar(1, 1, 1);
  WaitFrames(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Object_Spawn(672, position[0], position[1],
                                                  position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)BattleEffect_UpdateDescendingOrbitObject;
      djinni->mode = 0;
      Object_SetMode((struct ObjectRuntime *)djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  WaitFrames(100);
  Audio_PlayCue(298);
  WaitFrames(1);
  Audio_PlayCue(151);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Object_Spawn(672, position[0], position[1],
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
    ObjectDispatch_InitializeFar((struct DispatchObject *)djinni,
                                 gBattleEffect_CommonParticleScript);
  }

  WaitFrames(15);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

void BattleEffect_UpdateRandomTargetParticle(struct EffectSlot *effect);
