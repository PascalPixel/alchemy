@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ToretoEda_InitializeBackdropTransition
	overlay_veneer ToretoEda_GetEntrances
	overlay_veneer ToretoEda_GetExits
	overlay_veneer ToretoEda_GetPlacements
	overlay_veneer ToretoEda_GetEvents
	overlay_veneer SceneData_ReturnZero
