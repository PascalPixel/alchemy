@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_GameTable
FarCall_GameTable:
	.global Trade_GetOfferStateFar
	.thumb_func
Trade_GetOfferStateFar:
	overlay_veneer Trade_GetOfferState
	.global UnitGetFar
	.thumb_func
UnitGetFar:
	overlay_veneer UnitGet
	.global UnitCalcFar
	.thumb_func
UnitCalcFar:
	overlay_veneer UnitCalc
	@ O2 pending: shared code calls this stub Item_Get while overlay veneers of that name target the other label
	.global Item_Get
	.thumb_func
Item_Get:
	.global Item_GetDirectFar
	.thumb_func
Item_GetDirectFar:
	overlay_veneer Item_GetDirect
	.global Shop_GetSelectionState
	.thumb_func
Shop_GetSelectionState:
	overlay_veneer Inventory_GetQuantity
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	overlay_veneer Inventory_AddItem
	.global PartyInventory_AddFar
	.thumb_func
PartyInventory_AddFar:
	overlay_veneer PartyInventory_Add
	.global Inventory_FindFar
	.thumb_func
Inventory_FindFar:
	overlay_veneer Inventory_Find
	.global PartyInventory_FindOwnerFar
	.thumb_func
PartyInventory_FindOwnerFar:
	overlay_veneer PartyInventory_FindOwner
	.global PartyInventory_RemoveFar
	.thumb_func
PartyInventory_RemoveFar:
	overlay_veneer PartyInventory_Remove
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer Inventory_Equip
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer Inventory_Remove
	.global Inventory_BreakFar
	.thumb_func
Inventory_BreakFar:
	overlay_veneer Inventory_Break
	.global Inventory_RepairFar
	.thumb_func
Inventory_RepairFar:
	overlay_veneer Inventory_Repair
	overlay_veneer Item_GetTargetMode
	.global Inventory_GetEquippedItemFar
	.thumb_func
Inventory_GetEquippedItemFar:
	overlay_veneer Inventory_GetEquippedItem
	.global BtlActGet
	.thumb_func
BtlActGet:
	overlay_veneer BattleAction_GetDirect
	.global OwnerAction_AddFar
	.thumb_func
OwnerAction_AddFar:
	overlay_veneer OwnerAction_Add
	.global Equipment_HasValueFar
	.thumb_func
Equipment_HasValueFar:
	overlay_veneer Equipment_HasValue
	.global GameState_InitDefaultsFar
	.thumb_func
GameState_InitDefaultsFar:
	overlay_veneer GameState_InitDefaults
	overlay_veneer OwnerAction_CheckLevelThreshold
	overlay_veneer Owner_InitRecords
	overlay_veneer Owner_LevelNoOp
	.global Owner_GetValueIfLevelThresholdReachedFar
	.thumb_func
Owner_GetValueIfLevelThresholdReachedFar:
	overlay_veneer Owner_GetValueIfLevelThresholdReached
	.global GameFlag_TestFar
	.thumb_func
GameFlag_TestFar:
	overlay_veneer GameFlag_Test
	.global GameFlag_SetBitFar
	.thumb_func
GameFlag_SetBitFar:
	overlay_veneer GameFlag_SetBit
	.global GameFlag_ClearBitFar
	.thumb_func
GameFlag_ClearBitFar:
	overlay_veneer GameFlag_ClearBit
	overlay_veneer GameFlag_Toggle
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer GameFlag_SetByte
	overlay_veneer GameFlag_IncrementByte
	overlay_veneer GameFlag_DecrementByte
	overlay_veneer GameFlag_GetNibble
	overlay_veneer GameFlag_SetNibble
	overlay_veneer Owner_RefreshAndResetZero
	.global Owner_AdjustFirstValueFar
	.thumb_func
Owner_AdjustFirstValueFar:
	overlay_veneer Owner_AdjustFirstValue
	.global UnitEpAddFar
	.thumb_func
UnitEpAddFar:
	overlay_veneer UnitEpAdd
	.global UnitRatioFar
	.thumb_func
UnitRatioFar:
	overlay_veneer UnitRatio
	.global Owner_UpdateRatioPairFar
	.thumb_func
Owner_UpdateRatioPairFar:
	overlay_veneer Owner_UpdateRatioPair
	overlay_veneer Owner_UpdateSecondInputAndRatios
	.global BtlUnitSetFar
	.thumb_func
BtlUnitSetFar:
	overlay_veneer BtlUnitSet
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	overlay_veneer Party_CountActiveOwners
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Party_AddActiveOwner
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Party_ListActiveOwners
	.global RollWeaponUnleashFar
	.thumb_func
RollWeaponUnleashFar:
	overlay_veneer RollWeaponUnleash
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer Party_RemoveActiveOwner
	.global Item_GetEquippedElementFar
	.thumb_func
Item_GetEquippedElementFar:
	overlay_veneer Item_GetEquippedElement
	.global BtlHitChk
	.thumb_func
BtlHitChk:
	overlay_veneer BattleFx_RollSuccess
	.global BtlAtkCalc
	.thumb_func
BtlAtkCalc:
	overlay_veneer Owner_ScaleAdjustedAmount
	.global BtlPowCalc
	.thumb_func
BtlPowCalc:
	overlay_veneer Owner_ScaleValueByCurve
	.global BtlRcvCalc
	.thumb_func
BtlRcvCalc:
	overlay_veneer Owner_ScaleValueByOffsetCurve
	.global Owner_GetRecordFar
	.thumb_func
Owner_GetRecordFar:
	overlay_veneer Owner_GetRecord
	.global BtlRndFar
	.thumb_func
BtlRndFar:
	overlay_veneer BtlRnd
	.global Djinn_AddToOwnerFar
	.thumb_func
Djinn_AddToOwnerFar:
	overlay_veneer Djinn_AddToOwner
	.global Djinn_ActivateFar
	.thumb_func
Djinn_ActivateFar:
	overlay_veneer Djinn_Activate
	.global Djinn_DeactivateFar
	.thumb_func
Djinn_DeactivateFar:
	overlay_veneer Djinn_Deactivate
	.global Trade_RemoveOfferFar
	.thumb_func
Trade_RemoveOfferFar:
	overlay_veneer Trade_RemoveOffer
	.global Trade_AddOfferFar
	.thumb_func
Trade_AddOfferFar:
	overlay_veneer Trade_AddOffer
	.global Djinn_TransferFar
	.thumb_func
Djinn_TransferFar:
	overlay_veneer Djinn_Transfer
	.global Trade_ListFlaggedEntriesFar
	.thumb_func
Trade_ListFlaggedEntriesFar:
	overlay_veneer Trade_ListFlaggedEntries
	.global SummonDefinition_Get
	.thumb_func
SummonDefinition_Get:
	overlay_veneer SummonDefinition_GetNear
	.global Djinn_GetDefinitionHeaderFar
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	overlay_veneer Djinn_GetDefinitionHeader
	.global Party_AdvanceOwnerCountToTargetFar
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	overlay_veneer Party_AdvanceOwnerCountToTarget
	.global Owner_GetResistanceValueFar
	.thumb_func
Owner_GetResistanceValueFar:
	overlay_veneer Owner_GetResistanceValue
	overlay_veneer Trade_CountPendingOffers
	.global Djinn_IsActiveFar
	.thumb_func
Djinn_IsActiveFar:
	overlay_veneer Djinn_IsActive
	.global Trade_CanOfferDjinnFar
	.thumb_func
Trade_CanOfferDjinnFar:
	overlay_veneer Trade_CanOfferDjinn
	.global Item_CanOwnerEquip
	.thumb_func
Item_CanOwnerEquip:
	overlay_veneer Item_CanOwnerEquipDirect
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	overlay_veneer Item_IsCompatibleWithOwner
	.global Inventory_FindEquippedFar
	.thumb_func
Inventory_FindEquippedFar:
	overlay_veneer Inventory_FindEquipped
	.global Party_AdjustCoinsFar
	.thumb_func
Party_AdjustCoinsFar:
	overlay_veneer Party_AdjustCoins
	.global Item_GetEquipmentGroupFar
	.thumb_func
Item_GetEquipmentGroupFar:
	overlay_veneer Item_GetEquipmentGroup
	.global Item_AdjustCounterFar
	.thumb_func
Item_AdjustCounterFar:
	overlay_veneer Item_AdjustCounter
	.global Inventory_CountFar
	.thumb_func
Inventory_CountFar:
	overlay_veneer Inventory_Count
	.global PartyInventory_HasSpaceFar
	.thumb_func
PartyInventory_HasSpaceFar:
	overlay_veneer PartyInventory_HasSpace
	.global Owner_GetLevelThresholdFar
	.thumb_func
Owner_GetLevelThresholdFar:
	overlay_veneer Owner_GetLevelThreshold
	.global Owner_RefreshActiveRatiosFar
	.thumb_func
Owner_RefreshActiveRatiosFar:
	overlay_veneer Owner_RefreshActiveRatios
	.global Owner_RefreshRatiosOnFlagFar
	.thumb_func
Owner_RefreshRatiosOnFlagFar:
	overlay_veneer Owner_RefreshRatiosOnFlag
	.global Party_SetFlag32AndRefreshMembersFar
	.thumb_func
Party_SetFlag32AndRefreshMembersFar:
	overlay_veneer Party_SetFlag32AndRefreshMembers
	.global Party_ApplyStatePresetFar
	.thumb_func
Party_ApplyStatePresetFar:
	overlay_veneer Party_ApplyStatePreset
	overlay_veneer Owner_RatioNoOp
	.global Djinn_AddToLeastLoadedOwnerFar
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	overlay_veneer Djinn_AddToLeastLoadedOwner
	.global Party_SumDjinnCountsFar
	.thumb_func
Party_SumDjinnCountsFar:
	overlay_veneer Party_SumDjinnCounts
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	overlay_veneer Party_AdjustSixDigitCounterB
	.global Party_AdjustCounterCappedAt28Far
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	overlay_veneer Party_AdjustCounterCappedAt28
	.global Inventory_CheckDiscardFar
	.thumb_func
Inventory_CheckDiscardFar:
	overlay_veneer Inventory_CheckDiscard
	.global Inventory_DiscardFar
	.thumb_func
Inventory_DiscardFar:
	overlay_veneer Inventory_Discard
	.global BtlEfxRevChkFar
	.thumb_func
BtlEfxRevChkFar:
	overlay_veneer BtlEfxRevChk
	.global Owner_RefreshClassActionsFar
	.thumb_func
Owner_RefreshClassActionsFar:
	overlay_veneer Owner_RefreshClassActions
	.global Party_GetAverageLevelFar
	.thumb_func
Party_GetAverageLevelFar:
	overlay_veneer Party_GetAverageLevel
	overlay_veneer Owner_RefreshDerivedData
	.global Inventory_CountItemFar
	.thumb_func
Inventory_CountItemFar:
	overlay_veneer Inventory_CountItem
	.global PartyInventory_CountItemFar
	.thumb_func
PartyInventory_CountItemFar:
	overlay_veneer PartyInventory_CountItem
	.global PartyInventory_DiscardFar
	.thumb_func
PartyInventory_DiscardFar:
	overlay_veneer PartyInventory_Discard
	.global GameFlag_RefreshLureCapFar
	.thumb_func
GameFlag_RefreshLureCapFar:
	overlay_veneer GameFlag_RefreshLureCap
	.global Equipment_GetUnleashRateBonusFar
	.thumb_func
Equipment_GetUnleashRateBonusFar:
	overlay_veneer Equipment_GetUnleashRateBonus
	.global Runtime_GetBuildStampTimeFar
	.thumb_func
Runtime_GetBuildStampTimeFar:
	overlay_veneer Runtime_GetBuildStampTime
	.global PartyInventory_CountFreeSlotsFar
	.thumb_func
PartyInventory_CountFreeSlotsFar:
	overlay_veneer PartyInventory_CountFreeSlots
	.global InventorySnapshot_SaveFar
	.thumb_func
InventorySnapshot_SaveFar:
	overlay_veneer InventorySnapshot_Save
	.global InventorySnapshot_RestoreFar
	.thumb_func
InventorySnapshot_RestoreFar:
	overlay_veneer InventorySnapshot_Restore
