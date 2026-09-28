@ Entry veneers opening the OVERLAY_6AE overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6AE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000314
	overlay_veneer Func_02000150
	overlay_veneer Func_0200015c
	overlay_veneer Func_02000164
	overlay_veneer Func_020001d0
	overlay_veneer Func_02000158
	overlay_veneer Func_02000450
