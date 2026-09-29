@ Entry veneers opening the ARUTIN_YAMA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — ARUTIN_YAMA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02002020
	overlay_veneer Func_020000e4
	overlay_veneer SceneData_GetTableC85c
	overlay_veneer Func_0200020c
	overlay_veneer Func_02001678
	overlay_veneer Func_020001c8
