@ Entry veneers opening the MAKYURI_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — MAKYURI_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003d20
	overlay_veneer Func_02000e20
	overlay_veneer SceneData_GetTableE614
	overlay_veneer Func_02000e80
	overlay_veneer Func_02000f30
	overlay_veneer SceneData_ReturnZero
