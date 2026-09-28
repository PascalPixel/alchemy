@ Entry veneers opening the WORLD_MAP overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — WORLD_MAP overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000dc4
	overlay_veneer Func_02000290
	overlay_veneer Func_0200029c
	overlay_veneer Func_020003f8
	overlay_veneer Func_02000dbc
	overlay_veneer Func_02000298
	overlay_veneer Func_020011f8
