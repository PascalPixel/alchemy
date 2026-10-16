/* FarCall_GameTable: one entry per 8-byte stub, in table order. */
FAR_CALL(Djinn_GetRecoveryTableFar, Djinn_GetRecoveryTable, void *, (s32 arg0))
#ifndef OWN_UnitGetFar
FAR_CALL(UnitGetFar, UnitGet, struct BattleUnit *, (u32 owner))
#endif
FAR_CALL(UnitCalcFar, UnitCalc, void, (s32 owner))
FAR_CALL(Item_GetDirectFar, Item_GetDirect, struct ItemDefinition *, (s32 item))
FAR_CALL(Shop_GetSelectionState, Inventory_GetQuantity, s32,
         (s32 owner, s32 slot))
FAR_CALL(Inventory_AddItemFar, Inventory_AddItem, s32,
         (s32 owner_id, s32 item_id))
FAR_CALL(PartyInventory_AddFar, PartyInventory_Add, s32, (s32 item_id))
FAR_CALL(Inventory_FindFar, Inventory_Find, s32, (s32 owner, s32 item_id))
FAR_CALL(PartyInventory_FindOwnerFar, PartyInventory_FindOwner, s32,
         (s32 item_id))
FAR_CALL(PartyInventory_RemoveFar, PartyInventory_Remove, s32, (s32 item_id))
FAR_CALL(Inventory_EquipFar, Inventory_Equip, s32, (s32 owner, s32 slot))
FAR_CALL(Inventory_RemoveFar, Inventory_Remove, s32, (s32 owner, s32 slot))
FAR_CALL(Inventory_BreakFar, Inventory_Break, s32, (s32 owner, s32 slot))
FAR_CALL(Inventory_RepairFar, Inventory_Repair, s32, (s32 owner, s32 slot))
FAR_SLOT(Item_GetTargetMode)
FAR_CALL(Inventory_GetEquippedItemFar, Inventory_GetEquippedItem, s32,
         (struct BattleUnit * inv, s32 type))
FAR_CALL(BattleActGet, BattleAction_GetDirect, struct BattleAct *,
         (s32 actionId))
FAR_CALL(Unit_LearnPsynergyFar, Unit_LearnPsynergy, s32, ())
FAR_CALL(Unit_KnowsActionFar, Unit_KnowsAction, s32, (s32 owner, s32 value))
FAR_CALL(GameState_InitDefaultsFar, GameState_InitDefaults, void, (void))
FAR_SLOT(Unit_CheckLevelUp)
FAR_SLOT(Party_InitCharacters)
FAR_SLOT(Owner_LevelNoOp)
FAR_CALL(Unit_TryLevelUpFar, Unit_TryLevelUp, struct LevelUpResult *,
         (s32 owner_id, struct LevelUpResult *value))
FAR_CALL(GameFlag_TestFar, GameFlag_Test, s32, (s32 flag))
FAR_CALL(GameFlag_SetBitFar, GameFlag_SetBit, s32, (s32 flag))
FAR_CALL(GameFlag_ClearBitFar, GameFlag_ClearBit, void, (s32 flag))
FAR_SLOT(GameFlag_Toggle)
FAR_CALL(GameFlag_GetByteFar, GameFlag_GetByte, s32, (s32 flag))
FAR_CALL(GameFlag_SetByteFar, GameFlag_SetByte, void, (s32 flag, s32 value))
FAR_SLOT(GameFlag_IncrementByte)
FAR_SLOT(GameFlag_DecrementByte)
FAR_SLOT(GameFlag_GetNibble)
FAR_SLOT(GameFlag_SetNibble)
FAR_SLOT(Game_Restart)
FAR_CALL(Unit_AddHpFar, Unit_AddHp, s32, (s32 owner, s32 delta))
FAR_CALL(Unit_AddPpFar, Unit_AddPp, s16, (s32 owner, s32 delta))
FAR_CALL(UnitRatioFar, UnitRatio, void, (s32 owner_id))
FAR_CALL(Unit_SetHpFar, Unit_SetHp, void, (struct BattleUnit * unit, s32 hp))
FAR_SLOT(Unit_SetPp)
FAR_CALL(BattleUnitSetFar, BattleUnitSet, s32,
         (s32 unitId, s32 enemyId, s32 suffix))
FAR_CALL(Party_CountActiveOwnersFar, Party_CountActiveOwners, s32, (void))
FAR_CALL(Party_AddActiveOwnerFar, Party_AddActiveOwner, s32, (s32 value))
FAR_CALL(Party_ListActiveOwnersFar, Party_ListActiveOwners, s32, (s16 *owners))
FAR_CALL(RollWeaponUnleashFar, RollWeaponUnleash, s32,
         (struct BattleUnit * owner))
FAR_CALL(Party_RemoveActiveOwnerFar, Party_RemoveActiveOwner, s32, (s32 value))
FAR_CALL(Item_GetEquippedElementFar, Item_GetEquippedElement, s32, (s32 id))
FAR_CALL(BattleHitCheck, BattleFx_RollSuccess, s32,
         (s32 caster, s32 target, s32 resistance_category, s32 effect_id,
          s32 success_scale))
FAR_CALL(BattleAtkCalc, Battle_CalcAttackDamage, s32,
         (s32 atk, s32 def, s32 add, s32 bonus))
FAR_CALL(BattlePowerCalc, Battle_CalcPsynergyPower, s32,
         (s32 power, s32 bonus, s32 rate))
FAR_CALL(BattleRecoveryCalc, Battle_CalcRecovery, s32,
         (s32 power, s32 element, s32 rate))
FAR_CALL(Enemy_GetDefinitionFar, Enemy_GetDefinition, const u8 *,
         (s32 selector))
FAR_CALL(BattleRandomFar, BattleRandom, s32, (void))
FAR_CALL(Djinn_AddToOwnerFar, Djinn_AddToOwner, s32,
         (s32 owner, s32 index, s32 bit))
FAR_CALL(Djinn_ActivateFar, Djinn_Activate, s32,
         (s32 owner, s32 index, s32 bit))
FAR_CALL(Djinn_DeactivateFar, Djinn_Deactivate, u32,
         (s32 owner, s32 index, s32 bit))
FAR_CALL(DjinnRecovery_RemoveFar, DjinnRecovery_Remove, s32,
         (s32 owner, s32 index, s32 bit))
FAR_CALL(DjinnRecovery_AddStandbyFar, DjinnRecovery_AddStandby, u32 *,
         (u32 kind, u32 first, u32 second))
FAR_CALL(Djinn_TransferFar, Djinn_Transfer, s32,
         (s32 source, s32 index, s32 bit, s32 target))
FAR_CALL(Summon_ListLearnedFar, Summon_ListLearned, s32, (u8 *output))
FAR_CALL(SummonDefinition_Get, SummonDefinition_GetNear,
         const struct SummonDefinition *, (u32 summon_id))
FAR_CALL(Djinn_GetAbilityIdFar, Djinn_GetAbilityId, s32,
         (u32 group, u32 index))
FAR_CALL(Unit_LevelUpToFar, Unit_LevelUpTo, void, (s32 unit, s32 lv))
FAR_CALL(Unit_GetElementLevelFar, Unit_GetElementLevel, s32,
         (s32 owner, s32 index))
FAR_SLOT(DjinnRecovery_CountStandby)
FAR_CALL(Djinn_IsActiveFar, Djinn_IsActive, u32,
         (s32 owner, s32 index, s32 bit))
FAR_CALL(Djinn_IsReadyFar, Djinn_IsReady, s32, (s32 owner, s32 index, s32 bit))
FAR_CALL(Item_CanOwnerEquip, Item_CanOwnerEquipDirect, s32,
         (s32 unit, s32 item))
FAR_CALL(Item_IsCompatibleWithOwnerFar, Item_IsCompatibleWithOwner, s32,
         (s32 unit, s32 item))
FAR_CALL(Inventory_FindEquippedFar, Inventory_FindEquipped, s32,
         (s32 owner, s32 type))
FAR_CALL(Party_AdjustCoinsFar, Party_AdjustCoins, s32, (s32 amount))
FAR_CALL(Item_GetEquipmentGroupFar, Item_GetEquipmentGroup, s32, (s32 item_id))
FAR_CALL(Item_AdjustCounterFar, Item_AdjustCounter, s32,
         (s32 item_id, s32 delta))
FAR_CALL(Inventory_CountFar, Inventory_Count, s32, (s32 owner))
FAR_CALL(PartyInventory_HasSpaceFar, PartyInventory_HasSpace, s32, (void))
FAR_CALL(Unit_GetExperienceForLevelFar, Unit_GetExperienceForLevel, u32,
         (s32 owner, s32 level))
FAR_CALL(Party_RestoreHpPpFar, Party_RestoreHpPp, void, (s32 arg0))
FAR_CALL(Party_RestorePpOfLitElementsFar, Party_RestorePpOfLitElements, void,
         (void))
FAR_CALL(Party_BeginChildhoodFar, Party_BeginChildhood, void, (void))
FAR_CALL(Party_EndChildhoodFar, Party_EndChildhood, void, (void))
FAR_SLOT(Owner_RatioNoOp)
FAR_CALL(Djinn_AddToLeastLoadedOwnerFar, Djinn_AddToLeastLoadedOwner, s32,
         (s32 element, s32 djinn))
FAR_CALL(Party_SumDjinnCountsFar, Party_SumDjinnCounts, s32, (s32 index))
FAR_CALL(Party_AdjustShopCreditFar, Party_AdjustShopCredit, s32, (s32 amount))
FAR_CALL(Party_AdjustShopGiftsFar, Party_AdjustShopGifts, s32, (s32 amount))
FAR_CALL(Inventory_CheckDiscardFar, Inventory_CheckDiscard, s32,
         (s32 owner, s32 slot))
FAR_CALL(Inventory_DiscardFar, Inventory_Discard, s32, (s32 owner, s32 slot))
FAR_CALL(Battle_IsReviveEffectFar, Battle_IsReviveEffect, s32, (s32 effect_id))
FAR_CALL(Unit_RefreshPsynergyFar, Unit_RefreshPsynergy, s32, (s32 owner))
FAR_CALL(Party_GetAverageLevelFar, Party_GetAverageLevel, u32, (void))
FAR_SLOT(Unit_UpdateClass)
FAR_CALL(Inventory_CountItemFar, Inventory_CountItem, s32,
         (s32 owner, s32 item_id))
FAR_CALL(PartyInventory_CountItemFar, PartyInventory_CountItem, s32,
         (s32 item_id))
FAR_CALL(PartyInventory_DiscardFar, PartyInventory_Discard, s32, (s32 item_id))
FAR_CALL(GameFlag_RefreshLureCapFar, GameFlag_RefreshLureCap, void, (void))
FAR_CALL(Equipment_GetUnleashRateBonusFar, Equipment_GetUnleashRateBonus, s32,
         (struct BattleUnit * owner))
FAR_CALL(Runtime_GetBuildStampTimeFar, Runtime_GetBuildStampTime, u32, ())
FAR_CALL(PartyInventory_CountFreeSlotsFar, PartyInventory_CountFreeSlots, s32,
         (void))
FAR_CALL(InventorySnapshot_SaveFar, InventorySnapshot_Save, void, (void))
FAR_CALL(InventorySnapshot_RestoreFar, InventorySnapshot_Restore, void, (void))
