@ Entry veneers opening the VINASU_CHOJO overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — VINASU_CHOJO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_InitActorsAndDispatchBySubstate
	overlay_veneer SceneData_GetTablee3d4
	overlay_veneer SceneData_GetTablee464
	overlay_veneer SceneData_GetTablee478
	overlay_veneer SceneData_GetTableE6ec
	overlay_veneer SceneData_ReturnZero
