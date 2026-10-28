#include "inventory.h"

s32 Inventory_Find(s32 owner, s32 item_id) {
  struct OwnerInventoryState *inv = Unit_Get(owner);
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if ((inv->inventory[slot] & ITEM_ID_MASK) == item_id) {
      return slot;
    }
  }
  return -1;
}

s32 PartyInventory_FindOwner(s32 itemId) {
  s16 owners[10];
  s32 ownerCount;
  s32 ownerIndex;
  s16 *ownerCursor;
  s16 owner;

  if (Inventory_Find(gSequenceParentWork.currentOwner, itemId) != -1)
    return gSequenceParentWork.currentOwner;
  ownerCount = Party_ListActiveOwners(owners);
  ownerCursor = owners;
  ownerIndex = 0;
  if (ownerIndex < ownerCount) {
    do {
      owner = *ownerCursor++;
      if (Inventory_Find(owner, itemId) != -1)
        return owner;
      ownerIndex++;
    } while (ownerIndex < ownerCount);
  }
  return -1;
}
