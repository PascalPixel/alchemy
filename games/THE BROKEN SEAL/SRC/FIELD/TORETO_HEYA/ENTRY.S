@ Entry veneers opening the TORETO_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TORETO_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001004
	overlay_veneer Func_02000314
	overlay_veneer Func_02000324
	overlay_veneer Func_0200032c
	overlay_veneer Func_02000694
	overlay_veneer Func_0200031c
