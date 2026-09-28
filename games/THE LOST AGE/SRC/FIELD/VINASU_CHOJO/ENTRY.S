@ Entry veneers opening the VINASU_CHOJO overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — VINASU_CHOJO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000a54
	overlay_veneer Func_02000650
	overlay_veneer Func_0200065c
	overlay_veneer Func_02000684
	overlay_veneer Func_02000a0c
	overlay_veneer Func_02000658
	overlay_veneer Func_02000b28
