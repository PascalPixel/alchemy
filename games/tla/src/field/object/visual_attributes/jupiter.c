/// @file
/// A Jupiter Djinni joining the party.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "gamepak/far_call/shop.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "battle/object_effect_scripts.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

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
  FieldEffect_InitializeSlots();
  scene = Ram_HeapSlots->effectWork;
  Shop_SaveAndTintFieldPaletteFar(0x20118c);
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
  Camera_WorldToScreen(&position);
  Actor_ReleaseFar(object);
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
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(40);
  Sound_PlayFar(164);
  wait(100);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
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

void FieldEffect_AdvanceAngleWithRamp(void *obj) {
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
void FieldEffect_ShrinkActorAndDestroySlow(void *obj) {
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
    Actor_ReleaseFar(obj);
}

struct BattleFxScene__motes;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

void FieldEffect_UpdateMarsSpark(struct FieldParticle *effect) {
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
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
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
      FieldEffect_ClearOwnedSlot(effect);
  }
}

#undef Actor_SetChildValueUnlessFifteenFar
#undef gBattleEffect_PulseScales
#undef gEffectWork
