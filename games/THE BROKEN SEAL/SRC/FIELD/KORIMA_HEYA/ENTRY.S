@ Entry veneers opening the KORIMA_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KORIMA_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_SetupEntryLayoutsBySelector
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetPreparedRecords
	overlay_veneer SceneData_GetFourthTable
	overlay_veneer SceneData_ReturnZero
