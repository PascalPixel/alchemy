@ Entry veneers opening the RUNPA_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — RUNPA_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001668
	overlay_veneer Func_02000314
	overlay_veneer Func_02000350
	overlay_veneer Func_02000388
	overlay_veneer Func_020006e4
	overlay_veneer Scene_GetRegions
