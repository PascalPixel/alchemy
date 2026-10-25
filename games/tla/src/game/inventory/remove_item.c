#include "inventory.h"

void Unit_Calculate(s32 owner);

static inline void Inventory_CompactSlots(s16 *inventory) {
  s32 usedSlots = 0;
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (inventory[slot] != 0)
      inventory[usedSlots++] = inventory[slot];
  }
  for (; usedSlots < INVENTORY_SLOTS; usedSlots++)
    inventory[usedSlots] = 0;
}

/// Empties an inventory slot outright, closes the inventory up and
/// recalculates the unit; returns 2, as Inventory_Remove does for a last item.
s32 Func_080af0e4(s32 owner, s32 slot) {
  struct OwnerInventoryState *unit = Unit_Get(owner);

  unit->inventory[slot] = 0;
  Inventory_CompactSlots((s16 *)unit->inventory);
  Unit_Calculate(owner);
  return 2;
}

s32 Inventory_Remove(s32 owner, s32 slot) {
  struct OwnerInventoryState *unit = Unit_Get(owner);
  u16 item = unit->inventory[slot];
  s32 result = -1;

  if (item != 0) {
    if (item & INVENTORY_QUANTITY_MASK) {
      unit->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
      result = 1;
    } else {
      unit->inventory[slot] = 0;
      Inventory_CompactSlots((s16 *)unit->inventory);
      result = 2;
    }
  }
  Unit_Calculate(owner);
  return result;
}
