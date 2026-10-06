.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_Initialize
	overlay_veneer Scene_GetEntrances
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
