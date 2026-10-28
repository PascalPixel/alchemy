#include "types.h"
#include "effect_slot.h"
#include "motobj.h"
#include "adventure.h"
#include "ram_buffer.h"

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

void *Object_GetById(s32 id);
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void Sound_PlayFar(s32 cue);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void WaitFrames(s32 frames);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void Object_SetMode(void *object, s32 mode);
void Camera_WorldToScreen(void *position);
void FieldParticle_Initialize(void *slot, s32 kind, s32 x, s32 z);
void Animation_ApplyChildValuesToRecordFar(s32 object, s32 value);
u32 Random16(void);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);
void Object_Destroy(void *object);
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

  djinni = Object_GetById(djinniId);
  leader = Object_GetById(gAdventure.currentOwner);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x201090);
  WaitFrames(30);
  ObjectMotion_ArmCallback(djinniId, 0x4000, 0);
  WaitFrames(20);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(20);
  Sound_PlayFar(140);
  djinni->update = BattleEffect_SpinUpAngle;
  djinni->timer = 0;
  WaitFrames(80);
  djinni->update = BattleEffect_ShrinkObjectAndDestroySlow;
  Object_SetMode(djinni, 3);

  // The sparks start where the Djinni is on screen.
  position.x = djinni->x;
  {
    void *p = &position;
    s32 y;

    /* FAKEMATCH: the barriers place the position's address, the y load and
     * the spark count where the native has them; without them the count
     * comes after the call (differ 4). */
    asm volatile("" : "+l"(p));
    y = djinni->y;
    remaining = 23;
    asm volatile("" : "+l"(remaining));
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
    value = Random16() / 3 + 0x10000; // 1.0 to 1.33
    ((struct FieldParticle *)slot)->scaleY = value;
    ((struct FieldParticle *)slot)->scaleX = value;
    remaining--;
    WaitFrames(1);
    slot = (u8 *)slot + sizeof(struct FieldParticle);
  } while (remaining >= 0);

  WaitFrames(60);
  ObjectMotion_ArmCallback(gAdventure.currentOwner, 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(Object_GetById(gAdventure.currentOwner), 28);

  // Every spark still in use stops its fountain (state 2). The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    WaitFrames(20);
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

  WaitFrames(60);
  leader->update = BattleEffect_SetObjectAlternatingWords;
  WaitFrames(100);

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

  WaitFrames(10);
  slot = NULL;
  leader->update = slot;
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  WaitFrames(30);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Spins an object 1/8 turn and shrinks it by 0x400 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroyFast(void *obj) {
  /* FAKEMATCH: the step pinned to r1 and used by an empty asm loads its literal
   * before the scales; as a plain local, 18 instructions differ in tla-en. */
  register s32 step asm("r1") = 0xFFFFFC00;
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
