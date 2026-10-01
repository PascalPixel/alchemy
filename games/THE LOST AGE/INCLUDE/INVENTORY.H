#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

struct ItemDefinition {
    s16 price;
    u8 type;
    u8 flags;
};

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Inventory_Count(s32 owner);
s32 Inventory_AddItem(s32 owner, s32 item);
s32 Inventory_Find(s32 owner, s32 item);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 PartyInventory_FindOwner(s32 item_id);

#endif
