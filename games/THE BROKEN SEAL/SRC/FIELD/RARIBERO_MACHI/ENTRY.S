.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_Initialize
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectTableByFlag9a7
	overlay_veneer SceneData_SelectSecondaryTableByFlag9a7
	overlay_veneer SceneData_ReturnZero
