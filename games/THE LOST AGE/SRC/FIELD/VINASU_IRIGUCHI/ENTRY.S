@ Entry veneers opening the VINASU_IRIGUCHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — VINASU_IRIGUCHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000128
	overlay_veneer Func_02000038
	overlay_veneer Func_02000044
	overlay_veneer Func_0200004c
	overlay_veneer Func_02000120
	overlay_veneer Func_02000040
	overlay_veneer Func_02000308
