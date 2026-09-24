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
	.irp target, 0x080b0445, 0x080b0279, 0x080b29a9, 0x080b3285
	overlay_veneer \target
	.endr
	.global ShopCursor_AdvanceFar
	.global Func_080b0020
	.thumb_func
ShopCursor_AdvanceFar:
Func_080b0020:
	.irp target, 0x080b08b9, 0x080b0959, 0x080b09fd, 0x080b0a21
	overlay_veneer \target
	.endr
	.irp target, 0x080b06c1, 0x080b0841, 0x080b0895, 0x080b3445
	overlay_veneer \target
	.endr
	.irp target, 0x080b04c5, 0x080b3399
	overlay_veneer \target
	.endr
