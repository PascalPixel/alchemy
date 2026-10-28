#include "owner_state.h"
#include "adventure.h"

s32 Unit_AddPp();

static __inline__ s32 Owner_GetPpRatio(s32 owner) {
  struct BattleUnit *unit = Unit_Get(owner);

  if (unit->maxPp <= 0)
    return 0;
  return (unit->pp << 16) / unit->maxPp;
}

s32 Func_080adc90(void) {
  s32 pp = 0;
  s32 max = 0;
  s32 count = Party_CountActiveOwners();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(gAdventure.activeOwners[i]);

    pp += unit->pp;
    max += unit->maxPp;
  }
  if (max == 0)
    return 0;
  return (pp << 16) / max;
}

s32 Func_080adcec(void) {
  s32 best = 0;
  s32 bestOwner = -1;
  s32 count = Party_CountActiveOwners();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    s32 ratio = Owner_GetPpRatio(gAdventure.activeOwners[i]);

    if (ratio > best) {
      best = ratio;
      bestOwner = gAdventure.activeOwners[i];
    }
  }
  if (bestOwner == -1)
    return 0;
  return Unit_AddPp(bestOwner, -1);
}

void Func_080add5c(s32 count) {
  s32 i;

  for (i = 0; i < count; i++)
    Func_080adcec();
  Func_080adc90();
}
