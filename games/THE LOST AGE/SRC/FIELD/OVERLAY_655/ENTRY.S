@ Entry veneers opening the OVERLAY_655 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_655 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200304c
	overlay_veneer Func_020011a4
	overlay_veneer Func_020011b0
	overlay_veneer Func_020011b8
	overlay_veneer Func_02001240
	overlay_veneer Func_020011ac
	overlay_veneer Func_02003b34
