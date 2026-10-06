.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer CommandTable_ConfigureCommandList
	overlay_veneer MenuTest_GetEntrances
	overlay_veneer MenuTest_GetExits
	overlay_veneer MenuTest_GetPlacements
	overlay_veneer MenuTest_GetEvents
	overlay_veneer SceneData_ReturnZero
