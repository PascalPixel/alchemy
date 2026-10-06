.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShianHeya_PrepareWindowTransition
	overlay_veneer ShianHeya_GetEntrances
	overlay_veneer ShianHeya_GetExits
	overlay_veneer ShianHeya_SelectPlacements
	overlay_veneer ShianHeya_SelectEvents
	overlay_veneer SceneData_ReturnZero
