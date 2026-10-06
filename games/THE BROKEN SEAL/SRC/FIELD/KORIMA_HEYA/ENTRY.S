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
