@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer XianInteriors_PrepareWindowTransition
	overlay_veneer XianInteriors_GetEntrances
	overlay_veneer XianInteriors_GetExits
	overlay_veneer XianInteriors_SelectPlacements
	overlay_veneer XianInteriors_SelectEvents
	overlay_veneer SceneData_ReturnZero
