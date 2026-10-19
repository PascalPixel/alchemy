#include "owner_state.h"

void UnitRatio(s32);

/// Adds delta to the PP of unit owner, clamped to 0..maxPp, recomputes its
/// gauges and returns the new PP.
s16 Unit_AddPp(s32 owner, s32 delta) {
  struct BattleUnit *state = UnitGet(owner);
  s32 cur = state->pp;
  s32 max = state->maxPp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->pp = result;
  UnitRatio(owner);
  return state->pp;
}
