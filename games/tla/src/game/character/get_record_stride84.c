#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"

s32 Inventory_GetEquippedItem(struct BattleUnit *owner, s32 type);

struct ClassDefinition *Class_GetDefinition(s32 classId) {
  return &gClass_DefinitionTable[classId];
}

void Unit_UpdateClass(s32 ownerId) {
  struct BattleUnit *owner = UnitGet(ownerId);

  owner->classIndex = Class_Determine(owner->enemyId, owner->djinnAvailable,
                                      Inventory_GetEquippedItem(owner, 10));
  Owner_UpdateClassActions(ownerId);
  Unit_CalcBaseElements(ownerId, owner->baseElement);
}

void Unit_RefreshPsynergy(s32 owner_id) { Unit_UpdateClass(owner_id); }

extern const s16 gElement_PowerResistCurve[5][2];

s32 Battle_GetElementMultiplier(s32 input, s32 halve) {
  s32 min;
  s32 max;
  s32 n;
  s32 i;
  s32 value;
  s32 dx;
  s32 dy;

  max = gElement_PowerResistCurve[0][0];
  min = gElement_PowerResistCurve[4][0];
  n = 5;
  if (input > max)
    input = max;
  else if (input < min)
    input = min;
  for (i = 0; i < n; i++) {
    if (input > gElement_PowerResistCurve[i][0])
      break;
  }
  if (i == n) {
    value = gElement_PowerResistCurve[i - 1][1];
  } else {
    dx = gElement_PowerResistCurve[i - 1][0] - gElement_PowerResistCurve[i][0];
    dy = gElement_PowerResistCurve[i - 1][1] - gElement_PowerResistCurve[i][1];
    value = (input - gElement_PowerResistCurve[i][0]) * dy / dx +
            gElement_PowerResistCurve[i][1];
  }
  switch (halve) {
  case 0:
    break;
  case 1:
    value /= 2;
    break;
  }
  return value + 0x100;
}
