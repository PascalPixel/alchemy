#include "inventory.h"

s32 Inventory_AddItem(s32 owner_id, s32 item_id) {
  struct OwnerInventoryState *inv = UnitGet(owner_id);
  struct ItemDefinition *item = Item_GetDirect(item_id);
  s32 slot;

  if ((item->flags & ITEM_STACKABLE) != 0) {
    slot = 0;
    if (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0) {
      do {
        slot++;
        if (slot >= INVENTORY_SLOTS)
          break;
      } while (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0);
    }
    if (slot != INVENTORY_SLOTS) {
      s32 entry = inv->inventory[slot];
      u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

      if (count > 29)
        return -1;
      {
        /* FAKEMATCH: a separate mask temporary preserves the and/store
         * registers. */
        s32 value = INVENTORY_ENTRY_MASK;

        value &= entry;
        value |= count << INVENTORY_QUANTITY_SHIFT;
        inv->inventory[slot] = value;
      }
      return slot;
    }
  }

  slot = 0;
  do {
    if (inv->inventory[slot] == 0) {
      inv->inventory[slot] = item_id;
      return slot;
    }
    slot++;
  } while (slot < INVENTORY_SLOTS);
  return -1;
}

s32 PartyInventory_Add(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;

  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      s16 ownerId = *ownerCursor++;

      if (Inventory_AddItem(ownerId, itemId) >= 0)
        return ownerId;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}

s32 PartyInventory_FindRoom(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;

  ownerCount = Party_ListActiveOwners(owners);
  for (ownerIndex = 0; ownerIndex < ownerCount; ownerIndex++) {
    s16 ownerId = owners[ownerIndex];
    s32 slot = Inventory_AddItem(ownerId, itemId);

    if (slot >= 0) {
      Inventory_Remove(ownerId, slot);
      return ownerId;
    }
  }
  return -1;
}
