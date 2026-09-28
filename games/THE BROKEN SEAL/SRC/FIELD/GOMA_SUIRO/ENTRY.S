@ Entry veneers opening the GOMA_SUIRO overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — GOMA_SUIRO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000414
	overlay_veneer Func_02000314
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000350
	overlay_veneer Func_020003e4
	overlay_veneer Scene_GetRegions
