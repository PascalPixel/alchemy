@ Entry veneers opening the OVERLAY_6B0 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6B0 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001e64
	overlay_veneer Func_02000804
	overlay_veneer Func_02000810
	overlay_veneer Func_02000818
	overlay_veneer Func_02001bec
	overlay_veneer Func_0200080c
	overlay_veneer Func_020026fc
