#include "types.h"
#include "effect_slot.h"
#include "motobj.h"
#include "party_state.h"
#include "ram_buffer.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct BattleFxScene {
  u8 unk00[0x50];
  struct EffectSlot slots[24];
  u8 unk710[0xe4];
};

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gFrameTick;
extern u32 gData_080f0bfc[];

void *Object_GetById(s32 id);
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void WaitFrames(s32 frames);
void BattleEffect_AdvanceSpinAngle();
void ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void Camera_WorldToScreen(struct EffectVector *position);
void Object_Destroy(void *object);
void EffectSlot_Initialize(struct EffectSlot *slot, s32 kind, s32 x, s32 z);
void Func_080d7b04(struct EffectSlot *slot);
void Animation_ApplyChildValuesToRecordFar(void *object, u32 value);
u32 Random16(void);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void Object_SetMode(void *object, s32 mode);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);

/// Ends a heavy impact: object id spins, leaps and vanishes in 24 sparks that
/// circle and then close on the current owner, which turns and takes mode 28.
void Djinn_RunJupiterJoinEffect(s32 id) {
  struct EffectVector position;
  struct MotionObject *object;
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 scale;
  s32 count;

  object = Object_GetById(id);
  if (object == NULL)
    return;
  BattleEffect_InitializeSlots();
  scene = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  Audio_PlayCue(173);
  Motion_SetVarCbAndRefresh(id, 1);
  Audio_PlayCue(174);
  Motion_SetVarCbAndRefresh(id, 1);
  Audio_PlayCue(175);
  Motion_SetVarCbAndRefresh(id, 1);
  WaitFrames(20);

  // Spin, then leap high.
  Audio_PlayCue(140);
  object->update = BattleEffect_AdvanceSpinAngle;
  WaitFrames(40);
  Audio_PlayCue(153);
  ObjectMotion_Launch(id, 12, 22);

  // 24 sparks from the object's place on screen, one a frame.
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Camera_WorldToScreen(&position);
  Object_Destroy(object);
  Audio_PlayCue(164);
  slot = scene->slots;
  count = 23;
  do {
    EffectSlot_Initialize(slot, 672, position.x, position.z);
    EffectSlot_SetCallback(slot, Func_080d7b04);
    EffectSlot_SetObjectMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = Random16() / 3 + 0x10000;
    slot->scaleY = scale;
    slot->scaleX = scale;
    count--;
    WaitFrames(1);
    slot++;
  } while (count >= 0);

  // The current owner turns to 0x4000 and takes mode 28.
  WaitFrames(60);
  ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(Object_GetById(gPartyState.currentOwner), 28);
  WaitFrames(40);
  Audio_PlayCue(164);
  WaitFrames(100);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

void BattleEffect_SetObjectAlternatingWords(u8 *object) {
  u32 *tick = &gFrameTick;
  u32 *table;
  u32 index;
  u32 value;

  asm volatile(
      ""
      : "+l"(tick)); /* FAKEMATCH: load the tick address before the table. */
  table = gData_080f0bfc;
  /* FAKEMATCH: keep the tick extraction in its own block; without this
   * boundary the table address loads after the tick (two reordered rows). */
  do {
    index = (*tick >> 2) & 1;
  } while (0);
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
  /* FAKEMATCH: the step is pinned to r1 and used by an empty asm before the
   * scales load, so its literal loads first; without the pin, 18 instructions
   * differ in tla-en. */
  register s32 step asm("r1") = 0xFFFFFE40;
  s32 scaleY;
  s32 scale;
  s32 angle;

  asm("" : : "l"(step));
  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  /* FAKEMATCH: the barrier keeps the angle's load ahead of the second
   * scale's add (differ 4 without it). */
  asm volatile("" : "+l"(angle));
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + 0x2000;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Object_Destroy(obj);
}
