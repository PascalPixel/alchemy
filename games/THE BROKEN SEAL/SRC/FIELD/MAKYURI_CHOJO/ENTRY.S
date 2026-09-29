@ Entry veneers opening the MAKYURI_CHOJO overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — MAKYURI_CHOJO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer MakyuriChojo_ApplyEntryState
	overlay_veneer SceneData_GetTableB938
	overlay_veneer SceneData_GetTableb9c8
	overlay_veneer SceneData_GetTableB9d4AfterStateCheck
	overlay_veneer SceneData_GetTablebbe4
	overlay_veneer get_runtime_default_result
