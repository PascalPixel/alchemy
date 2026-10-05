.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ArutamiraDou_ApplyEntryState
	overlay_veneer ArutamiraDou_GetEntrances
	overlay_veneer ArutamiraDou_GetExits
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
