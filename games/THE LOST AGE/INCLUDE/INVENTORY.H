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
s32 Inventory_Add(s32 owner, s32 item);
s32 Inventory_Find(s32 owner, s32 item);

#endif
