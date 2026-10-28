#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

#include "item.h"

#include "owner_state.h"
#include "adventure.h"

s32 Inventory_Count(s32 owner);
s32 Inventory_CountItem(s32 owner, s32 item);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *
Inventory_GetEquippedDefinition(struct OwnerInventoryState *owner, s32 type);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 Inventory_AddItem(s32 owner, s32 item);
s32 Inventory_Find(s32 owner, s32 item);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 PartyInventory_FindOwner(s32 item_id);

#endif
