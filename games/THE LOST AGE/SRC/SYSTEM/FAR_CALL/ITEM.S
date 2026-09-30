@ Far-call stub table at 0x080ad1b0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global Func_080ad1d8
	.thumb_func
Func_080ad1d8:
	overlay_veneer Func_080afeb0
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
