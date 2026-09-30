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
	overlay_veneer Owner_AdjustSecondValue
	.global Owner_RecalculateRatiosFar
	.thumb_func
Owner_RecalculateRatiosFar:
	overlay_veneer Owner_RecalculateRatios
	overlay_veneer Owner_UpdateRatioPair
	overlay_veneer Owner_UpdateSecondInputAndRatios
	.global BattleUnit_AssignFar
	.thumb_func
BattleUnit_AssignFar:
	overlay_veneer Func_080afbec
	.global Func_080ad0f0
	.thumb_func
Func_080ad0f0:
	overlay_veneer Party_CountActiveOwners
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Func_080afdd8
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Party_ListActiveOwners
	overlay_veneer Func_080b04bc
	.global Func_080ad110
	.thumb_func
Func_080ad110:
	overlay_veneer Func_080afe1c
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
	overlay_veneer Func_080ad3a8
	.global BattleRandom16Far
	.thumb_func
BattleRandom16Far:
	overlay_veneer Func_080b0378
	.global Djinn_AddToOwnerFar
	.thumb_func
Djinn_AddToOwnerFar:
	overlay_veneer Djinn_AddToOwner
	.global Djinn_ActivateFar
	.thumb_func
Djinn_ActivateFar:
	overlay_veneer Djinn_Activate
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
	overlay_veneer Func_080aff28
	overlay_veneer SummonDefinition_GetNear
	overlay_veneer Djinn_GetDefinitionHeader
	.global Party_AdvanceOwnerCountToTargetFar
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	overlay_veneer Party_AdvanceOwnerCountToTarget
	overlay_veneer Func_080b0028
	overlay_veneer Trade_CountPendingOffers
