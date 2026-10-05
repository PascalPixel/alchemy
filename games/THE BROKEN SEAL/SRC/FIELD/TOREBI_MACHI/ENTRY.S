.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer TorebiMachi_ApplyEntryState
	overlay_veneer TorebiMachi_GetEntrances
	overlay_veneer TorebiMachi_GetExits
	overlay_veneer TorebiMachi_GetPlacements
	overlay_veneer TorebiMachi_SelectEvents
	overlay_veneer SceneData_ReturnZero
