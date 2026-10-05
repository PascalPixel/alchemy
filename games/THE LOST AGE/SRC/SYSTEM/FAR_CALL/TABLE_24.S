.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Party_FindOwnerWithActionFar
	.thumb_func
Party_FindOwnerWithActionFar:
	overlay_veneer Party_FindOwnerWithAction
	.global Func_080ad2f0
	.thumb_func
Func_080ad2f0:
	overlay_veneer PartyInventory_FindRoom
	.global Func_080ad2f8
	.thumb_func
Func_080ad2f8:
	overlay_veneer Func_080aff94
	.global Djinn_ActivateOfferedForPartyFar
	.thumb_func
Djinn_ActivateOfferedForPartyFar:
	overlay_veneer Djinn_ActivateOfferedForParty
	.global Func_080ad308
	.thumb_func
Func_080ad308:
	overlay_veneer Func_080af0e4
	.global Func_080ad310
	.thumb_func
Func_080ad310:
	overlay_veneer Func_080add5c
