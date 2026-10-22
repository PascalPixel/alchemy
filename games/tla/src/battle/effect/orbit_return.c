#include "fixed_math.h"
#include "system.h"
#include "effect_slot.h"

void BattleEffect_ClearOwnedSlot(struct EffectSlot *);

struct Output_08097f80 {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);

void BattleEffect_UpdateOrbitAndReturn(struct EffectSlot *effect) {
  struct Output_08097f80 position;
  s8 *statePointer = &effect->state;
  s32 state;

next_state:
  state = *statePointer;
  if (state == 0) {
    u32 angle;

    position.x = effect->originX;
    position.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x40000;
    effect->maxSpeed = 0x40000;
    effect->limitTurnStep = state;
    (*statePointer)++;
    return;
  }

  if (state == 1) {
    if (EffectSlot_HasPendingTarget(effect) == 0) {
      (*statePointer)++;
      goto next_state;
    }
    return;
  }

  if (state == 2) {
    effect->targetX = effect->originX;
    effect->targetZ = effect->originZ;
    {
      u32 value = 0x400;

      effect->maxTurnStep = value;
    }
    effect->limitTurnStep = 1;
    (*statePointer)++;
    return;
  }

  if (state == 3 && EffectSlot_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}
