/// @file
/// Halt: pausing and resuming objects.
// Part of radial.c.

#include "../radial_internal.h"
#include "module/field/event_commands.h"
#include "module/field/radial.h"

void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object) {
  if (object->age == 80) {
    object->update = 0;
    Sound_PlayFar(136);
  }
  object->age++;
}

/* VIEW(age_and_cue): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); f0dfd00 has void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p). */
extern void Vector_AddPolarOffset__age_and_cue(s32 radius, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");

void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = Ram_HeapSlots->effectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset__age_and_cue(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->limitTurnStep = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset__age_and_cue(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void BattleEffect_MarkChildAndRunFallbackTransition(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
  FieldPsynergy_Halt();
}

void FieldPsynergy_Halt(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *user = scene->mainObject;
  struct Actor *obj;
  struct FieldParticle *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Actor_Spawn(309, 0, 0, 0);
  step = 0;
  Actor_SetMode(obj, 0);
  if (obj == 0)
    return;

  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleEffect_Interpolate(Q16_ONE, 0x4000, step);
    obj->scaleX = scale;
    obj->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  wait(5);
  Actor_SetMode(obj, 1);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(109);

  slot = &((struct FallbackScene *)scene)->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset__age_and_cue(4 << 16, Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateHaltParticle);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  wait(8);
  Actor_Destroy(obj);
  wait(4);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_SetRandomTableValueOnObject(struct ActorView *object) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 index = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 id) {
  struct ActorView *object;
  struct AnimationEntry *entry;

  object = ObjectTable_Get(id);
  if (object != NULL) {
    gMainRoutineWork.savedCallback = (u32)object->update;
    gMainRoutineWork.savedByte = 0;
    if (object->kind == 1) {
      entry = object->target.animation->entries[0];
      if (entry != NULL) {
        gMainRoutineWork.savedByte = entry->param;
      }
    }
    object->update = BattleEffect_SetRandomTableValueOnObject;
    object->value5b = 1;
    Actor_SetAnimationSpeedFar(object, 0);
  }
}

void BattleEffect_ResumeObject(s32 id) {
  struct ActorView *object = ObjectTable_Get(id);

  if (object != NULL) {
    if (object->update == BattleEffect_SetRandomTableValueOnObject) {
      object->update = (void *)gMainRoutineWork.savedCallback;
      gMainRoutineWork.savedCallback = 0;
      Animation_ApplyChildValuesFar(object, (s8)gMainRoutineWork.savedByte);
    }
    object->value5b = 0;
    Actor_SetAnimationSpeedFar(object, 16);
  }
}

// Battle effects: filter the effect's target object.

s32 BattleEffect_GetFlags();

/// Returns no target (-1) when the current effect definition's flags are
/// 0xff, else id.
/// id: target object ID.
s32 BattleEffect_FilterObjectIdByFlags(s32 id) {
  if (BattleEffect_GetFlags() == 0xff)
    return -1;
  return id;
}

/// Runs field effect 13 (FieldPsynergy_Catch) for the request dispatcher.
void BattleEffect_CallEffect13(void) { FieldPsynergy_Catch(); }

static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / 10;
}
