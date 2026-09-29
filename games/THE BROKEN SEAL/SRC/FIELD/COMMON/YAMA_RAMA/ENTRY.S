@ Entry veneers opening the YAMA_RAMA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — YAMA_RAMA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020011b0
	overlay_veneer Func_0200007c
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020000b8
	overlay_veneer Func_02001180
	overlay_veneer Scene_GetRegions
