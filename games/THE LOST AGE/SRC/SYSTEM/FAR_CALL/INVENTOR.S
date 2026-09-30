@ Far-call stub table at 0x080ad000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall005
Resource_FarCall005:
	.global Trade_GetOfferStateFar
	.thumb_func
Trade_GetOfferStateFar:
	overlay_veneer Trade_GetOfferState
	.global Owner_RecalculateStatsFar
	.thumb_func
Owner_RecalculateStatsFar:
	.global BattleUnit_Recalculate
	.thumb_func
BattleUnit_Recalculate:
	overlay_veneer Owner_RecalculateStats
	.global Item_Get
	.thumb_func
Item_Get:
	overlay_veneer Item_GetDirect
	.global Inventory_GetQuantityFar
	.thumb_func
Inventory_GetQuantityFar:
	overlay_veneer Inventory_GetQuantity
	.global Inventory_AddFar
	.thumb_func
Inventory_AddFar:
	overlay_veneer Inventory_Add
	.global PartyInventory_AddFar
	.thumb_func
PartyInventory_AddFar:
	overlay_veneer PartyInventory_Add
	overlay_veneer Inventory_Find
	.global PartyInventory_FindOwnerFar
	.thumb_func
PartyInventory_FindOwnerFar:
	overlay_veneer PartyInventory_FindOwner
