@ Far-call stub table at 0x080ad2b8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad2b8
	.thumb_func
Func_080ad2b8:
	overlay_veneer Func_080adbec
	.global Equipment_GetUnleashRateBonusFar
	.thumb_func
Equipment_GetUnleashRateBonusFar:
	overlay_veneer Equipment_GetUnleashRateBonus
	.global Func_080ad2c8
	.thumb_func
Func_080ad2c8:
	overlay_veneer Func_080add74
	.global PartyInventory_CountFreeSlotsFar
	.thumb_func
PartyInventory_CountFreeSlotsFar:
	overlay_veneer PartyInventory_CountFreeSlots
	.global Func_080ad2d8
	.thumb_func
Func_080ad2d8:
	overlay_veneer Func_080b1088
	.global Func_080ad2e0
	.thumb_func
Func_080ad2e0:
	overlay_veneer Func_080b11f8
