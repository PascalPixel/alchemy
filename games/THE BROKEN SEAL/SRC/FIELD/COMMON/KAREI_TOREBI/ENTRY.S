@ Entry veneers opening the KAREI_TOREBI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KAREI_TOREBI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200086c
	overlay_veneer Func_02000040
	overlay_veneer SceneData_GetTable98a0
	overlay_veneer Func_020000a0
	overlay_veneer Func_0200019c
	overlay_veneer SceneData_ReturnZero
