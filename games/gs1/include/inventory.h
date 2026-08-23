#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

#include "item.h"
#include "owner_state.h"
#include "party_state.h"

s32 Func_080784b0(s32 owner, s32 slot);
s32 Func_080784d8(s32 owner);
s32 Func_08078500(void);
s32 Func_08078550(void);
s32 Func_08078588(s32 owner, s32 item);
s32 Func_08078618(s32 item);
s32 Func_08078664(s32 owner, s32 item);
s32 Func_08078698();
s32 Func_08078708(s32 owner, s32 slot);
s32 Func_080787dc(s32 owner, s32 type);
struct ItemDefinition *Func_0807882c(
    struct OwnerInventoryState *owner,
    s32 type);
s32 Func_08078870(struct OwnerInventoryState *owner, s32 type);
s32 Func_080788c4(s32 owner, s32 slot);
s32 Func_08078948(s32 owner, s32 slot);
s32 Func_08078980(s32 owner, s32 slot);
s32 Func_080789dc(s32 item);
s32 Func_08078a08(s32 item);
s32 Func_08078a34(s32 owner, s32 slot);
s32 Func_08077060(s32 owner, s32 slot);
s32 Func_08078a60(s32 owner, s32 slot);
s32 Func_08078af8(s32 owner, s32 item);
s32 Func_08078b60(s32 item);
s32 Func_08078bc0(s32 owner, s32 value);

#define Inventory_GetQuantity           Func_080784b0
#define Inventory_Count                 Func_080784d8
#define PartyInventory_HasSpace         Func_08078500
#define PartyInventory_CountFreeSlots   Func_08078550
#define Inventory_Add                   Func_08078588
#define PartyInventory_Add              Func_08078618
#define Inventory_Find                  Func_08078664
#define PartyInventory_FindOwner        Func_08078698
#define Inventory_Equip                 Func_08078708
#define Inventory_FindEquipped          Func_080787dc
#define Inventory_GetEquippedDefinition Func_0807882c
#define Inventory_GetEquippedItem       Func_08078870
#define Inventory_Remove                Func_080788c4
#define Inventory_Discard               Func_08078948
#define Inventory_CheckDiscard          Func_08078980
#define PartyInventory_Remove           Func_080789dc
#define PartyInventory_Discard          Func_08078a08
#define Inventory_Break                 Func_08078a34
#define Inventory_BreakFar              Func_08077060
#define Inventory_Repair                Func_08078a60
#define Inventory_CountItem             Func_08078af8
#define PartyInventory_CountItem        Func_08078b60
#define Equipment_HasValue              Func_08078bc0

#endif
