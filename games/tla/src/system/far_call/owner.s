.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Owner_AdjustFirstValueFar
	.thumb_func
Owner_AdjustFirstValueFar:
	overlay_veneer Owner_AdjustFirstValue
	.global UnitPpAddFar
	.thumb_func
UnitPpAddFar:
	overlay_veneer UnitPpAdd
	.global UnitRatioFar
	.thumb_func
UnitRatioFar:
	overlay_veneer UnitRatio
	.global Owner_UpdateRatioPairFar
	.thumb_func
Owner_UpdateRatioPairFar:
	overlay_veneer Owner_UpdateRatioPair
	.global Owner_UpdateSecondInputAndRatiosFar
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	overlay_veneer Owner_UpdateSecondInputAndRatios
	.global BattleUnitSetFar
	.thumb_func
BattleUnitSetFar:
	overlay_veneer Func_080afbec
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
	.global BattleHitCheck
	.thumb_func
BattleHitCheck:
	overlay_veneer Func_080b075c
	.global BattleAtkCalc
	.thumb_func
BattleAtkCalc:
	overlay_veneer Owner_ScaleAdjustedAmount
	.global BattlePowerCalc
	.thumb_func
BattlePowerCalc:
	overlay_veneer Owner_ScaleValueByCurve
	.global BattleRecoveryCalc
	.thumb_func
BattleRecoveryCalc:
	overlay_veneer Owner_ScaleValueByOffsetCurve
	.global Owner_GetRecordFar
	.thumb_func
Owner_GetRecordFar:
	overlay_veneer Owner_GetRecord
	.global BattleRandomFar
	.thumb_func
BattleRandomFar:
	overlay_veneer BattleRandom
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
	.global Trade_CountPendingOffersFar
	.thumb_func
Trade_CountPendingOffersFar:
	overlay_veneer Trade_CountPendingOffers
