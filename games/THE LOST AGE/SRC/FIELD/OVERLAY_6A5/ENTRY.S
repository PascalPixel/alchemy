@ Entry veneers opening the OVERLAY_6A5 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6A5 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003d9c
	overlay_veneer Func_0200005c
	overlay_veneer Func_02000068
	overlay_veneer Func_02000070
	overlay_veneer Func_02003a20
	overlay_veneer Func_02000064
	overlay_veneer Func_02003f1c
