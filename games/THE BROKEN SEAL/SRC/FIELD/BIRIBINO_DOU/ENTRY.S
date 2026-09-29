@ Entry veneers opening the BIRIBINO_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BIRIBINO_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200046c
	overlay_veneer Func_02000040
	overlay_veneer SceneData_GetEventTable
	overlay_veneer Func_020000a0
	overlay_veneer Func_020000f4
	overlay_veneer SceneData_ReturnZero
