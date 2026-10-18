// Near miss, differ 6 of 43 (two reordered pairs), 2026-10-18: empties an
// inventory slot outright, closes the inventory up and recalculates the unit;
// returns 2, as Inventory_Remove does for a last item, with The Broken Seal's
// compaction. Remaining: the native loads both pooled halfword zeros earlier
// (ldr r3 before adds r6, r0 and lsls r5; ldr r2 before adds r0, r3, r6).
// A do-while tail behind an empty asm on the zero lays the loops out
// otherwise (differ 42).
#include "inventory.h"

void UnitCalc(s32 owner);

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

s32 Func_080af0e4(s32 owner, s32 slot) {
  struct OwnerInventoryState *unit = UnitGet(owner);

  unit->inventory[slot] = 0;
  Inventory_CompactSlots((s16 *)unit->inventory);
  UnitCalc(owner);
  return 2;
}
