@ Entry veneers opening the OVERLAY_664 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_664 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020051f8
	overlay_veneer Func_020025cc
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02002604
	overlay_veneer Func_02004c00
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02005cb8
