@ Entry veneers opening the GOMA_IRIGUCHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — GOMA_IRIGUCHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer GomaIriguchi_RestoreEntryState
	overlay_veneer SceneData_GetTable92f8
	overlay_veneer SceneData_GetTable9358
	overlay_veneer SceneData_GetTable9368
	overlay_veneer SceneData_GetTable9488
	overlay_veneer SceneData_ReturnZero
