#include "owner_state.h"
#include "fixed_math.h"

void UnitRatio(s32 ownerId) {
  s32 first;
  s32 second;
  s32 firstValue;
  s32 secondValue;
  struct BattleUnit *owner;

  owner = UnitGet(ownerId);
  first = (owner->hp << 14) / owner->maxHp;
  firstValue = 0x4000;
  if (first <= 0x4000) {
    firstValue = 0;
    if (first >= 0) {
      firstValue = first;
    }
  }
  owner->hpGauge = firstValue;
  if (owner->hpGauge == 0 && owner->hp != 0) {
    firstValue = 1;
    owner->hpGauge = firstValue;
  }
  second = (owner->pp << 14) / owner->maxPp;
  secondValue = 0x4000;
  if (second <= 0x4000) {
    secondValue = 0;
    if (second >= 0) {
      secondValue = second;
    }
  }
  owner->ppGauge = secondValue;
  if (owner->ppGauge == 0 && owner->pp != 0) {
    secondValue = 1;
    owner->ppGauge = secondValue;
  }
}

void Unit_SetHp(struct BattleUnit *state, s32 input) {
  s32 value;

  if (input > state->maxHp) {
    value = state->maxHp;
  } else {
    value = 0;
    if (input >= 0) {
      value = input;
    }
  }
  state->hp = value;
  value = ((value << 16) >> 2) / state->maxHp;

  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    state->hpGauge = output;
    if ((output << 16) == 0 && state->hp != 0) {
      state->hpGauge = 1;
    }
  }

  value = (state->pp << 14) / state->maxPp;
  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    state->ppGauge = output;
    if ((output << 16) == 0 && state->pp != 0) {
      state->ppGauge = 1;
    }
  }
}
