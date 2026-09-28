@ Entry veneers opening the SORU_STAR overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SORU_STAR overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer SoruStar_ApplyEntryState
	overlay_veneer SceneData_GetTableCd88
	overlay_veneer SceneData_GetTableCDB8
	overlay_veneer SceneData_GetTableCdc4
	overlay_veneer SceneData_GetTableCFA4
	overlay_veneer SceneData_ReturnZero
