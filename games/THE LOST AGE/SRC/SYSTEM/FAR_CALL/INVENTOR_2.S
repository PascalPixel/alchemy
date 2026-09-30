@ Far-call stub table at 0x080ad040: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global PartyInventory_RemoveFar
	.thumb_func
PartyInventory_RemoveFar:
	overlay_veneer PartyInventory_Remove
	.global Func_080ad048
	.thumb_func
Func_080ad048:
	overlay_veneer Func_080aef34
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer Inventory_Remove
	.global Inventory_BreakFar
	.thumb_func
Inventory_BreakFar:
	overlay_veneer Inventory_Break
	overlay_veneer Inventory_Repair
	overlay_veneer Func_080af338
