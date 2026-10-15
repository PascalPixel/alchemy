#include "inventory.h"

s32 Inventory_CheckUnequip(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = UnitGet(owner);
  s32 raw = inv->inventory[slot];
  s32 item_id;
  struct ItemDefinition *item;
  s32 result;

  asm volatile(
      ""); /* FAKEMATCH: ⚓️ loads the slot before it builds the item mask */
  /* FAKEMATCH: the mask is built in the register it masks into; raw &
   * ITEM_ID_MASK swaps r3 and r5 */
  item_id = ITEM_ID_MASK;
  item_id &= raw;
  item = Item_GetDirect(item_id);
  result = 0;

  if (item_id == 0)
    return 8;
  if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
    result = 1;
  return result;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot) {
  struct OwnerInventoryState *inv = UnitGet(owner);
  s32 raw = inv->inventory[slot];
  s32 item_id;
  struct ItemDefinition *item;
  s32 result;

  asm volatile(
      ""); /* FAKEMATCH: ⚓️ loads the slot before it builds the item mask */
  /* FAKEMATCH: the mask is built in the register it masks into; raw &
   * ITEM_ID_MASK swaps r3 and r5 */
  item_id = ITEM_ID_MASK;
  item_id &= raw;
  item = Item_GetDirect(item_id);
  result = 0;

  if (item_id == 0)
    return 8;
  if ((item->flags & ITEM_INDISPENSABLE) != 0)
    result = 2;
  if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
      (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
    result |= 1;
  return result;
}

s32 PartyInventory_Remove(s32 item_id) {
  s32 owner = PartyInventory_FindOwner(item_id);

  if (owner == -1)
    return 0;
  Inventory_Remove(owner, Inventory_Find(owner, item_id));
  return 0;
}

s32 PartyInventory_Discard(s32 item_id) {
  s32 owner = PartyInventory_FindOwner(item_id);

  if (owner == -1)
    return 0;
  Inventory_Discard(owner, Inventory_Find(owner, item_id));
  return 0;
}
