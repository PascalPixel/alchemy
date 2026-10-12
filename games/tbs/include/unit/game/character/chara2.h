#ifndef ALCHEMY_UNIT_GAME_CHARACTER_CHARA2_H
#define ALCHEMY_UNIT_GAME_CHARACTER_CHARA2_H

#include "types.h"

struct BtlAct;
struct BtlUnit;
struct CharacterDefinition;
struct ItemDefinition;
struct LevelUpResult;

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item);
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item);
s32 Item_GetEquipmentGroup(s32 item_id);
s32 Inventory_GetQuantity(s32 owner, s32 slot);
s32 Inventory_Count(s32 owner);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_AddItem(s32 owner_id, s32 item_id);
s32 PartyInventory_Add(s32 item_id);
s32 Inventory_Find(s32 owner, s32 item_id);
s32 PartyInventory_FindOwner(s32 item_id);
s32 Inventory_Equip(s32 owner, s32 slot);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BtlUnit *inv, s32 type);
s32 Inventory_GetEquippedItem(struct BtlUnit *inv, s32 type);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 Inventory_CheckDiscard(s32 owner, s32 slot);
s32 PartyInventory_Remove(s32 item_id);
s32 PartyInventory_Discard(s32 item_id);
s32 Inventory_Break(s32 owner, s32 slot);
s32 Inventory_Repair(s32 owner, s32 slot);
u8 Item_GetTargetMode(s32 item_id);
s32 ItemCounter_Adjust(s32 index, s32 delta);
s32 Item_AdjustCounter(s32 item_id, s32 delta);
s32 Inventory_CountItem(s32 owner, s32 item_id);
s32 PartyInventory_CountItem(s32 item_id);
struct BtlAct *BattleAction_GetDirect(s32 action_id);
s32 Equipment_HasValue(s32 owner, s32 value);
s32 Owner_RefreshClassActions(s32 owner);
s32 OwnerAction_Add(s32 state_index, s32 value);
struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *value);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Owner_InitRecords(void);
void Owner_LevelNoOp(void);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res);
struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner_no, struct LevelUpResult *value);
void Party_AdvanceOwnerCountToTarget(s32 unit, s32 lv);

#endif
