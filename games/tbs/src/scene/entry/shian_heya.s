@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ShianHeya_PrepareWindowTransition
	overlay_veneer ShianHeya_GetEntrances
	overlay_veneer ShianHeya_GetExits
	overlay_veneer ShianHeya_SelectPlacements
	overlay_veneer ShianHeya_SelectEvents
	overlay_veneer SceneData_ReturnZero
