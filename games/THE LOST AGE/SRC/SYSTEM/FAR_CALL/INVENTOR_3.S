@ Far-call stub table at 0x080ad070: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Inventory_GetEquippedItemFar
	.thumb_func
Inventory_GetEquippedItemFar:
	overlay_veneer Inventory_GetEquippedItem
	.global BattleAction_Get
	.thumb_func
BattleAction_Get:
	overlay_veneer Func_080af43c
	overlay_veneer Func_080af6f4
	.global Equipment_HasValueFar
	.thumb_func
Equipment_HasValueFar:
	overlay_veneer Func_080af4b8
	.global Func_080ad090
	.thumb_func
Func_080ad090:
	overlay_veneer Func_080addf0
	overlay_veneer Func_080af794
	overlay_veneer Func_080af7ac
	overlay_veneer Func_080af8cc
