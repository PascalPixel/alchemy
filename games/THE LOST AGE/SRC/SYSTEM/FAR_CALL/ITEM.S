@ Far-call stub table at 0x080ad1b0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad1b0
	.thumb_func
Func_080ad1b0:
	overlay_veneer Djinn_IsActive
	overlay_veneer Trade_CanOfferDjinn
	overlay_veneer Item_CanOwnerEquipDirect
	overlay_veneer Item_IsCompatibleWithOwner
	overlay_veneer Inventory_FindEquipped
	.global Func_080ad1d8
	.thumb_func
Func_080ad1d8:
	overlay_veneer Func_080afeb0
	overlay_veneer Item_GetEquipmentGroup
	.global Func_080ad1e8
	.thumb_func
Func_080ad1e8:
	overlay_veneer Item_AdjustCounter
	overlay_veneer Inventory_Count
	.global Func_080ad1f8
	.thumb_func
Func_080ad1f8:
	overlay_veneer PartyInventory_HasSpace
