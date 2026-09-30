@ Far-call stub table at 0x080ad248: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad248
	.thumb_func
Func_080ad248:
	overlay_veneer Djinn_AddToLeastLoadedOwner
	overlay_veneer Party_SumDjinnCounts
	overlay_veneer Func_080afed4
	overlay_veneer Func_080aff00
	overlay_veneer Func_080af244
	overlay_veneer Func_080af1fc
	.global Func_080ad278
	.thumb_func
Func_080ad278:
	overlay_veneer Inventory_Discard
	.global BattleFx_IsReviveFar
	.thumb_func
BattleFx_IsReviveFar:
	overlay_veneer Func_080b0740
	.global Func_080ad288
	.thumb_func
Func_080ad288:
	overlay_veneer Func_080b02d4
	.global Func_080ad290
	.thumb_func
Func_080ad290:
	overlay_veneer Func_080ad360
	.global Func_080ad298
	.thumb_func
Func_080ad298:
	overlay_veneer Func_080b0298
	.global Func_080ad2a0
	.thumb_func
Func_080ad2a0:
	overlay_veneer Inventory_CountItem
	.global Func_080ad2a8
	.thumb_func
Func_080ad2a8:
	overlay_veneer PartyInventory_CountItem
	.global Func_080ad2b0
	.thumb_func
Func_080ad2b0:
	overlay_veneer Func_080af2c0
