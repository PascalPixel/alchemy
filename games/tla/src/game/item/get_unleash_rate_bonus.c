#include "types.h"
#include "battle_unit.h"
#include "item.h"
#include "inventory.h"
#include "battle_random.h"

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner) {
  s32 sum;
  s32 i;
  s32 j;
  struct ItemDefinition *item;

  sum = 0;
  for (i = 0; i < 15; i++) {
    if (owner->inventory[i] & 0x200) {
      item = Item_GetDirect(owner->inventory[i]);
      for (j = 0; j < 4; j++) {
        if (item->effects[j].kind == 23)
          sum += item->effects[j].amount;
      }
    }
  }
  if (sum < 0)
    sum = 0;
  return sum;
}

s32 RollWeaponUnleash(struct BattleUnit *owner) {
  struct ItemDefinition *item;
  s32 rate;

  if (owner->classIndex == 0) {
    return 1;
  }
  item = Inventory_GetEquippedDefinition(owner, 1);
  if (item == NULL) {
    return 1;
  }
  if (item->unleashAction == 0) {
    return 1;
  }
  rate = ((Equipment_GetUnleashRateBonus(owner) + (item->unleashChance * 5))
          << 0x10) /
         100;
  if (rate > (s32)(Battle_Random() & 0xFFFF)) {
    return item->unleashAction;
  }
  return 1;
}
