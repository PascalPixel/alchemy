@ Entry veneers opening the OVERLAY_676 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_676 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02002278
	overlay_veneer Func_02000204
	overlay_veneer Func_02000210
	overlay_veneer Func_02000218
	overlay_veneer Func_020004ec
	overlay_veneer Func_0200020c
	overlay_veneer Func_020022b0
