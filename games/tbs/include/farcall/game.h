/// @file
/// Party members, items, Djinn and game-state operations.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/GAME.S.

/// Far call to Djinn_GetRecoveryTable.
void *Djinn_GetRecoveryTableFar(s32 arg0);
#ifndef OWN_UnitGetFar
/// Far call to UnitGet.
struct BattleUnit *UnitGetFar(u32 owner);
#endif
/// Far call to UnitCalc.
void UnitCalcFar(s32 owner);
/// Far call to Item_GetDirect.
struct ItemDefinition *Item_GetDirectFar(s32 item);
/// Far call to Inventory_GetQuantity.
s32 Shop_GetSelectionState(s32 owner, s32 slot);
/// Far call to Inventory_AddItem.
s32 Inventory_AddItemFar(s32 owner_id, s32 item_id);
/// Far call to PartyInventory_Add.
s32 PartyInventory_AddFar(s32 item_id);
/// Far call to Inventory_Find.
s32 Inventory_FindFar(s32 owner, s32 item_id);
/// Far call to PartyInventory_FindOwner.
s32 PartyInventory_FindOwnerFar(s32 item_id);
/// Far call to PartyInventory_Remove.
s32 PartyInventory_RemoveFar(s32 item_id);
/// Far call to Inventory_Equip.
s32 Inventory_EquipFar(s32 owner, s32 slot);
/// Far call to Inventory_Remove.
s32 Inventory_RemoveFar(s32 owner, s32 slot);
/// Far call to Inventory_Break.
s32 Inventory_BreakFar(s32 owner, s32 slot);
/// Far call to Inventory_Repair.
s32 Inventory_RepairFar(s32 owner, s32 slot);
/// Far call to Inventory_GetEquippedItem.
s32 Inventory_GetEquippedItemFar(struct BattleUnit *inv, s32 type);
/// Far call to BattleAction_GetDirect.
struct BattleAct *BattleActGet(s32 actionId);
/// Far call to Unit_LearnPsynergy.
s32 Unit_LearnPsynergyFar();
/// Far call to Unit_KnowsAction.
s32 Unit_KnowsActionFar(s32 owner, s32 value);
/// Far call to GameState_InitDefaults.
void GameState_InitDefaultsFar(void);
/// Far call to Unit_TryLevelUp.
struct LevelUpResult *Unit_TryLevelUpFar(s32 owner_id,
                                         struct LevelUpResult *value);
/// Far call to GameFlag_Test.
s32 GameFlag_TestFar(s32 flag);
/// Far call to GameFlag_SetBit.
s32 GameFlag_SetBitFar(s32 flag);
/// Far call to GameFlag_ClearBit.
void GameFlag_ClearBitFar(s32 flag);
/// Far call to GameFlag_GetByte.
s32 GameFlag_GetByteFar(s32 flag);
/// Far call to GameFlag_SetByte.
void GameFlag_SetByteFar(s32 flag, s32 value);
/// Far call to Unit_AddHp.
s32 Unit_AddHpFar(s32 owner, s32 delta);
/// Far call to Unit_AddPp.
s16 Unit_AddPpFar(s32 owner, s32 delta);
/// Far call to UnitRatio.
void UnitRatioFar(s32 owner_id);
/// Far call to Unit_SetHp.
void Unit_SetHpFar(struct BattleUnit *unit, s32 hp);
/// Far call to BattleUnitSet.
s32 BattleUnitSetFar(s32 unitId, s32 enemyId, s32 suffix);
/// Far call to Party_CountActiveOwners.
s32 Party_CountActiveOwnersFar(void);
/// Far call to Party_AddActiveOwner.
s32 Party_AddActiveOwnerFar(s32 value);
/// Far call to Party_ListActiveOwners.
s32 Party_ListActiveOwnersFar(s16 *owners);
/// Far call to RollWeaponUnleash.
s32 RollWeaponUnleashFar(struct BattleUnit *owner);
/// Far call to Party_RemoveActiveOwner.
s32 Party_RemoveActiveOwnerFar(s32 value);
/// Far call to Item_GetEquippedElement.
s32 Item_GetEquippedElementFar(s32 id);
/// Far call to BattleFx_RollSuccess.
s32 BattleHitCheck(s32 caster, s32 target, s32 resistance_category,
                   s32 effect_id, s32 success_scale);
/// Far call to Battle_CalcAttackDamage.
s32 BattleAtkCalc(s32 atk, s32 def, s32 add, s32 bonus);
/// Far call to Battle_CalcPsynergyPower.
s32 BattlePowerCalc(s32 power, s32 bonus, s32 rate);
/// Far call to Battle_CalcRecovery.
s32 BattleRecoveryCalc(s32 power, s32 element, s32 rate);
/// Far call to Enemy_GetDefinition.
const u8 *Enemy_GetDefinitionFar(s32 selector);
/// Far call to BattleRandom.
s32 BattleRandomFar(void);
/// Far call to Djinn_AddToOwner.
s32 Djinn_AddToOwnerFar(s32 owner, s32 index, s32 bit);
/// Far call to Djinn_Activate.
s32 Djinn_ActivateFar(s32 owner, s32 index, s32 bit);
/// Far call to Djinn_Deactivate.
u32 Djinn_DeactivateFar(s32 owner, s32 index, s32 bit);
/// Far call to DjinnRecovery_Remove.
s32 DjinnRecovery_RemoveFar(s32 owner, s32 index, s32 bit);
/// Far call to DjinnRecovery_AddStandby.
u32 *DjinnRecovery_AddStandbyFar(u32 kind, u32 first, u32 second);
/// Far call to Djinn_Transfer.
s32 Djinn_TransferFar(s32 source, s32 index, s32 bit, s32 target);
/// Far call to Summon_ListLearned.
s32 Summon_ListLearnedFar(u8 *output);
/// Far call to SummonDefinition_GetNear.
const struct SummonDefinition *SummonDefinition_Get(u32 summon_id);
/// Far call to Djinn_GetAbilityId.
s32 Djinn_GetAbilityIdFar(u32 group, u32 index);
/// Far call to Unit_LevelUpTo.
void Unit_LevelUpToFar(s32 unit, s32 lv);
/// Far call to Unit_GetElementLevel.
s32 Unit_GetElementLevelFar(s32 owner, s32 index);
/// Far call to Djinn_IsActive.
u32 Djinn_IsActiveFar(s32 owner, s32 index, s32 bit);
/// Far call to Djinn_IsReady.
s32 Djinn_IsReadyFar(s32 owner, s32 index, s32 bit);
/// Far call to Item_CanOwnerEquipDirect.
s32 Item_CanOwnerEquip(s32 unit, s32 item);
/// Far call to Item_IsCompatibleWithOwner.
s32 Item_IsCompatibleWithOwnerFar(s32 unit, s32 item);
/// Far call to Inventory_FindEquipped.
s32 Inventory_FindEquippedFar(s32 owner, s32 type);
/// Far call to Party_AdjustCoins.
s32 Party_AdjustCoinsFar(s32 amount);
/// Far call to Item_GetEquipmentGroup.
s32 Item_GetEquipmentGroupFar(s32 item_id);
/// Far call to Item_AdjustCounter.
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);
/// Far call to Inventory_Count.
s32 Inventory_CountFar(s32 owner);
/// Far call to PartyInventory_HasSpace.
s32 PartyInventory_HasSpaceFar(void);
/// Far call to Unit_GetExperienceForLevel.
u32 Unit_GetExperienceForLevelFar(s32 owner, s32 level);
/// Far call to Party_RestoreHpPp.
void Party_RestoreHpPpFar(s32 arg0);
/// Far call to Party_RestorePpOfLitElements.
void Party_RestorePpOfLitElementsFar(void);
/// Far call to Party_BeginChildhood.
void Party_BeginChildhoodFar(void);
/// Far call to Party_EndChildhood.
void Party_EndChildhoodFar(void);
/// Far call to Djinn_AddToLeastLoadedOwner.
s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinn);
/// Far call to Party_SumDjinnCounts.
s32 Party_SumDjinnCountsFar(s32 index);
/// Far call to Party_AdjustShopCredit.
s32 Party_AdjustShopCreditFar(s32 amount);
/// Far call to Party_AdjustShopGifts.
s32 Party_AdjustShopGiftsFar(s32 amount);
/// Far call to Inventory_CheckDiscard.
s32 Inventory_CheckDiscardFar(s32 owner, s32 slot);
/// Far call to Inventory_Discard.
s32 Inventory_DiscardFar(s32 owner, s32 slot);
/// Far call to Battle_IsReviveEffect.
s32 Battle_IsReviveEffectFar(s32 effect_id);
/// Far call to Unit_RefreshPsynergy.
s32 Unit_RefreshPsynergyFar(s32 owner);
/// Far call to Party_GetAverageLevel.
u32 Party_GetAverageLevelFar(void);
/// Far call to Inventory_CountItem.
s32 Inventory_CountItemFar(s32 owner, s32 item_id);
/// Far call to PartyInventory_CountItem.
s32 PartyInventory_CountItemFar(s32 item_id);
/// Far call to PartyInventory_Discard.
s32 PartyInventory_DiscardFar(s32 item_id);
/// Far call to GameFlag_RefreshLureCap.
void GameFlag_RefreshLureCapFar(void);
/// Far call to Equipment_GetUnleashRateBonus.
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);
/// Far call to Runtime_GetBuildStampTime.
u32 Runtime_GetBuildStampTimeFar();
/// Far call to PartyInventory_CountFreeSlots.
s32 PartyInventory_CountFreeSlotsFar(void);
/// Far call to InventorySnapshot_Save.
void InventorySnapshot_SaveFar(void);
/// Far call to InventorySnapshot_Restore.
void InventorySnapshot_RestoreFar(void);
