@ Entry veneers opening the OVERLAY_674 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_674 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02002b64
	overlay_veneer Func_02000038
	overlay_veneer Func_02000044
	overlay_veneer Func_0200004c
	overlay_veneer Func_0200019c
	overlay_veneer Func_02000040
	overlay_veneer Func_0200340c
