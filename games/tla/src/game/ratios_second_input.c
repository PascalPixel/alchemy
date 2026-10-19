#include "battle_unit.h"

/// Sets the unit's PP, clamped to 0..maxPp, and recomputes its HP and PP
/// gauges.
void Unit_SetPp(struct BattleUnit *unit, s32 pp) {
  s32 clamped;

  if (pp > unit->maxPp) {
    clamped = unit->maxPp;
  } else {
    clamped = 0;
    if (pp >= 0)
      clamped = pp;
  }
  unit->pp = clamped;
  Unit_UpdateGauges(unit);
}
