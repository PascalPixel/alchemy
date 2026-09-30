@ Entry veneers opening the OVERLAY_661 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_661 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003094
	overlay_veneer Func_0200028c
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020002f8
	overlay_veneer Func_020003c8
	overlay_veneer Func_020002b8
	overlay_veneer Func_02003168
