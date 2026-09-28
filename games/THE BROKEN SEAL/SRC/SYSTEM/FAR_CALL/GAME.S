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
	overlay_veneer Func_08078414
	.global Shop_GetSelectionState
	.thumb_func
Shop_GetSelectionState:
	overlay_veneer Inventory_GetQuantity
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	overlay_veneer Inventory_AddItem
	.global Func_08077030
	.thumb_func
Func_08077030:
	overlay_veneer PartyInventory_Add
	.global Item_FindSlot
	.thumb_func
Item_FindSlot:
	overlay_veneer Inventory_Find
	overlay_veneer PartyInventory_FindOwner
	.global PartyInventory_RemoveFar
	.global Func_08077048
	.thumb_func
PartyInventory_RemoveFar:
Func_08077048:
	overlay_veneer PartyInventory_Remove
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer Inventory_Equip
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer Func_080788c4
	.global Func_08077060
	.thumb_func
Func_08077060:
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
	overlay_veneer Func_08078b9c
	.global OwnerAction_AddFar
	.thumb_func
OwnerAction_AddFar:
	overlay_veneer OwnerAction_Add
	.global Equipment_HasValueFar
	.thumb_func
Equipment_HasValueFar:
	overlay_veneer Equipment_HasValue
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
	overlay_veneer GameFlag_ClearBit
	overlay_veneer GameFlag_Toggle
	.global GameFlag_GetByteFar
	.global Func_080770e0
	.thumb_func
GameFlag_GetByteFar:
Func_080770e0:
	overlay_veneer GameFlag_GetByte
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
	.global Func_08077140
	.thumb_func
Func_08077140:
	overlay_veneer BattleUnit_Assign
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	overlay_veneer Func_080795fc
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Party_AddActiveOwner
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Func_080796c4
	overlay_veneer Func_08079d1c
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer Party_RemoveActiveOwner
	overlay_veneer Item_GetEquippedElement
	.global Func_08077178
	.thumb_func
Func_08077178:
	overlay_veneer BattleFx_RollSuccess
	.global Func_08077180
	.thumb_func
Func_08077180:
	overlay_veneer Owner_ScaleAdjustedAmount
	.global Func_08077188
	.thumb_func
Func_08077188:
	overlay_veneer Owner_ScaleValueByCurve
	.global Func_08077190
	.thumb_func
Func_08077190:
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
	overlay_veneer Djinn_Transfer
	overlay_veneer Trade_ListFlaggedEntries
	.global Func_080771e0
	.thumb_func
Func_080771e0:
	overlay_veneer SummonDefinition_GetNear
	.global Func_080771e8
	.thumb_func
Func_080771e8:
	overlay_veneer Djinn_GetDefinitionHeader
	.global Func_080771f0
	.thumb_func
Func_080771f0:
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
	overlay_veneer Func_0807842c
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	overlay_veneer Func_0807845c
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
	overlay_veneer Item_AdjustCounter
	.global Ability_GetAvailability
	.thumb_func
Ability_GetAvailability:
	.global Inventory_CountFar
	.thumb_func
Inventory_CountFar:
	overlay_veneer Inventory_Count
	overlay_veneer PartyInventory_HasSpace
	.global Owner_GetLevelThresholdFar
	.global Func_08077258
	.thumb_func
Owner_GetLevelThresholdFar:
Func_08077258:
	overlay_veneer Owner_GetLevelThreshold
	overlay_veneer Owner_RefreshActiveRatios
	overlay_veneer Owner_RefreshRatiosOnFlag
	overlay_veneer Party_SetFlag32AndRefreshMembers
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
	overlay_veneer Party_GetAverageLevel
	overlay_veneer Owner_RefreshDerivedData
	.global Inventory_CountItemFar
	.global Func_080772d8
	.thumb_func
Inventory_CountItemFar:
Func_080772d8:
	overlay_veneer Inventory_CountItem
	overlay_veneer PartyInventory_CountItem
	overlay_veneer PartyInventory_Discard
	.global GameFlag_RefreshLureCapFar
	.global Func_080772f0
	.thumb_func
GameFlag_RefreshLureCapFar:
Func_080772f0:
	overlay_veneer GameFlag_RefreshLureCap
	overlay_veneer Equipment_GetUnleashRateBonus
	overlay_veneer Runtime_GetBuildStampTime
	overlay_veneer PartyInventory_CountFreeSlots
	overlay_veneer Func_0807a664
	overlay_veneer InventorySnapshot_Restore