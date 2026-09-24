@ Far-call stub table heading the menu code at 0x080a1000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global RunAssetSelectionScreenFar
	.global Func_080a1000
	.thumb_func
RunAssetSelectionScreenFar:
Func_080a1000:
	.irp target, 0x080a24d1, 0x080a5b95, 0x080a7479, 0x080a7381
	overlay_veneer \target
	.endr
	.irp target, 0x080a1779
	overlay_veneer \target
	.endr
	.global PsynergyMenu_InitializeEntryObjectsFar
	.global Func_080a1028
	.thumb_func
PsynergyMenu_InitializeEntryObjectsFar:
Func_080a1028:
	.irp target, 0x080a1871, 0x080a195d, 0x080a17c5, 0x080aa56d
	overlay_veneer \target
	.endr
	.irp target, 0x080a4925
	overlay_veneer \target
	.endr
