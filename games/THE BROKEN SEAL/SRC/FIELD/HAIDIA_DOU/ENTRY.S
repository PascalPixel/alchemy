@ Entry veneers opening the HAIDIA_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020016f0
	overlay_veneer Func_02000d20
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000d80
	overlay_veneer Func_0200169c
	overlay_veneer Scene_GetRegions
