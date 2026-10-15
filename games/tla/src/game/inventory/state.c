#include "inventory.h"
#include "battle_system.h"
#include "battle_unit.h"

enum { ITEM_COUNTER_COUNT = 300 };

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

s32 Inventory_Break(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] |= 0x400;
  return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = UnitGet(owner);
  if (inv->inventory[slot] == 0) {
    return -1;
  }
  inv->inventory[slot] &= ~0x400;
  return 0;
}

u8 Item_GetTargetMode(s32 itemId) {
  return BattleAction_GetDirect(Item_GetDirect(itemId)->actionId)->targetMode;
}

s32 ItemCounter_Adjust(s32 index, s32 delta) {
  s32 counter_slot = index;
  u8 *data = gItemCounters;

  index = 0;
  if (counter_slot < ITEM_COUNTER_COUNT) {
    s32 value = data[counter_slot];

    value += delta;

    if (value < 0) {
      value = 0;
    } else if (value > 99) {
      value = 99;
      index = 99;
    } else {
      index = value;
    }
    data[counter_slot] = value;
  }
  return index;
}
