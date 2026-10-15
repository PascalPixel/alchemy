@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer FieldScene_SetupEntryLayoutsBySelector
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetPreparedRecords
	overlay_veneer SceneData_GetFourthTable
	overlay_veneer SceneData_ReturnZero
