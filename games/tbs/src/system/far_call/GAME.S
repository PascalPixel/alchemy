@ Party members, items, Djinn and game-state operations.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_GameTable
FarCall_GameTable:
	export Djinn_GetRecoveryTableFar, Djinn_GetRecoveryTable
	export UnitGetFar, UnitGet
	export UnitCalcFar, UnitCalc
	export Item_GetDirectFar, Item_GetDirect
	export Shop_GetSelectionState, Inventory_GetQuantity
	export Inventory_AddItemFar, Inventory_AddItem
	export PartyInventory_AddFar, PartyInventory_Add
	export Inventory_FindFar, Inventory_Find
	export PartyInventory_FindOwnerFar, PartyInventory_FindOwner
	export PartyInventory_RemoveFar, PartyInventory_Remove
	export Inventory_EquipFar, Inventory_Equip
	export Inventory_RemoveFar, Inventory_Remove
	export Inventory_BreakFar, Inventory_Break
	export Inventory_RepairFar, Inventory_Repair
	overlay_veneer Item_GetTargetMode
	export Inventory_GetEquippedItemFar, Inventory_GetEquippedItem
	export BattleActGet, BattleAction_GetDirect
	export Unit_LearnPsynergyFar, Unit_LearnPsynergy
	export Unit_KnowsActionFar, Unit_KnowsAction
	export GameState_InitDefaultsFar, GameState_InitDefaults
	overlay_veneer Unit_CheckLevelUp
	overlay_veneer Party_InitCharacters
	overlay_veneer Unit_LevelNoOp
	export Unit_TryLevelUpFar, Unit_TryLevelUp
	export GameFlag_TestFar, GameFlag_Test
	export GameFlag_SetBitFar, GameFlag_SetBit
	export GameFlag_ClearBitFar, GameFlag_ClearBit
	overlay_veneer GameFlag_Toggle
	export GameFlag_GetByteFar, GameFlag_GetByte
	export GameFlag_SetByteFar, GameFlag_SetByte
	overlay_veneer GameFlag_IncrementByte
	overlay_veneer GameFlag_DecrementByte
	overlay_veneer GameFlag_GetNibble
	overlay_veneer GameFlag_SetNibble
	overlay_veneer Game_Restart
	export Unit_AddHpFar, Unit_AddHp
	export Unit_AddPpFar, Unit_AddPp
	export UnitRatioFar, UnitRatio
	export Unit_SetHpFar, Unit_SetHp
	overlay_veneer Unit_SetPp
	export BattleUnitSetFar, BattleUnitSet
	export Party_CountActiveOwnersFar, Party_CountActiveOwners
	export Party_AddActiveOwnerFar, Party_AddActiveOwner
	export Party_ListActiveOwnersFar, Party_ListActiveOwners
	export RollWeaponUnleashFar, RollWeaponUnleash
	export Party_RemoveActiveOwnerFar, Party_RemoveActiveOwner
	export Item_GetEquippedElementFar, Item_GetEquippedElement
	export BattleHitCheck, Battle_RollEffectSuccess
	export BattleAtkCalc, Battle_CalcAttackDamage
	export BattlePowerCalc, Battle_CalcPsynergyPower
	export BattleRecoveryCalc, Battle_CalcRecovery
	export Enemy_GetDefinitionFar, Enemy_GetDefinition
	export BattleRandomFar, BattleRandom
	export Djinn_AddToOwnerFar, Djinn_AddToOwner
	export Djinn_ActivateFar, Djinn_Activate
	export Djinn_DeactivateFar, Djinn_Deactivate
	export DjinnRecovery_RemoveFar, DjinnRecovery_Remove
	export DjinnRecovery_AddStandbyFar, DjinnRecovery_AddStandby
	export Djinn_TransferFar, Djinn_Transfer
	export Summon_ListLearnedFar, Summon_ListLearned
	export SummonDefinition_Get, SummonDefinition_GetNear
	export Djinn_GetAbilityIdFar, Djinn_GetAbilityId
	export Unit_LevelUpToFar, Unit_LevelUpTo
	export Unit_GetElementLevelFar, Unit_GetElementLevel
	overlay_veneer DjinnRecovery_CountStandby
	export Djinn_IsActiveFar, Djinn_IsActive
	export Djinn_IsReadyFar, Djinn_IsReady
	export Item_CanOwnerEquip, Item_CanOwnerEquipDirect
	export Item_IsCompatibleWithOwnerFar, Item_IsCompatibleWithOwner
	export Inventory_FindEquippedFar, Inventory_FindEquipped
	export Party_AdjustCoinsFar, Party_AdjustCoins
	export Item_GetEquipmentGroupFar, Item_GetEquipmentGroup
	export Item_AdjustCounterFar, Item_AdjustCounter
	export Inventory_CountFar, Inventory_Count
	export PartyInventory_HasSpaceFar, PartyInventory_HasSpace
	export Unit_GetExperienceForLevelFar, Unit_GetExperienceForLevel
	export Party_RestoreHpPpFar, Party_RestoreHpPp
	export Party_RestorePpOfLitElementsFar, Party_RestorePpOfLitElements
	export Party_BeginChildhoodFar, Party_BeginChildhood
	export Party_EndChildhoodFar, Party_EndChildhood
	overlay_veneer Unit_RatioNoOp
	export Djinn_AddToLeastLoadedOwnerFar, Djinn_AddToLeastLoadedOwner
	export Party_SumDjinnCountsFar, Party_SumDjinnCounts
	export Party_AdjustShopCreditFar, Party_AdjustShopCredit
	export Party_AdjustShopGiftsFar, Party_AdjustShopGifts
	export Inventory_CheckDiscardFar, Inventory_CheckDiscard
	export Inventory_DiscardFar, Inventory_Discard
	export Battle_IsReviveEffectFar, Battle_IsReviveEffect
	export Unit_RefreshPsynergyFar, Unit_RefreshPsynergy
	export Party_GetAverageLevelFar, Party_GetAverageLevel
	overlay_veneer Unit_UpdateClass
	export Inventory_CountItemFar, Inventory_CountItem
	export PartyInventory_CountItemFar, PartyInventory_CountItem
	export PartyInventory_DiscardFar, PartyInventory_Discard
	export GameFlag_RefreshLureCapFar, GameFlag_RefreshLureCap
	export Equipment_GetUnleashRateBonusFar, Equipment_GetUnleashRateBonus
	export Runtime_GetBuildStampTimeFar, Runtime_GetBuildStampTime
	export PartyInventory_CountFreeSlotsFar, PartyInventory_CountFreeSlots
	export InventorySnapshot_SaveFar, InventorySnapshot_Save
	export InventorySnapshot_RestoreFar, InventorySnapshot_Restore
