@ Entry veneers opening the OVERLAY_692 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_692 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003678
	overlay_veneer Func_020008e4
	overlay_veneer Func_020008f0
	overlay_veneer Func_020008f8
	overlay_veneer Func_02003418
	overlay_veneer Func_020008ec
	overlay_veneer Func_02003ee0
