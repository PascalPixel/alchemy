.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer Title_Run
	overlay_veneer Title_GetEntrances
	overlay_veneer Title_GetExits
	overlay_veneer Title_GetPlacements
	overlay_veneer Title_GetEvents
	overlay_veneer SceneData_ReturnZero
