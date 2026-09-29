@ Entry veneers opening the KOROSSEO_KAWA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KOROSSEO_KAWA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer KorosseoKawa_ApplyEntryState
	overlay_veneer SceneData_GetTableC194
	overlay_veneer SceneData_GetTablec1dc
	overlay_veneer SceneData_GetTablec1f4
	overlay_veneer SceneData_GetTablec420
	overlay_veneer get_default_result
