@ Entry veneers opening the OVERLAY_6A8 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6A8 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02002614
	overlay_veneer Func_02000478
	overlay_veneer Func_02000484
	overlay_veneer Func_0200048c
	overlay_veneer Func_02000598
	overlay_veneer Func_02000480
	overlay_veneer Func_02002798
