@ Entry veneers opening the SUHARA_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer SceneState_InitEntryWorkspaceAndFlag96f
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectActorTableByFlag96f
	overlay_veneer SceneData_SelectExtraTableByFlag96f
	overlay_veneer SceneData_ReturnZero
