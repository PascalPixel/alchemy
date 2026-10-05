.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ToretoEda_InitializeBackdropTransition
	overlay_veneer ToretoEda_GetEntrances
	overlay_veneer ToretoEda_GetExits
	overlay_veneer ToretoEda_GetPlacements
	overlay_veneer ToretoEda_GetEvents
	overlay_veneer SceneData_ReturnZero
