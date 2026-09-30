@ Entry veneers opening the OVERLAY_656 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_656 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001abc
	overlay_veneer Func_02000038
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020000b8
	overlay_veneer Func_020004f4
	overlay_veneer Func_02000078
	overlay_veneer Func_02001c38
