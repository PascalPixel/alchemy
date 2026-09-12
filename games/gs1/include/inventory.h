#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

#include "item.h"
#include "owner_state.h"
#include "party_state.h"

s32 Inventory_GetQuantity(s32 owner, s32 slot);
s32 Inventory_Count(s32 owner);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_Add(s32 owner, s32 item);
s32 PartyInventory_Add(s32 item);
s32 Inventory_Find(s32 owner, s32 item);
s32 PartyInventory_FindOwner(void);
s32 Inventory_Equip(s32 owner, s32 slot);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(
    struct OwnerInventoryState *owner,
    s32 type);
s32 Inventory_GetEquippedItem(struct OwnerInventoryState *owner, s32 type);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 Inventory_CheckDiscard(s32 owner, s32 slot);
s32 PartyInventory_Remove(s32 item);
s32 PartyInventory_Discard(s32 item);
s32 Inventory_Break(s32 owner, s32 slot);
s32 Inventory_BreakFar(s32 owner, s32 slot);
s32 Inventory_Repair(s32 owner, s32 slot);
s32 Inventory_CountItem(s32 owner, s32 item);
s32 PartyInventory_CountItem(s32 item);
s32 Equipment_HasValue(s32 owner, s32 value);

#endif
