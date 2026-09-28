@ Entry veneers opening the VINASU_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — VINASU_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003068
	overlay_veneer Func_02000e04
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer Func_02000e88
	overlay_veneer Func_02002f8c
	overlay_veneer SceneData_ReturnZero
