@ Entry veneers opening the OVERLAY_688 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_688 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000190
	overlay_veneer Func_02000044
	overlay_veneer Func_02000050
	overlay_veneer Func_02000058
	overlay_veneer Func_02000118
	overlay_veneer Func_0200004c
	overlay_veneer Func_0200030c
