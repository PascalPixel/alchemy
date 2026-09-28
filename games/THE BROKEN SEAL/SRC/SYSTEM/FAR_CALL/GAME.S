@ Far-call stub table heading the party, item and flag code at 0x08077000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Trade_GetOfferStateFar
	.global Func_08077000
	.thumb_func
Trade_GetOfferStateFar:
Func_08077000:
	overlay_veneer Trade_GetOfferState
	.global Func_08077008
	.thumb_func
Func_08077008:
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	.global Runtime_GetObject
	.thumb_func
Runtime_GetObject:
	overlay_veneer Owner_GetState
	.global BattleUnit_Recalculate
	.thumb_func
BattleUnit_Recalculate:
	.global Func_08077010
	.thumb_func
Func_08077010:
	.global Owner_RecalculateStatsFar
	.thumb_func
Owner_RecalculateStatsFar:
	overlay_veneer Owner_RecalculateStats
	.global Func_08077018
	.thumb_func
Func_08077018:
	.global Item_Get
	.thumb_func
Item_Get:
	overlay_veneer Item_GetDirect
	.global Shop_GetSelectionState
	.thumb_func
Shop_GetSelectionState:
	overlay_veneer Inventory_GetQuantity
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	.global Func_08077028
	.thumb_func
Func_08077028:
	overlay_veneer Inventory_AddItem
	.global Func_08077030
	.thumb_func
Func_08077030:
	.global PartyInventory_AddFar
	.thumb_func
PartyInventory_AddFar:
	overlay_veneer PartyInventory_Add
	.global Item_FindSlot
	.thumb_func
Item_FindSlot:
	.global Inventory_FindFar
	.thumb_func
Inventory_FindFar:
	overlay_veneer Inventory_Find
	.global PartyInventory_FindOwnerFar
	.thumb_func
PartyInventory_FindOwnerFar:
	.global Func_08077040
	.thumb_func
Func_08077040:
	overlay_veneer PartyInventory_FindOwner
	.global PartyInventory_RemoveFar
	.global Func_08077048
	.thumb_func
PartyInventory_RemoveFar:
	.thumb_func
Func_08077048:
	overlay_veneer PartyInventory_Remove
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	.global Func_08077050
	.thumb_func
Func_08077050:
	overlay_veneer Inventory_Equip
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer Inventory_Remove
	.global Inventory_BreakFar
	.thumb_func
Inventory_BreakFar:
	overlay_veneer Inventory_Break
	.global Func_08077068
	.thumb_func
Func_08077068:
	overlay_veneer Inventory_Repair
	overlay_veneer Item_GetTargetMode
	.global Inventory_GetEquippedItemFar
	.global Func_08077078
	.thumb_func
Inventory_GetEquippedItemFar:
Func_08077078:
	overlay_veneer Inventory_GetEquippedItem
	.global Ability_GetData
	.thumb_func
Ability_GetData:
	.global BattleAction_Get
	.thumb_func
BattleAction_Get:
	.global Func_08077080
	.thumb_func
Func_08077080:
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
	.global Func_080770b8
	.thumb_func
Func_080770b8:
	overlay_veneer Owner_GetValueIfLevelThresholdReached
	.global Func_080770c0
	.thumb_func
Func_080770c0:
	.global GameFlag_IsSet
	.thumb_func
GameFlag_IsSet:
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
	.global Func_080770d0
	.thumb_func
Func_080770d0:
	overlay_veneer GameFlag_ClearBit
	overlay_veneer GameFlag_Toggle
	.global GameFlag_GetByteFar
	.global Func_080770e0
	.thumb_func
GameFlag_GetByteFar:
	.thumb_func
Func_080770e0:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	.global Func_080770e8
	.thumb_func
Func_080770e8:
	overlay_veneer GameFlag_SetByte
	overlay_veneer GameFlag_IncrementByte
	overlay_veneer GameFlag_DecrementByte
	overlay_veneer GameFlag_GetNibble
	overlay_veneer GameFlag_SetNibble
	overlay_veneer Owner_RefreshAndResetZero
	.global Owner_AdjustFirstValueFar
	.global Func_08077118
	.thumb_func
Owner_AdjustFirstValueFar:
Func_08077118:
	overlay_veneer Owner_AdjustFirstValue
	.global Func_08077120
	.thumb_func
Func_08077120:
	.global Owner_AdjustSecondValueFar
	.thumb_func
Owner_AdjustSecondValueFar:
	overlay_veneer Owner_AdjustSecondValue
	.global Func_08077128
	.thumb_func
Func_08077128:
	.global Owner_RecalculateRatiosFar
	.thumb_func
Owner_RecalculateRatiosFar:
	overlay_veneer Owner_RecalculateRatios
	.global Owner_UpdateRatioPairFar
	.thumb_func
Owner_UpdateRatioPairFar:
	overlay_veneer Owner_UpdateRatioPair
	overlay_veneer Owner_UpdateSecondInputAndRatios
	.global BattleUnit_AssignFar
	.thumb_func
BattleUnit_AssignFar:
	overlay_veneer BattleUnit_Assign
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	.global Func_08077148
	.thumb_func
Func_08077148:
	overlay_veneer Party_CountActiveOwners
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Party_AddActiveOwner
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Party_ListActiveOwners
	overlay_veneer RollWeaponUnleash
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer Party_RemoveActiveOwner
	overlay_veneer Item_GetEquippedElement
	.global Battle_HitCheck
	.thumb_func
Battle_HitCheck:
	overlay_veneer BattleFx_RollSuccess
	.global Battle_CalcAttack
	.thumb_func
Battle_CalcAttack:
	overlay_veneer Owner_ScaleAdjustedAmount
	.global Battle_CalcPower
	.thumb_func
Battle_CalcPower:
	overlay_veneer Owner_ScaleValueByCurve
	.global Battle_CalcRestore
	.thumb_func
Battle_CalcRestore:
	overlay_veneer Owner_ScaleValueByOffsetCurve
	.global Owner_GetRecordFar
	.thumb_func
Owner_GetRecordFar:
	overlay_veneer Owner_GetRecord
	.global BattleRandom16Far
	.thumb_func
BattleRandom16Far:
	.global Func_080771a0
	.thumb_func
Func_080771a0:
	overlay_veneer BattleRandom16
	.global Djinn_AddToOwnerFar
	.thumb_func
Djinn_AddToOwnerFar:
	overlay_veneer Djinn_AddToOwner
	.global Djinn_ActivateFar
	.thumb_func
Djinn_ActivateFar:
	.global Func_080771b0
	.thumb_func
Func_080771b0:
	overlay_veneer Djinn_Activate
	.global Func_080771b8
	.thumb_func
Func_080771b8:
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
	overlay_veneer Trade_ListFlaggedEntries
	.global SummonDefinition_Get
	.thumb_func
SummonDefinition_Get:
	overlay_veneer SummonDefinition_GetNear
	.global Func_080771e8
	.thumb_func
Func_080771e8:
	overlay_veneer Djinn_GetDefinitionHeader
	.global Func_080771f0
	.thumb_func
Func_080771f0:
	.global Party_AdvanceOwnerCountToTargetFar
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	overlay_veneer Party_AdvanceOwnerCountToTarget
	overlay_veneer Owner_GetResistanceValue
	overlay_veneer Trade_CountPendingOffers
	.global Func_08077208
	.thumb_func
Func_08077208:
	overlay_veneer Djinn_IsActive
	.global Trade_CanOfferDjinnFar
	.thumb_func
Trade_CanOfferDjinnFar:
	overlay_veneer Trade_CanOfferDjinn
	.global Func_08077218
	.thumb_func
Func_08077218:
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
	.global Party_AdjustSixDigitCounterAFar
	.thumb_func
Party_AdjustSixDigitCounterAFar:
	overlay_veneer Party_AdjustSixDigitCounterA
	.global Item_GetEquipmentGroupFar
	.thumb_func
Item_GetEquipmentGroupFar:
	overlay_veneer Item_GetEquipmentGroup
	.global Ability_GetMaximum
	.thumb_func
Ability_GetMaximum:
	.global Func_08077240
	.thumb_func
Func_08077240:
	.global Item_AdjustCounterFar
	.thumb_func
Item_AdjustCounterFar:
	overlay_veneer Item_AdjustCounter
	.global Ability_GetAvailability
	.thumb_func
Ability_GetAvailability:
	.global Inventory_CountFar
	.thumb_func
Inventory_CountFar:
	overlay_veneer Inventory_Count
	.global PartyInventory_HasSpaceFar
	.thumb_func
PartyInventory_HasSpaceFar:
	overlay_veneer PartyInventory_HasSpace
	.global Owner_GetLevelThresholdFar
	.global Func_08077258
	.thumb_func
Owner_GetLevelThresholdFar:
Func_08077258:
	overlay_veneer Owner_GetLevelThreshold
	.global Owner_RefreshActiveRatiosFar
	.thumb_func
Owner_RefreshActiveRatiosFar:
	overlay_veneer Owner_RefreshActiveRatios
	.global Owner_RefreshRatiosOnFlagFar
	.thumb_func
Owner_RefreshRatiosOnFlagFar:
	.global Func_08077268
	.thumb_func
Func_08077268:
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
	.global Func_08077288
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
Func_08077288:
	overlay_veneer Djinn_AddToLeastLoadedOwner
	.global Party_SumDjinnCountsFar
	.thumb_func
Party_SumDjinnCountsFar:
	overlay_veneer Party_SumDjinnCounts
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	overlay_veneer Party_AdjustSixDigitCounterB
	.global Func_080772a0
	.thumb_func
Func_080772a0:
	overlay_veneer Party_AdjustCounterCappedAt28
	.global Inventory_CheckDiscardFar
	.thumb_func
Inventory_CheckDiscardFar:
	overlay_veneer Inventory_CheckDiscard
	.global Func_080772b0
	.thumb_func
Func_080772b0:
	.global Inventory_DiscardFar
	.thumb_func
Inventory_DiscardFar:
	overlay_veneer Inventory_Discard
	.global BattleFx_IsReviveFar
	.thumb_func
BattleFx_IsReviveFar:
	.global Func_080772b8
	.thumb_func
Func_080772b8:
	overlay_veneer BattleFx_IsRevive
	.global Func_080772c0
	.thumb_func
Func_080772c0:
	overlay_veneer Owner_RefreshClassActions
	.global Party_GetAverageLevelFar
	.thumb_func
Party_GetAverageLevelFar:
	overlay_veneer Party_GetAverageLevel
	overlay_veneer Owner_RefreshDerivedData
	.global Inventory_CountItemFar
	.global Func_080772d8
	.thumb_func
Inventory_CountItemFar:
Func_080772d8:
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
	.global Func_080772f0
	.thumb_func
GameFlag_RefreshLureCapFar:
Func_080772f0:
	overlay_veneer GameFlag_RefreshLureCap
	overlay_veneer Equipment_GetUnleashRateBonus
	.global Runtime_GetBuildStampTimeFar
	.thumb_func
Runtime_GetBuildStampTimeFar:
	overlay_veneer Runtime_GetBuildStampTime
	.global PartyInventory_CountFreeSlotsFar
	.thumb_func
PartyInventory_CountFreeSlotsFar:
	overlay_veneer PartyInventory_CountFreeSlots
	.global Func_08077310
	.thumb_func
Func_08077310:
	overlay_veneer Func_0807a664
	.global InventorySnapshot_RestoreFar
	.thumb_func
InventorySnapshot_RestoreFar:
	overlay_veneer InventorySnapshot_Restore