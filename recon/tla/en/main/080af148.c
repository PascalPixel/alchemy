// Near miss, differ 2 of 57 (one reordered pair), 2026-10-18: Inventory_Remove,
// ported from The Broken Seal's function of the same name
// (games/tbs/src/game/game.c). Remaining: before the tail-clearing loop the
// native loads the pooled halfword zero (ldr r2) before it forms the slot
// address (adds r0, r3, r6); this forms the address first, the order ⚓️'s
// native gives pool loads throughout. In the sched2 dump the zero (cost 2,
// priority 2, no dependents) ties the address (priority 2, one dependent)
// and loses; with a pool load cost of 3 it would have priority 3 and the
// native order. An asm taking the zero as "0" and giving it back as a
// dependent: as s32 the zero becomes movs (differ 13), as u16 it is promoted
// and extended (differ 10). Five minutes of permute, 10,000 candidates, found
// nothing lower.
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
