#include "owner_state.h"
#include "fixed_math.h"

void Unit_UpdateRatios(s32 ownerId) {
  s32 first;
  s32 second;
  s32 firstValue;
  s32 secondValue;
  struct BattleUnit *owner;

  owner = Unit_Get(ownerId);
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

/// Sets the unit's HP, clamped to 0..maxHp, and recomputes its HP and PP
/// gauges inline.
void Unit_SetHp(struct BattleUnit *unit, s32 hp) {
  s32 value;

  if (hp > unit->maxHp) {
    value = unit->maxHp;
  } else {
    value = 0;
    if (hp >= 0) {
      value = hp;
    }
  }
  unit->hp = value;
  value = ((value << 16) >> 2) / unit->maxHp;

  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->hpGauge = output;
    if ((output << 16) == 0 && unit->hp != 0) {
      unit->hpGauge = 1;
    }
  }

  value = (unit->pp << 14) / unit->maxPp;
  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->ppGauge = output;
    if ((output << 16) == 0 && unit->pp != 0) {
      unit->ppGauge = 1;
    }
  }
}
