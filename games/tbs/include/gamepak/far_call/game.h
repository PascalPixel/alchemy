#ifndef ALCHEMY_FAR_CALL_GAME_H
#define ALCHEMY_FAR_CALL_GAME_H

/// @file
/// Party members, items, Djinn and game-state operations.

struct DjinnRecoveryTable *Djinn_GetRecoveryTableFar(s32 enemySide);
struct Unit *Unit_GetFar(u32 unitId);
/// Works a unit's stats out again from its base stats, equipment, set Djinn,
/// class and battle stat levels, keeping its HP and PP at the same fraction.
void Unit_CalculateFar(s32 unitId);
struct ItemDefinition *Item_GetDirectFar(s32 item);
/// Returns how many of the item in `slot` the unit carries, 0 for an empty
/// slot (Inventory_GetQuantity); the sell screen reads a stack's size with it.
s32 Shop_GetSelectionState(s32 unitId, s32 slot);
s32 Inventory_AddItemFar(s32 unitId, s32 itemId);
s32 PartyInventory_AddFar(s32 itemId);
s32 Inventory_FindFar(s32 unitId, s32 itemId);
s32 PartyInventory_FindMemberFar(s32 itemId);
s32 PartyInventory_RemoveFar(s32 itemId);
s32 Inventory_EquipFar(s32 unitId, s32 slot);
s32 Inventory_RemoveFar(s32 unitId, s32 slot);
s32 Inventory_BreakFar(s32 unitId, s32 slot);
s32 Inventory_RepairFar(s32 unitId, s32 slot);
s32 Inventory_GetEquippedItemFar(struct Unit *unit, s32 type);
/// Returns the definition of an ability by its low 14 bits, the first for an id
/// past the table (Ability_GetDirect); every bank reads abilities so.
struct AbilityDefinition *Ability_Get(s32 abilityId);
s32 Unit_LearnPsynergyFar();
s32 Unit_KnowsPsynergyFar(s32 unitId, s32 abilityId);
void MainRoutineState_InitializeDefaultsFar(void);
struct LevelUpResult *Unit_TryLevelUpFar(s32 unitId,
                                         struct LevelUpResult *gains);
s32 GameFlag_GetFar(s32 flag);
s32 GameFlag_SetFar(s32 flag);
/// Clears the flag, returning nothing where GameFlag_SetFar returns its
/// byte.
void GameFlag_ClearFar(s32 flag);
s32 GameFlag_GetByteFar(s32 flag);
void GameFlag_SetByteFar(s32 flag, s32 value);
/// Adds `delta` to the unit's HP, kept between 0 and its maximum, updates its
/// gauges and returns the new HP.
s32 Unit_AddHpFar(s32 unitId, s32 delta);
/// Adds `delta` to the unit's PP, kept between 0 and its maximum, updates its
/// gauges and returns the new PP.
s16 Unit_AddPpFar(s32 unitId, s32 delta);
void Unit_RecalculateGaugesFar(s32 unitId);
/// Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
void Unit_SetHpFar(struct Unit *unit, s32 hp);
s32 BattleUnit_SetFar(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountMembersFar(void);
s32 Party_AddMemberFar(s32 unitId);
s32 Party_ListMembersFar(s16 *unitIds);
s32 RollWeaponUnleashFar(struct Unit *unit);
s32 Party_RemoveMemberFar(s32 unitId);
s32 Item_GetEquippedElementFar(s32 unitId);
s32 Battle_RollEffectSuccessFar(s32 caster, s32 target, s32 element,
                                s32 effectId, s32 successScale);
s32 Battle_CalculateAttackDamageFar(s32 attack, s32 defense, s32 addedPower,
                                    s32 elementDifference);
s32 Battle_CalculatePsynergyPowerFar(s32 power, s32 elementDifference,
                                     s32 powerMultiplier);
s32 Battle_CalculateRecoveryFar(s32 power, s32 elementPower,
                                s32 powerMultiplier);
const struct EnemyDefinition *Enemy_GetDefinitionFar(s32 enemyId);
s32 Battle_RandomFar(void);
s32 Djinn_GiveFar(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_SetFar(s32 unitId, s32 element, s32 djinnIndex);
u32 Djinn_PutOnStandbyFar(s32 unitId, s32 element, s32 djinnIndex);
s32 DjinnRecovery_RemoveFar(s32 unitId, s32 element, s32 djinnIndex);
u32 *DjinnRecovery_AddStandbyFar(u32 unitId, u32 element, u32 djinnIndex);
s32 Djinn_TransferFar(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 Summon_ListLearnedFar(u8 *output);
/// Returns a summon's definition, or NULL past the sixteenth
/// (SummonDefinition_GetNear).
const struct SummonDefinition *SummonDefinition_Get(u32 summonId);
s32 Djinn_GetAbilityIdFar(u32 element, u32 djinni);
void Unit_LevelUpToFar(s32 unitId, s32 targetLevel);
s32 Unit_GetElementLevelFar(s32 unitId, s32 element);
u32 Djinn_IsSetFar(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_CanSetFar(s32 unitId, s32 element, s32 djinnIndex);
/// Returns whether the character's class may equip the item, 0 for any unit
/// past the eight characters (Item_CanUnitEquipDirect).
s32 Item_CanUnitEquip(s32 unitId, s32 itemId);
s32 Item_IsCompatibleWithUnitFar(s32 unitId, s32 itemId);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
s32 Party_AdjustCoinsFar(s32 amount);
s32 Item_GetEquipmentGroupFar(s32 itemId);
s32 Item_AdjustCounterFar(s32 itemId, s32 delta);
s32 Inventory_CountFar(s32 unitId);
s32 PartyInventory_HasSpaceFar(void);
u32 Unit_GetExperienceForLevelFar(s32 unitId, s32 level);
void Party_RestoreHpPpFar(s32 cureConditions);
void Party_RestorePpOfLitElementsFar(void);
void Party_StartChildhoodFar(void);
void Party_EndChildhoodFar(void);
s32 Djinn_GiveToPartyFar(s32 element, s32 djinn);
s32 Party_SumDjinnCountsFar(s32 element);
/// Adds `amount` to the shop credit purchases build up and Game Tickets spend,
/// kept between 0 and COIN_MAX, and returns the new credit.
s32 Party_AdjustShopCreditFar(s32 amount);
/// Adds `amount` to the count of Game Tickets given, which stops at
/// SHOP_GIFTS_MAX and picks the next ticket's credit cost, and returns the new
/// count.
s32 Party_AdjustShopGiftsFar(s32 amount);
s32 Inventory_CheckDiscardFar(s32 unitId, s32 slot);
s32 Inventory_DiscardFar(s32 unitId, s32 slot);
s32 Battle_IsReviveEffectFar(s32 effectId);
s32 Unit_RefreshPsynergyFar(s32 unitId);
u32 Party_GetAverageLevelFar(void);
s32 Inventory_CountItemFar(s32 unitId, s32 itemId);
s32 PartyInventory_CountItemFar(s32 itemId);
s32 PartyInventory_DiscardFar(s32 itemId);
void GameFlag_RefreshLureCapFar(void);
s32 Equipment_GetUnleashRateBonusFar(struct Unit *unit);
u32 BuildStamp_GetTimeFar();
s32 PartyInventory_CountFreeSlotsFar(void);
void InventorySnapshot_SaveFar(void);
void InventorySnapshot_RestoreFar(void);

#endif
