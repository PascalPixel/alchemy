@ Far-call stub table at 0x080ad0c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Owner_AdjustFirstValueFar
	.thumb_func
Owner_AdjustFirstValueFar:
	overlay_veneer Owner_AdjustFirstValue
	.global Owner_AdjustSecondValueFar
	.thumb_func
Owner_AdjustSecondValueFar:
	.thumb_func
	.global Func_080ad0c8
	.thumb_func
Func_080ad0c8:
	overlay_veneer Owner_AdjustSecondValue
	.global Owner_RecalculateRatiosFar
	.thumb_func
Owner_RecalculateRatiosFar:
	overlay_veneer Owner_RecalculateRatios
	.global Owner_UpdateRatioPairFar
	.thumb_func
Owner_UpdateRatioPairFar:
	overlay_veneer Owner_UpdateRatioPair
	.global Owner_UpdateSecondInputAndRatiosFar
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	overlay_veneer Owner_UpdateSecondInputAndRatios
	.global BattleUnit_AssignFar
	.thumb_func
BattleUnit_AssignFar:
	overlay_veneer Func_080afbec
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	overlay_veneer Party_CountActiveOwners
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Func_080afdd8
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Party_ListActiveOwners
	.global Func_080ad108
	.thumb_func
Func_080ad108:
	overlay_veneer Func_080b04bc
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer Func_080afe1c
	.global Func_080ad118
	.thumb_func
Func_080ad118:
	overlay_veneer Func_080b0434
	.global Battle_HitCheck
	.thumb_func
Battle_HitCheck:
	overlay_veneer Func_080b075c
	.global Battle_CalcAttack
	.thumb_func
Battle_CalcAttack:
	overlay_veneer Func_080b03ac
	.global Battle_CalcPower
	.thumb_func
Battle_CalcPower:
	overlay_veneer Func_080b03e0
	.global Battle_CalcRestore
	.thumb_func
Battle_CalcRestore:
	overlay_veneer Func_080b0408
	.global Func_080ad140
	.thumb_func
Func_080ad140:
	overlay_veneer Owner_GetRecord
	.global BattleRandom16Far
	.thumb_func
BattleRandom16Far:
	overlay_veneer BattleRandom16
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
	.global Func_080ad180
	.thumb_func
Func_080ad180:
	overlay_veneer Func_080aff28
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
	.global Func_080ad1a0
	.thumb_func
Func_080ad1a0:
	overlay_veneer Func_080b0028
	.global Trade_CountPendingOffersFar
	.thumb_func
Trade_CountPendingOffersFar:
	overlay_veneer Trade_CountPendingOffers
