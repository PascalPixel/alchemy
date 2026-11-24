#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/// @file
/// A unit's fifteen inventory slots: counting, finding, adding and taking
/// items, and finding who in the party carries one.

#include "game/item.h"

#include "game/owner_state.h"
#include "system/main_routine_state.h"

s32 Inventory_Count(s32 owner);
s32 Inventory_CountItem(s32 owner, s32 item);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct Unit *unit,
                                                       s32 type);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 Inventory_AddItem(s32 owner, s32 item);
s32 Inventory_Find(s32 owner, s32 item);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 PartyInventory_FindMember(s32 itemId);

#endif
