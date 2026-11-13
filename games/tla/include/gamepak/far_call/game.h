#ifndef ALCHEMY_FAR_CALL_GAME_H
#define ALCHEMY_FAR_CALL_GAME_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct BattleUnit;
struct EnemyDefinition;

void Unit_LevelUpToFar(s32 owner, s32 level);

void Unit_LearnPsynergyFar(s32 owner, s32 action);

s32 Inventory_EquipFar(s32 owner, s32 slot);

s32 Inventory_GetEquippedItemFar(struct BattleUnit *unit, s32 slot);

s32 Djinn_CanSetFar(s32 owner, s32 element, s32 djinn);
s32 Inventory_FindEquippedFar(s32 owner, s32 kind);
s32 Djinn_IsSetFar(s32 unitId, s32 element, s32 index);

u32 *DjinnRecovery_AddStandbyFar(u32 owner, u32 element, u32 djinn);
s32 Item_GetEquippedElementFar(s32 unitId);

s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);

s32 PartyInventory_AddFar(s32 item);

const struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);
s32 Summon_ListLearnedFar(u8 *list);
void Djinn_PutOnStandbyFar(s32 unitId, s32 element, s32 djinni);

s32 Party_GetAverageLevelFar(void);

s32 Item_AdjustCounterFar(s32 item, s32 delta);

s32 Inventory_CountItemFar(s32 member, s32 item);
void Inventory_DiscardFar(s32 member, s32 slot);
void Unit_RefreshPsynergyFar(s32 member);
s32 PartyInventory_CountItemFar(s32 item);

void PartyInventory_RemoveFar(s32 item);

s32 Djinn_GiveToPartyFar(s32 element, s32 djinni);

s32 Unit_GetElementLevelFar(s32 ownerId, s32 element);

s32 Item_IsCompatibleWithUnitFar(s32 unit, s32 item);

s32 Inventory_FindFar(s32 unitId, s32 item_id);

s32 Party_AdjustShopCreditFar(s32 amount);

void Inventory_RepairFar(s32 unitId, s32 slot);

void Party_AdjustShopGiftsFar(s32 amount);

// What several parts call, declared once here for all of them.
s32 Item_GetEquipmentGroupFar(s32 item);
s32 Inventory_CountFar(s32 unit);

// No prototype, as The Broken Seal declares it: this caller passes 0, and
// FileList_SelectFile's twin passes nothing.
u32 BuildStamp_GetTimeFar();

#endif
