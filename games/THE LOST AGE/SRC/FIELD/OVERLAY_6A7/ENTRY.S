@ Entry veneers opening the OVERLAY_6A7 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6A7 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000618
	overlay_veneer Func_02000048
	overlay_veneer Func_02000054
	overlay_veneer Func_0200005c
	overlay_veneer Func_02000610
	overlay_veneer Func_02000050
	overlay_veneer Func_02000680
