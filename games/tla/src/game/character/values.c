#include "owner_state.h"
#include "owner_value.h"

void UnitRatio(s32);

/// Adds delta to the HP of unit owner, clamped to 0..maxHp, recomputes its
/// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta) {
  struct BattleUnit *state = UnitGet(owner);
  s32 cur = state->hp;
  s32 max = state->maxHp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->hp = result;
  UnitRatio(owner);
  return state->hp;
}
