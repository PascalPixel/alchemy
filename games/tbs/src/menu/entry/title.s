@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Title_Run
	overlay_veneer Title_GetEntrances
	overlay_veneer Title_GetExits
	overlay_veneer Title_GetPlacements
	overlay_veneer Title_GetEvents
	overlay_veneer SceneData_ReturnZero
