@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_Initialize
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectTableByFlag9a7
	overlay_veneer SceneData_SelectSecondaryTableByFlag9a7
	overlay_veneer SceneData_ReturnZero
