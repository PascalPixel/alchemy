@ Far-call stub table heading the shop and inn code at 0x080b0000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080b0000
	.thumb_func
Func_080b0000:
	overlay_veneer 0x080b0445
	overlay_veneer 0x080b0279
	overlay_veneer 0x080b29a9
	overlay_veneer 0x080b3285
	.global ShopCursor_AdvanceFar
	.global Func_080b0020
	.thumb_func
ShopCursor_AdvanceFar:
Func_080b0020:
	overlay_veneer 0x080b08b9
	.global ShopCursor_MoveTowardTargetFar
	.thumb_func
ShopCursor_MoveTowardTargetFar:
	overlay_veneer 0x080b0959
	.global Shop_SetCursorFar
	.thumb_func
Shop_SetCursorFar:
	overlay_veneer 0x080b09fd
	.global Func_080b0038
	.thumb_func
Func_080b0038:
	.global ShopCursor_SetPositionImmediateFar
	.thumb_func
ShopCursor_SetPositionImmediateFar:
	overlay_veneer 0x080b0a21
	overlay_veneer 0x080b06c1
	.global Unnamed_080b0840Far
	.thumb_func
Unnamed_080b0840Far:
	overlay_veneer 0x080b0841
	.global Shop_InitEffectFar
	.thumb_func
Shop_InitEffectFar:
	overlay_veneer 0x080b0895
	overlay_veneer 0x080b3445
	overlay_veneer 0x080b04c5
	overlay_veneer 0x080b3399
